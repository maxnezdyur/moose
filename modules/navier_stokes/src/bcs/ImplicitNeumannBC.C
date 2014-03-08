<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ImplicitNeumannBC.h"

registerMooseObject("NavierStokesApp", ImplicitNeumannBC);

InputParameters
ImplicitNeumannBC::validParams()
{
  InputParameters params = IntegratedBC::validParams();
  params.addClassDescription("This class implements a form of the Neumann boundary condition in "
                             "which the boundary term is treated 'implicitly'.");
  return params;
}

ImplicitNeumannBC::ImplicitNeumannBC(const InputParameters & parameters) : IntegratedBC(parameters)
{
}

Real
ImplicitNeumannBC::computeQpResidual()
=======
#include "ImplicitNeumannBC.h"

template<>
InputParameters validParams<ImplicitNeumannBC>()
{
  InputParameters params = validParams<IntegratedBC>();
  return params;
}



ImplicitNeumannBC::ImplicitNeumannBC(const std::string & name, InputParameters parameters) :
  IntegratedBC(name, parameters)
{
}



Real ImplicitNeumannBC::computeQpResidual()
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
  return _grad_u[_qp] * _normals[_qp] * _test[_i][_qp];
}

<<<<<<< HEAD
Real
ImplicitNeumannBC::computeQpJacobian()
{
  return (_grad_phi[_j][_qp] * _normals[_qp]) * _test[_i][_qp];
}

Real
ImplicitNeumannBC::computeQpOffDiagJacobian(unsigned /*jvar*/)
=======



Real ImplicitNeumannBC::computeQpJacobian()
{
  return (_grad_phi[_j][_qp]*_normals[_qp]) * _test[_i][_qp];
}




Real ImplicitNeumannBC::computeQpOffDiagJacobian(unsigned /*jvar*/)
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
  // off-diagonal derivatives are all zero.
  return 0.;
}
