//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "HourglassStabilization.h"

// MOOSE includes
#include "ElasticityTensorTools.h"
#include "MooseVariable.h"

#include "libmesh/elem.h"
#include "libmesh/enum_to_string.h"
#include "libmesh/int_range.h"
#include "libmesh/quadrature.h"

#include <cmath>

registerMooseObject("SolidMechanicsApp", HourglassStabilization);

namespace
{
/// Classical Flanagan-Belytschko hourglass mode vectors in libMesh node ordering
const std::vector<std::vector<Real>> quad4_hourglass_modes = {{1.0, -1.0, 1.0, -1.0}};
const std::vector<std::vector<Real>> hex8_hourglass_modes = {
    {1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0},
    {1.0, 1.0, -1.0, -1.0, -1.0, -1.0, 1.0, 1.0},
    {1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0},
    {-1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0}};

/// Relative floor on |det(A)| below which the reference geometry matrix is treated as singular.
/// This threshold is an implementation detail and not part of the object's interface.
constexpr Real det_relative_tol = 1.0e-12;

/**
 * Invert the leading n_dim x n_dim block of the symmetric reference geometry matrix by its
 * adjugate, substituting the isotropic inverse I / h2 when that block is singular.
 * @param A the reference geometry matrix, with only its leading n_dim x n_dim block populated
 * @param n_dim the element's spatial dimension
 * @param h2 the squared length scale trace(A) / n_dim
 * @return the inverse of the leading block, with the remaining entries left at zero
 */
RealTensorValue
invertReferenceGeometry(const RealTensorValue & A, const unsigned int n_dim, const Real h2)
{
  mooseAssert(n_dim == 2 || n_dim == 3, "Hourglass geometry is two- or three-dimensional");

  // For n_dim == 3 the whole tensor is populated, so libMesh's own determinant and inverse apply.
  // The n_dim == 2 case has no counterpart there -- libMesh's tensor dimension is fixed at compile
  // time -- so the leading 2x2 block is inverted by its adjugate here.
  RealTensorValue adjugate;
  Real det = 0.0;
  if (n_dim == 2)
  {
    adjugate(0, 0) = A(1, 1);
    adjugate(0, 1) = -A(0, 1);
    adjugate(1, 0) = -A(1, 0);
    adjugate(1, 1) = A(0, 0);
    det = A(0, 0) * adjugate(0, 0) + A(0, 1) * adjugate(1, 0);
  }
  else
    det = A.det();

  // A well-conditioned block has |det| of the order of h2^n_dim; far below that the element is
  // degenerate. The isotropic inverse is the exact inverse for a square or a cube and keeps the
  // projection finite, including on an element collapsed to a point, where h2 vanishes as well.
  if (std::abs(det) <= det_relative_tol * std::pow(h2, static_cast<Real>(n_dim)))
  {
    RealTensorValue isotropic;
    if (h2 > 0.0)
      for (const auto a : make_range(n_dim))
        isotropic(a, a) = 1.0 / h2;
    return isotropic;
  }

  return n_dim == 2 ? adjugate / det : A.inverse();
}
}

InputParameters
HourglassStabilization::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addClassDescription(
      "Hourglass stabilization of one scalar field on QUAD4 and HEX8 elements integrated with a "
      "single quadrature point, using Flanagan-Belytschko mode vectors projected onto the "
      "reference geometry so that the tangent is exact and affine fields are left unstabilized.");
  params.addRangeCheckedParam<Real>(
      "penalty", 0.1, "penalty >= 0", "Dimensionless hourglass stabilization coefficient.");
  MooseEnum stiffness_source("elasticity_tensor scalar_property", "elasticity_tensor");
  params.addParam<MooseEnum>(
      "stiffness_source",
      stiffness_source,
      "Which material property the stabilization takes its stiffness scale from: the isotropic "
      "shear modulus of a rank-four elasticity tensor, or a scalar property named by "
      "'scalar_stiffness'.");
  params.addParam<MaterialPropertyName>(
      "elasticity_tensor",
      "elasticity_tensor",
      "The name of the elasticity tensor the stabilization takes its shear modulus from. Used only "
      "when stiffness_source = elasticity_tensor.");
  params.addParam<MaterialPropertyName>(
      "scalar_stiffness",
      "The name of the scalar material property the stabilization is scaled by, for example a "
      "thermal conductivity. Required when stiffness_source = scalar_property.");
  params.addParam<std::string>("base_name", "Material property base name");
  // The reference-configuration geometry is what makes the tangent exact and leaves the
  // stabilization coupled to no other variable, so this kernel pins the frame rather than
  // offering it as a choice.
  params.set<bool>("use_displaced_mesh") = false;
  params.suppressParameter<bool>("use_displaced_mesh");
  return params;
}

