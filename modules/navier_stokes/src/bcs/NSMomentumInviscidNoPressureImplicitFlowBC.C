<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "NSMomentumInviscidNoPressureImplicitFlowBC.h"

registerMooseObject("NavierStokesApp", NSMomentumInviscidNoPressureImplicitFlowBC);

InputParameters
NSMomentumInviscidNoPressureImplicitFlowBC::validParams()
{
  InputParameters params = NSMomentumInviscidBC::validParams();
  params.addClassDescription(
      "Momentum equation boundary condition used when pressure *is not* integrated by parts.");
  return params;
}

NSMomentumInviscidNoPressureImplicitFlowBC::NSMomentumInviscidNoPressureImplicitFlowBC(
    const InputParameters & parameters)
  : NSMomentumInviscidBC(parameters)
{
}

Real
NSMomentumInviscidNoPressureImplicitFlowBC::computeQpResidual()
=======
#include "NSMomentumInviscidNoPressureImplicitFlowBC.h"

template<>
InputParameters validParams<NSMomentumInviscidNoPressureImplicitFlowBC>()
{
  InputParameters params = validParams<NSMomentumInviscidBC>();

  return params;
}




NSMomentumInviscidNoPressureImplicitFlowBC::NSMomentumInviscidNoPressureImplicitFlowBC(const std::string & name, InputParameters parameters)
    : NSMomentumInviscidBC(name, parameters)
{
}




Real NSMomentumInviscidNoPressureImplicitFlowBC::computeQpResidual()
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
  // Velocity vector object
  RealVectorValue vel(_u_vel[_qp], _v_vel[_qp], _w_vel[_qp]);

  // Velocity vector dotted with normal
  Real u_dot_n = vel * _normals[_qp];

  // The current value of the vector (rho*u)(u.n)
  RealVectorValue rhou_udotn = u_dot_n * _rho[_qp] * vel;

<<<<<<< HEAD
  return convectiveQpResidualHelper(rhou_udotn(_component));
}

Real
NSMomentumInviscidNoPressureImplicitFlowBC::computeQpJacobian()
=======
  return
    this->convective_qp_residual( rhou_udotn(_component) );
}



Real NSMomentumInviscidNoPressureImplicitFlowBC::computeQpJacobian()
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
  // There is no Jacobian for the pressure term when the pressure is specified,
  // so all we have left is the convective part.  The on-diagonal variable number
  // is _component+1
<<<<<<< HEAD
  return convectiveQpJacobianHelper(_component + 1);
}

Real
NSMomentumInviscidNoPressureImplicitFlowBC::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (isNSVariable(jvar))
    return convectiveQpJacobianHelper(mapVarNumber(jvar));
  else
    return 0.0;
=======
  return this->convective_qp_jacobian(_component+1);
}



Real NSMomentumInviscidNoPressureImplicitFlowBC::computeQpOffDiagJacobian(unsigned jvar)
{
  return this->convective_qp_jacobian( this->map_var_number(jvar) );
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
