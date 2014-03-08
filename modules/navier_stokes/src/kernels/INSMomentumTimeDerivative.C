<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "INSMomentumTimeDerivative.h"

registerMooseObject("NavierStokesApp", INSMomentumTimeDerivative);

InputParameters
INSMomentumTimeDerivative::validParams()
{
  InputParameters params = TimeDerivative::validParams();
  params.addClassDescription("This class computes the time derivative for the incompressible "
                             "Navier-Stokes momentum equation.");
  params.addParam<MaterialPropertyName>("rho_name", "rho", "density name");
  return params;
}

INSMomentumTimeDerivative::INSMomentumTimeDerivative(const InputParameters & parameters)
  : TimeDerivative(parameters), _rho(getMaterialProperty<Real>("rho_name"))
{
}
=======
#include "INSMomentumTimeDerivative.h"

template<>
InputParameters validParams<INSMomentumTimeDerivative>()
{
  InputParameters params = validParams<TimeDerivative>();
  params.addRequiredParam<Real>("rho", "density");
  return params;
}


INSMomentumTimeDerivative::INSMomentumTimeDerivative(const std::string & name,
                                                     InputParameters parameters) :
  TimeDerivative(name,parameters),
  _rho(getParam<Real>("rho"))
{}


>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

Real
INSMomentumTimeDerivative::computeQpResidual()
{
<<<<<<< HEAD
  return _rho[_qp] * TimeDerivative::computeQpResidual();
}

Real
INSMomentumTimeDerivative::computeQpJacobian()
{
  return _rho[_qp] * TimeDerivative::computeQpJacobian();
}

Real
INSMomentumTimeDerivative::computeQpOffDiagJacobian(unsigned)
=======
  return _rho * TimeDerivative::computeQpResidual();
}



Real
INSMomentumTimeDerivative::computeQpJacobian()
{
  return _rho * TimeDerivative::computeQpJacobian();
}



Real INSMomentumTimeDerivative::computeQpOffDiagJacobian(unsigned)
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
  return 0.;
}
