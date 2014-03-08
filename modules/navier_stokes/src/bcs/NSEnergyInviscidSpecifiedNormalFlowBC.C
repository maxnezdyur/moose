<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

// Navier-Stokes includes
#include "NS.h"
#include "NSEnergyInviscidSpecifiedNormalFlowBC.h"

registerMooseObject("NavierStokesApp", NSEnergyInviscidSpecifiedNormalFlowBC);

InputParameters
NSEnergyInviscidSpecifiedNormalFlowBC::validParams()
{
  InputParameters params = NSEnergyInviscidBC::validParams();
  params.addRequiredCoupledVar(NS::pressure, "pressure");
  params.addDeprecatedCoupledVar("p", NS::pressure, "1/1/2022");
  params.addRequiredParam<Real>("un", "The specified value of u.n for this boundary");
  return params;
}

NSEnergyInviscidSpecifiedNormalFlowBC::NSEnergyInviscidSpecifiedNormalFlowBC(
    const InputParameters & parameters)
  : NSEnergyInviscidBC(parameters), _pressure(coupledValue(NS::pressure)), _un(getParam<Real>("un"))
{
}

Real
NSEnergyInviscidSpecifiedNormalFlowBC::computeQpResidual()
{
  return qpResidualHelper(_pressure[_qp], _un);
}

Real
NSEnergyInviscidSpecifiedNormalFlowBC::computeQpJacobian()
{
  return computeJacobianHelper(/*on-diagonal variable is energy=*/4);
}

Real
NSEnergyInviscidSpecifiedNormalFlowBC::computeQpOffDiagJacobian(unsigned jvar)
{
  if (isNSVariable(jvar))
    return computeJacobianHelper(mapVarNumber(jvar));
  else
    return 0.0;
}

Real
NSEnergyInviscidSpecifiedNormalFlowBC::computeJacobianHelper(unsigned var_number)
{
  // For specified u.n, term "A" is zero, see base class for details.
  return qpJacobianTermB(var_number, _un) + qpJacobianTermC(var_number, _un);
=======
#include "NSEnergyInviscidSpecifiedNormalFlowBC.h"

template<>
InputParameters validParams<NSEnergyInviscidSpecifiedNormalFlowBC>()
{
  InputParameters params = validParams<NSEnergyInviscidBC>();

  // Coupled variables
  params.addRequiredCoupledVar("pressure", "");

  // Required parameters
  params.addRequiredParam<Real>("un", "The specified value of u.n for this boundary");

  return params;
}



NSEnergyInviscidSpecifiedNormalFlowBC::NSEnergyInviscidSpecifiedNormalFlowBC(const std::string & name, InputParameters parameters)
    : NSEnergyInviscidBC(name, parameters),

      // Aux Variables
      _pressure(coupledValue("pressure")),

      // Required parameters
      _un(getParam<Real>("un"))
{
}




Real NSEnergyInviscidSpecifiedNormalFlowBC::computeQpResidual()
{
  return this->qp_residual(_pressure[_qp], _un);
}




Real NSEnergyInviscidSpecifiedNormalFlowBC::computeQpJacobian()
{
  return this->compute_jacobian(/*on-diagonal variable is energy=*/4);
}




Real NSEnergyInviscidSpecifiedNormalFlowBC::computeQpOffDiagJacobian(unsigned jvar)
{
  return this->compute_jacobian( this->map_var_number(jvar) );
}




Real NSEnergyInviscidSpecifiedNormalFlowBC::compute_jacobian(unsigned var_number)
{
  // For specified u.n, term "A" is zero, see base class for details.
  return
    this->qp_jacobian_termB(var_number, _un) +
    this->qp_jacobian_termC(var_number, _un);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