HourglassStabilization::HourglassStabilization(const InputParameters & parameters)
  : Kernel(parameters),
    _penalty(getParam<Real>("penalty")),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _use_elasticity_tensor(getParam<MooseEnum>("stiffness_source") == "elasticity_tensor"),
    // The elasticity tensor is a base_name-disambiguated property of the solid mechanics family, so
    // it carries the prefix; scalar_stiffness is an outside property and is read by its given name.
    _elasticity_tensor(_use_elasticity_tensor
                           ? &getMaterialPropertyByName<RankFourTensor>(
                                 _base_name + getParam<MaterialPropertyName>("elasticity_tensor"))
                           : nullptr),
    // Only declared once the parameter is known to be set, so the paramError below is reached
    // instead of an error from inside the property lookup.
    // An AD-declared property (ADHeatConductionMaterial, ADComputeIsotropicElasticityTensor) cannot
    // be retrieved as non-AD, so bind whichever form the supplying material actually declared. Only
    // the value is used either way, which is exactly the intent -- see the tangent caveat below.
    _scalar_stiffness(
        !_use_elasticity_tensor && isParamValid("scalar_stiffness") &&
                !hasADMaterialPropertyByName<Real>(
                    getParam<MaterialPropertyName>("scalar_stiffness"))
            ? &getMaterialPropertyByName<Real>(getParam<MaterialPropertyName>("scalar_stiffness"))
            : nullptr),
    _ad_scalar_stiffness(
        !_use_elasticity_tensor && isParamValid("scalar_stiffness") &&
                hasADMaterialPropertyByName<Real>(
                    getParam<MaterialPropertyName>("scalar_stiffness"))
            ? &getADMaterialPropertyByName<Real>(getParam<MaterialPropertyName>("scalar_stiffness"))
            : nullptr),
    _n_modes(0),
    _c(0.0)
{
  if (!_use_elasticity_tensor && !isParamValid("scalar_stiffness"))
    paramError("scalar_stiffness",
               "A scalar stiffness property must be supplied when stiffness_source = "
               "scalar_property.");

  if (!_use_elasticity_tensor && isParamSetByUser("elasticity_tensor"))
    paramError("elasticity_tensor",
               "elasticity_tensor is set but stiffness_source = scalar_property, so it would be "
               "ignored. Set stiffness_source = elasticity_tensor to use it.");

  if (_use_elasticity_tensor && isParamValid("scalar_stiffness"))
    paramError("scalar_stiffness",
               "scalar_stiffness is set but stiffness_source = elasticity_tensor, so it would "
               "be ignored. Set stiffness_source = scalar_property to use it.");
}

const std::string &
HourglassStabilization::quadratureRemediationSnippet()
{
  static const std::string snippet = "[Executioner]\n"
                                     "  [Quadrature]\n"
                                     "    type = GAUSS\n"
                                     "    element_order = CONSTANT\n"
                                     "  []\n"
                                     "[]\n";
  return snippet;
}

const std::vector<std::vector<Real>> *
HourglassStabilization::modeVectors(const libMesh::ElemType elem_type)
{
  switch (elem_type)
  {
    case libMesh::QUAD4:
      return &quad4_hourglass_modes;

    case libMesh::HEX8:
      return &hex8_hourglass_modes;

    default:
      return nullptr;
  }
}

void
HourglassStabilization::computeProjectedModes(const std::vector<std::vector<Real>> & gamma,
                                              const std::vector<Point> & ref_coords,
                                              const unsigned int n_dim,
                                              std::vector<std::vector<Real>> & ghat,
                                              Real & h2)
{
  mooseAssert(n_dim == 2 || n_dim == 3, "Hourglass geometry is two- or three-dimensional");
  mooseAssert(!ref_coords.empty(), "An element has at least one node");

  const auto n_nodes = ref_coords.size();

  Point average;
  for (const auto & coord : ref_coords)
    average += coord;
  average /= static_cast<Real>(n_nodes);

  // d_j = ref_coords[j] - average is formed inline at each use rather than cached, so this
  // function allocates nothing on a path that runs once per element per residual and Jacobian.
  RealTensorValue A;
  for (const auto i : index_range(ref_coords))
  {
    const Point d = ref_coords[i] - average;
    for (const auto a : make_range(n_dim))
      for (const auto b : make_range(n_dim))
        A(a, b) += d(a) * d(b);
  }

  Real trace = 0.0;
  for (const auto a : make_range(n_dim))
    trace += A(a, a);
  h2 = trace / n_dim;

  const RealTensorValue M = invertReferenceGeometry(A, n_dim, h2);

  ghat.resize(gamma.size());
  for (const auto m : index_range(gamma))
  {
    mooseAssert(gamma[m].size() == n_nodes, "Each mode vector carries one entry per element node");

    // The linear part of the mode, p^m = sum_j gamma^m_j d_j. The classical modes already have zero
    // mean, so removing the linear part is all that the affine projection needs.
    RealVectorValue p;
    for (const auto j : index_range(ref_coords))
    {
      const Point d = ref_coords[j] - average;
      for (const auto a : make_range(n_dim))
        p(a) += gamma[m][j] * d(a);
    }

    const RealVectorValue q = M * p;

    ghat[m].resize(n_nodes);
    for (const auto i : index_range(ref_coords))
      ghat[m][i] = gamma[m][i] - q * (ref_coords[i] - average);
  }
}

