//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "Kernel.h"
#include "RankFourTensor.h"

#include "libmesh/enum_elem_type.h"
#include "libmesh/point.h"

/**
 * Hourglass stabilization of one scalar first order Lagrange field on a QUAD4 or HEX8 element
 * integrated with a single quadrature point.
 *
 * Underintegrating a QUAD4 or a HEX8 leaves the element operator rank-deficient: at one quadrature
 * point the discrete gradient operator of a second-order operator is n_dim x n, so the element
 * operator reaches rank n_dim at most where rank n - 1 is needed. A QUAD4 supplies rank 2 of the
 * required 3 and so admits one spurious zero-energy hourglass mode; a HEX8 supplies rank 3 of the
 * required 7 and admits four. That nullspace is a property of the element and the quadrature rule
 * alone, not of the physics, so the same construction stabilizes a displacement component, a
 * temperature, or any other field carrying a second-order operator.
 *
 * This kernel penalizes the amplitude of the classical Flanagan-Belytschko mode vectors after
 * projecting the affine space out of them, which restores rank while leaving every affine field,
 * and therefore the patch test and rigid-body motion, exactly unstabilized.
 *
 * All geometry is taken from the reference configuration, so the projected mode vectors and the
 * length scale are geometry-only. When the stiffness material property is solution-independent too
 * -- the ordinary linear-elastic or constant-conductivity case -- computeQpJacobian() is the exact
 * derivative of the residual and the stabilization couples no other variable, so the off-diagonal
 * tangent is identically zero. With a solution-dependent stiffness (a variable elasticity tensor,
 * or k(T)) the d(stiffness)/du term is missing: Newton still converges as an inexact method, but
 * -snes_test_jacobian is no longer clean, and for k(T) on a stabilized temperature the omission
 * lands in this kernel's own diagonal block rather than off-diagonal. The other cost is that the
 * stabilization is scaled by the undeformed geometry and so does not track large deformation.
 *
 * The kernel does not set the quadrature rule. MOOSE's object-level quadrature API can only raise
 * the order and never lower it, so the user requests one-point quadrature through
 * [Executioner][Quadrature] and this kernel validates the rule it is given.
 */
class HourglassStabilization : public Kernel
{
public:
  static InputParameters validParams();

  HourglassStabilization(const InputParameters & parameters);

  /**
   * The pasteable [Executioner][Quadrature] block that requests one-point quadrature. Shared with
   * QuasiStaticSolidMechanicsPhysics so the remediation text in this kernel's run-time error and
   * the action's early diagnostic cannot drift apart. Callers supply their own separation ahead of
   * it.
   * @return the block text, terminated by a newline
   */
  static const std::string & quadratureRemediationSnippet();

  /**
   * The classical Flanagan-Belytschko hourglass mode vectors of an element type, in libMesh node
   * ordering: the single xi*eta mode for QUAD4, and the xi*eta, eta*zeta, xi*zeta and xi*eta*zeta
   * modes for HEX8.
   * @param elem_type the element type to look up
   * @return gamma[m][i], the m-th mode's value at node i, or nullptr for an element type that has
   *         no Flanagan-Belytschko hourglass construction
   */
  static const std::vector<std::vector<Real>> * modeVectors(const libMesh::ElemType elem_type);

  /**
   * Project the affine space out of the classical mode vectors using the element's reference
   * geometry. This is a pure function of its arguments, which is what makes the stabilization's
   * Jacobian exact.
   *
   * With d_i the node positions relative to their average, A = sum_i d_i d_i^T and M = A^-1, the
   * linear part of each mode, p^m = sum_j gamma^m_j d_j, is removed as
   * ghat^m_i = gamma^m_i - (p^m)^T M d_i. The classical modes already have zero mean, so removing
   * the linear part is enough to make them orthogonal to that space. When A is singular the
   * isotropic inverse I / h2 is used instead, which keeps ghat finite. That branch covers a
   * genuinely degenerate element, but it is also reached by a well-shaped element whose nodes do
   * not span n_dim of the global axes -- the caller rejects that case rather than rely on it.
   *
   * @param gamma the classical mode vectors from modeVectors()
   * @param ref_coords the reference (undeformed) coordinates of the element's nodes
   * @param n_dim the element's topological dimension: 2 for QUAD4, 3 for HEX8. A is built from the
   *        leading n_dim global coordinate components, so a 2D element must lie in constant z.
   * @param ghat filled with the projected mode vectors, ghat[m][i]
   * @param h2 filled with the rotation-invariant squared length scale trace(A) / n_dim
   */
  static void computeProjectedModes(const std::vector<std::vector<Real>> & gamma,
                                    const std::vector<Point> & ref_coords,
                                    const unsigned int n_dim,
                                    std::vector<std::vector<Real>> & ghat,
                                    Real & h2);

protected:
  virtual void precalculateResidual() override;
  virtual void precalculateJacobian() override;

  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;

private:
  /**
   * Validate the quadrature rule, the element type and the variable's element dof count, then
   * the projected mode vectors and the stabilization scale of the current element. Called once per
   * element from both precalculate hooks so the residual and Jacobian paths cannot drift.
   */
  void computeElementGeometry();

  /// Dimensionless hourglass stabilization coefficient
  const Real _penalty;

  /// Base name prefix of the material properties this kernel reads
  const std::string _base_name;

  /// Whether the stabilization scale comes from an elasticity tensor instead of a scalar property
  const bool _use_elasticity_tensor;

  /// Elasticity tensor supplying the shear modulus, or nullptr when a scalar property is used
  const MaterialProperty<RankFourTensor> * const _elasticity_tensor;

  /// Scalar stiffness the stabilization is scaled by, or nullptr when the elasticity tensor is
  /// used or when the property was declared as AD
  const MaterialProperty<Real> * const _scalar_stiffness;

  /// Set instead of _scalar_stiffness when the named scalar property was declared AD. Only its
  /// value is read: the tangent already treats the stiffness as solution-independent.
  const ADMaterialProperty<Real> * const _ad_scalar_stiffness;

  /// Number of hourglass modes of the current element
  unsigned int _n_modes;

  /// Reference coordinates of the current element's nodes
  std::vector<Point> _ref_coords;

  /// Projected mode vectors of the current element, ghat[m][i]
  std::vector<std::vector<Real>> _ghat;

  /// Amplitude of each hourglass mode in the current elemental solution, H[m]
  std::vector<Real> _H;

  /// Pointwise stabilization scale, penalty * stiffness / h2
  Real _c;
};
