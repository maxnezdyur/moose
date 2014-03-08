<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "NSEnergyInviscidSpecifiedBC.h"

registerMooseObject("NavierStokesApp", NSEnergyInviscidSpecifiedBC);

InputParameters
NSEnergyInviscidSpecifiedBC::validParams()
{
  InputParameters params = NSEnergyInviscidBC::validParams();
  params.addRequiredParam<Real>("un", "The specified value of u.n for this boundary");
  return params;
}

NSEnergyInviscidSpecifiedBC::NSEnergyInviscidSpecifiedBC(const InputParameters & parameters)
  : NSEnergyInviscidBC(parameters),
    _specified_pressure(getParam<Real>("specified_pressure")),
    _un(getParam<Real>("un"))
{
}

Real
NSEnergyInviscidSpecifiedBC::computeQpResidual()
{
  return qpResidualHelper(_specified_pressure, _un);
}

Real
NSEnergyInviscidSpecifiedBC::computeQpJacobian()
{
  return this->computeJacobianHelper(/*on-diagonal variable is energy=*/4);
}

Real
NSEnergyInviscidSpecifiedBC::computeQpOffDiagJacobian(unsigned jvar)
{
  if (isNSVariable(jvar))
    return computeJacobianHelper(mapVarNumber(jvar));
  else
    return 0.0;
}

Real
NSEnergyInviscidSpecifiedBC::computeJacobianHelper(unsigned var_number)
{
  // When both pressure and u.n are specified, only term B of the Jacobian is non-zero.
  return qpJacobianTermB(var_number, _un);
=======
#include "NSEnergyInviscidSpecifiedBC.h"

template<>
InputParameters validParams<NSEnergyInviscidSpecifiedBC>()
{
  InputParameters params = validParams<NSEnergyInviscidBC>();

  // Required parameters
  params.addRequiredParam<Real>("specified_pressure", "The specified pressure for this boundary");
  params.addRequiredParam<Real>("un", "The specified value of u.n for this boundary");

  return params;
}



NSEnergyInviscidSpecifiedBC::NSEnergyInviscidSpecifiedBC(const std::string & name, InputParameters parameters)
    : NSEnergyInviscidBC(name, parameters),

      // Required parameters
      _specified_pressure(getParam<Real>("specified_pressure")),
      _un(getParam<Real>("un"))
{
}




Real NSEnergyInviscidSpecifiedBC::computeQpResidual()
{
  return this->qp_residual(_specified_pressure, _un);
}




Real NSEnergyInviscidSpecifiedBC::computeQpJacobian()
{
  return this->compute_jacobian(/*on-diagonal variable is energy=*/4);
}




Real NSEnergyInviscidSpecifiedBC::computeQpOffDiagJacobian(unsigned jvar)
{
  return this->compute_jacobian( this->map_var_number(jvar) );
}




Real NSEnergyInviscidSpecifiedBC::compute_jacobian(unsigned var_number)
{
  // When both pressure and u.n are specified, only term B of the Jacobian is non-zero.
  return this->qp_jacobian_termB(var_number, _un);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
