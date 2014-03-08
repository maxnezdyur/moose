<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "INSPressurePoisson.h"
#include "MooseMesh.h"

registerMooseObject("NavierStokesApp", INSPressurePoisson);

InputParameters
INSPressurePoisson::validParams()
{
  InputParameters params = Kernel::validParams();

  params.addClassDescription("This class computes the pressure Poisson solve which is part of the "
                             "'split' scheme used for solving the incompressible Navier-Stokes "
                             "equations.");
=======
#include "INSPressurePoisson.h"

template<>
InputParameters validParams<INSPressurePoisson>()
{
  InputParameters params = validParams<Kernel>();

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  // Coupled variables
  params.addRequiredCoupledVar("a1", "x-acceleration");
  params.addCoupledVar("a2", "y-acceleration"); // only required in 2D and 3D
  params.addCoupledVar("a3", "z-acceleration"); // only required in 3D

<<<<<<< HEAD
  // Optional parameters
  params.addParam<MaterialPropertyName>("rho_name", "rho", "density_name");
=======
  // Required parameters
  params.addRequiredParam<Real>("rho", "density");
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  return params;
}

<<<<<<< HEAD
INSPressurePoisson::INSPressurePoisson(const InputParameters & parameters)
  : Kernel(parameters),

    // Gradients
    _grad_a1(coupledGradient("a1")),
    _grad_a2(_mesh.dimension() >= 2 ? coupledGradient("a2") : _grad_zero),
    _grad_a3(_mesh.dimension() == 3 ? coupledGradient("a3") : _grad_zero),

    // Variable numberings
    _a1_var_number(coupled("a1")),
    _a2_var_number(_mesh.dimension() >= 2 ? coupled("a2") : libMesh::invalid_uint),
    _a3_var_number(_mesh.dimension() == 3 ? coupled("a3") : libMesh::invalid_uint),

    // Material Properties
    _rho(getMaterialProperty<Real>("rho_name"))
{
}

Real
INSPressurePoisson::computeQpResidual()
=======


INSPressurePoisson::INSPressurePoisson(const std::string & name, InputParameters parameters) :
  Kernel(name, parameters),

  // Gradients
  _grad_a1(coupledGradient("a1")),
  _grad_a2(_mesh.dimension() >= 2 ? coupledGradient("a2") : _grad_zero),
  _grad_a3(_mesh.dimension() == 3 ? coupledGradient("a3") : _grad_zero),

  // Variable numberings
  _a1_var_number(coupled("a1")),
  _a2_var_number(_mesh.dimension() >= 2 ? coupled("a2") : libMesh::invalid_uint),
  _a3_var_number(_mesh.dimension() == 3 ? coupled("a3") : libMesh::invalid_uint),

  // Required parameters
  _rho(getParam<Real>("rho"))
{
}



Real INSPressurePoisson::computeQpResidual()
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
  // Laplacian part
  Real laplacian_part = _grad_u[_qp] * _grad_test[_i][_qp];

  // Divergence part
<<<<<<< HEAD
  Real div_part =
      _rho[_qp] * (_grad_a1[_qp](0) + _grad_a2[_qp](1) + _grad_a3[_qp](2)) * _test[_i][_qp];
=======
  Real div_part = _rho * (_grad_a1[_qp](0) + _grad_a2[_qp](1) + _grad_a3[_qp](2)) * _test[_i][_qp];
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  // Return the result
  return laplacian_part + div_part;
}

<<<<<<< HEAD
Real
INSPressurePoisson::computeQpJacobian()
=======



Real INSPressurePoisson::computeQpJacobian()
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
  return _grad_phi[_j][_qp] * _grad_test[_i][_qp];
}

<<<<<<< HEAD
Real
INSPressurePoisson::computeQpOffDiagJacobian(unsigned jvar)
{
  if (jvar == _a1_var_number)
    return _rho[_qp] * _grad_phi[_j][_qp](0) * _test[_i][_qp];

  else if (jvar == _a2_var_number)
    return _rho[_qp] * _grad_phi[_j][_qp](1) * _test[_i][_qp];

  else if (jvar == _a3_var_number)
    return _rho[_qp] * _grad_phi[_j][_qp](2) * _test[_i][_qp];
=======



Real INSPressurePoisson::computeQpOffDiagJacobian(unsigned jvar)
{
  if (jvar == _a1_var_number)
    return _rho * _grad_phi[_j][_qp](0) * _test[_i][_qp];

  else if (jvar == _a2_var_number)
    return _rho * _grad_phi[_j][_qp](1) * _test[_i][_qp];

  else if (jvar == _a3_var_number)
    return _rho * _grad_phi[_j][_qp](2) * _test[_i][_qp];
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  else
    return 0;
}