void
HourglassStabilization::computeElementGeometry()
{
  if (_qrule->n_points() != 1)
    mooseError("Hourglass stabilization requires a single-point quadrature rule, but the rule on "
               "this block has ",
               _qrule->n_points(),
               " points. Request one-point quadrature with\n\n",
               quadratureRemediationSnippet());

  const auto elem_type = _current_elem->type();
  const auto * const gamma = modeVectors(elem_type);
  if (!gamma)
    mooseError("Hourglass stabilization is only defined for QUAD4 and HEX8 elements, but this "
               "block contains ",
               libMesh::Utility::enum_to_string(elem_type),
               " elements. TRI3 and TET4 have a constant gradient over the element and so are not "
               "rank-deficient under one-point quadrature, and higher-order elements have a "
               "different zero-energy mode structure. Restrict this kernel to the QUAD4 and HEX8 "
               "blocks with 'block', and leave the remaining blocks at full quadrature.");

  const auto n_nodes = _current_elem->n_nodes();
  const auto & u_nodal = _var.dofValues();
  if (u_nodal.size() != n_nodes)
    mooseError("Hourglass stabilization needs one nodal value of '",
               _var.name(),
               "' per element node, but this ",
               libMesh::Utility::enum_to_string(elem_type),
               " element carries ",
               u_nodal.size(),
               " degrees of freedom of that variable for ",
               n_nodes,
               " nodes. Use a first order Lagrange variable.");

  mooseAssert(_test.size() == n_nodes, "The test functions are indexed by element node");

  // use_displaced_mesh is pinned false, so the element's nodes carry reference coordinates.
  _ref_coords.resize(n_nodes);
  for (const auto i : make_range(n_nodes))
    _ref_coords[i] = _current_elem->node_ref(i);

  // computeProjectedModes builds the reference geometry matrix from the leading dim() components of
  // the nodal coordinates, so a two-dimensional element is projected onto the global x and y axes.
  // A QUAD4 lying outside a plane of constant z would make that matrix singular, fall through to
  // the degenerate-geometry branch, and yield a mode that annihilates neither the in-plane nor the
  // out-of-plane affine field -- a wrong answer with no diagnostic. Reject that orientation.
  const auto n_dim = _current_elem->dim();
  if (n_dim == 2)
  {
    Real out_of_plane = 0.0;
    Real extent = 0.0;
    for (const auto i : make_range(n_nodes))
    {
      out_of_plane = std::max(out_of_plane, std::abs(_ref_coords[i](2) - _ref_coords[0](2)));
      extent = std::max(extent, (_ref_coords[i] - _ref_coords[0]).norm());
    }
    if (extent > 0.0 && out_of_plane > 1.0e-8 * extent)
      mooseError("Hourglass stabilization projects a two-dimensional element onto the global x and "
                 "y axes, so a QUAD4 must lie in a plane of constant z. This element spans ",
                 out_of_plane,
                 " in z over an extent of ",
                 extent,
                 ". Rotate the mesh into a plane of constant z, or use HEX8 elements.");
  }

  Real h2 = 0.0;
  computeProjectedModes(*gamma, _ref_coords, n_dim, _ghat, h2);
  _n_modes = _ghat.size();

  // The stiffness scale is treated as solution-independent in the tangent, which holds for any
  // ordinary linear setup. With a solution-dependent stiffness the d(stiffness)/du term is missing
  // and the tangent becomes inexact.
  const Real stiffness =
      _use_elasticity_tensor
          ? ElasticityTensorTools::getIsotropicShearModulus((*_elasticity_tensor)[0])
          : (_scalar_stiffness ? (*_scalar_stiffness)[0]
                               : MetaPhysicL::raw_value((*_ad_scalar_stiffness)[0]));

  // An element collapsed to a point has no length scale to stabilize against.
  _c = h2 > 0.0 ? _penalty * stiffness / h2 : 0.0;
}

void
HourglassStabilization::precalculateResidual()
{
  computeElementGeometry();

  const auto & u_nodal = _var.dofValues();
  _H.assign(_n_modes, 0.0);
  for (const auto m : make_range(_n_modes))
    for (const auto j : make_range(u_nodal.size()))
      _H[m] += _ghat[m][j] * u_nodal[j];
}

void
HourglassStabilization::precalculateJacobian()
{
  computeElementGeometry();
}

Real
HourglassStabilization::computeQpResidual()
{
  // Assembly multiplies by _JxW[_qp] * _coord[_qp] once. On a QUAD4 the one-point rule integrates
  // the bilinear Jacobian determinant exactly, so that product is the element measure and the
  // integrated coefficient is penalty * stiffness * V / h2. A distorted HEX8 determinant carries
  // higher-order terms, so there the measure -- and the coefficient -- hold approximately.
  Real r = 0.0;
  for (const auto m : make_range(_n_modes))
    r += _ghat[m][_i] * _H[m];

  return _c * r;
}

Real
HourglassStabilization::computeQpJacobian()
{
  Real j = 0.0;
  for (const auto m : make_range(_n_modes))
    j += _ghat[m][_i] * _ghat[m][_j];

  return _c * j;
}
