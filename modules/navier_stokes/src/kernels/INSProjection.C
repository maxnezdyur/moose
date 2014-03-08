<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "INSProjection.h"
#include "MooseMesh.h"
#include "NS.h"

registerMooseObject("NavierStokesApp", INSProjection);

InputParameters
INSProjection::validParams()
{
  InputParameters params = Kernel::validParams();

  params.addClassDescription("This class computes the 'projection' part of the 'split' method for "
                             "solving incompressible Navier-Stokes.");
=======
#include "INSProjection.h"

template<>
InputParameters validParams<INSProjection>()
{
  InputParameters params = validParams<Kernel>();

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  // Coupled variables
  params.addRequiredCoupledVar("a1", "x-acceleration");
  params.addCoupledVar("a2", "y-acceleration"); // only required in 2D and 3D
  params.addCoupledVar("a3", "z-acceleration"); // only required in 3D
<<<<<<< HEAD
  params.addRequiredCoupledVar(NS::pressure, "pressure");
  params.addDeprecatedCoupledVar("p", NS::pressure, "1/1/2022");

  // Required parameters
  params.addRequiredParam<unsigned>(
      "component",
      "0,1,2 depending on if we are solving the x,y,z component of the momentum equation");

  // Optional parameters
  params.addParam<MaterialPropertyName>("rho_name", "rho", "density name");
=======
  params.addRequiredCoupledVar("p", "pressure");

  // Required parameters
  params.addRequiredParam<Real>("rho", "density");
  params.addRequiredParam<unsigned>("component", "0,1,2 depending on if we are solving the x,y,z component of the momentum equation");
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  return params;
}

<<<<<<< HEAD
INSProjection::INSProjection(const InputParameters & parameters)
  : Kernel(parameters),

    // Coupled variables
    _a1(coupledValue("a1")),
    _a2(_mesh.dimension() >= 2 ? coupledValue("a2") : _zero),
    _a3(_mesh.dimension() == 3 ? coupledValue("a3") : _zero),

    // Gradients
    _grad_p(coupledGradient(NS::pressure)),

    // Variable numberings
    _a1_var_number(coupled("a1")),
    _a2_var_number(_mesh.dimension() >= 2 ? coupled("a2") : libMesh::invalid_uint),
    _a3_var_number(_mesh.dimension() == 3 ? coupled("a3") : libMesh::invalid_uint),
    _p_var_number(coupled(NS::pressure)),

    // Required parameters
    _component(getParam<unsigned>("component")),

    // Material properties
    _rho(getMaterialProperty<Real>("rho_name"))
{
}

Real
INSProjection::computeQpResidual()
=======


INSProjection::INSProjection(const std::string & name, InputParameters parameters) :
  Kernel(name, parameters),

  // Coupled variables
  _a1(coupledValue("a1")),
  _a2(_mesh.dimension() >= 2 ? coupledValue("a2") : _zero),
  _a3(_mesh.dimension() == 3 ? coupledValue("a3") : _zero),

  // Gradients
  _grad_p(coupledGradient("p")),

  // Variable numberings
  _a1_var_number(coupled("a1")),
  _a2_var_number(_mesh.dimension() >= 2 ? coupled("a2") : libMesh::invalid_uint),
  _a3_var_number(_mesh.dimension() == 3 ? coupled("a3") : libMesh::invalid_uint),
  _p_var_number(coupled("p")),

  // Required parameters
  _rho(getParam<Real>("rho")),
  _component(getParam<unsigned>("component"))
{
}



Real INSProjection::computeQpResidual()
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
  // Vector object for a
  RealVectorValue a(_a1[_qp], _a2[_qp], _a3[_qp]);

  // Vector object for test function (only the component'th entry is non-zero)
  RealVectorValue test;
  test(_component) = _test[_i][_qp];

  // "Symmetric" part, -a.test
  Real symmetric_part = -a(_component) * _test[_i][_qp];

<<<<<<< HEAD
  // The pressure part, (1/_rho[_qp]) * (grad(p).v)
  Real pressure_part = (1. / _rho[_qp]) * (_grad_p[_qp] * test);
=======
  // The pressure part, (1/_rho) * (grad(p).v)
  Real pressure_part = (1./_rho) * (_grad_p[_qp] * test);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  // Return the result
  return symmetric_part + pressure_part;
}

<<<<<<< HEAD
Real
INSProjection::computeQpJacobian()
=======



Real INSProjection::computeQpJacobian()
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
  // There will be a diagonal component from the time derivative term...
  return 0.;
}

<<<<<<< HEAD
Real
INSProjection::computeQpOffDiagJacobian(unsigned jvar)
=======



Real INSProjection::computeQpOffDiagJacobian(unsigned jvar)
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
  if (((jvar == _a1_var_number) && (_component == 0)) ||
      ((jvar == _a2_var_number) && (_component == 1)) ||
      ((jvar == _a3_var_number) && (_component == 2)))
  {
    // The symmetric term's Jacobian is only non-zero when the
    // component of 'a' being differentiated is the same as _component.
    return -_phi[_j][_qp] * _test[_i][_qp];
  }

  else if (jvar == _p_var_number)
  {
<<<<<<< HEAD
    return (1. / _rho[_qp]) * (_grad_phi[_j][_qp](_component) * _test[_i][_qp]);
=======
    return (1./_rho) * (_grad_phi[_j][_qp](_component) * _test[_i][_qp]);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  }

  else
    return 0;
}
