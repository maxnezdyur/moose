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
#include "NSEnergyInviscidUnspecifiedBC.h"

registerMooseObject("NavierStokesApp", NSEnergyInviscidUnspecifiedBC);

InputParameters
NSEnergyInviscidUnspecifiedBC::validParams()
{
  InputParameters params = NSEnergyInviscidBC::validParams();
  params.addRequiredCoupledVar(NS::pressure, "pressure");
  params.addDeprecatedCoupledVar("p", NS::pressure, "1/1/2022");
  return params;
}

NSEnergyInviscidUnspecifiedBC::NSEnergyInviscidUnspecifiedBC(const InputParameters & parameters)
  : NSEnergyInviscidBC(parameters), _pressure(coupledValue(NS::pressure))
{
}

Real
NSEnergyInviscidUnspecifiedBC::computeQpResidual()
=======
#include "NSEnergyInviscidUnspecifiedBC.h"

template<>
InputParameters validParams<NSEnergyInviscidUnspecifiedBC>()
{
  InputParameters params = validParams<NSEnergyInviscidBC>();

  // Coupled variables
  params.addRequiredCoupledVar("pressure", "");

  return params;
}



NSEnergyInviscidUnspecifiedBC::NSEnergyInviscidUnspecifiedBC(const std::string & name, InputParameters parameters)
    : NSEnergyInviscidBC(name, parameters),

      // Aux Variables
      _pressure(coupledValue("pressure"))
{
}




Real NSEnergyInviscidUnspecifiedBC::computeQpResidual()
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
  // Velocity vector object
  RealVectorValue vel(_u_vel[_qp], _v_vel[_qp], _w_vel[_qp]);

  // Normal component
  Real un = vel * _normals[_qp];

<<<<<<< HEAD
  return qpResidualHelper(_pressure[_qp], un);
}

Real
NSEnergyInviscidUnspecifiedBC::computeQpJacobian()
{
  return computeJacobianHelper(/*on-diagonal variable is energy=*/4);
}

Real
NSEnergyInviscidUnspecifiedBC::computeQpOffDiagJacobian(unsigned jvar)
{
  if (isNSVariable(jvar))
    return computeJacobianHelper(mapVarNumber(jvar));
  else
    return 0.0;
}

Real
NSEnergyInviscidUnspecifiedBC::computeJacobianHelper(unsigned var_number)
=======
  return this->qp_residual(_pressure[_qp], un);
}




Real NSEnergyInviscidUnspecifiedBC::computeQpJacobian()
{
  return this->compute_jacobian(/*on-diagonal variable is energy=*/4);
}




Real NSEnergyInviscidUnspecifiedBC::computeQpOffDiagJacobian(unsigned jvar)
{
  return this->compute_jacobian( this->map_var_number(jvar) );
}




Real NSEnergyInviscidUnspecifiedBC::compute_jacobian(unsigned var_number)
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
  // Velocity vector object
  RealVectorValue vel(_u_vel[_qp], _v_vel[_qp], _w_vel[_qp]);

  // Normal component of velocity
  Real un = vel * _normals[_qp];

  // When both u.n and pressure are unspecified, all 3 Jacobian terms apply.
  // See base class for details.
<<<<<<< HEAD
  return qpJacobianTermA(var_number, _pressure[_qp]) + qpJacobianTermB(var_number, un) +
         qpJacobianTermC(var_number, un);
=======
  return
    this->qp_jacobian_termA(var_number, _pressure[_qp]) +
    this->qp_jacobian_termB(var_number, un) +
    this->qp_jacobian_termC(var_number, un);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
