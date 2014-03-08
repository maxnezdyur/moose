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
#include "NSMomentumInviscidFlux.h"

// FluidProperties includes
#include "IdealGasFluidProperties.h"

registerMooseObject("NavierStokesApp", NSMomentumInviscidFlux);

InputParameters
NSMomentumInviscidFlux::validParams()
{
  InputParameters params = NSKernel::validParams();
  params.addClassDescription(
      "The inviscid flux (convective + pressure terms) for the momentum conservation equations.");
  params.addRequiredCoupledVar(NS::pressure, "pressure");
  params.addRequiredParam<unsigned int>(
      "component",
      "0,1,2 depending on if we are solving the x,y,z component of the momentum equation");
  return params;
}

NSMomentumInviscidFlux::NSMomentumInviscidFlux(const InputParameters & parameters)
  : NSKernel(parameters),
    _pressure(coupledValue(NS::pressure)),
    _component(getParam<unsigned int>("component"))
{
}
=======
#include "NSMomentumInviscidFlux.h"


template<>
InputParameters validParams<NSMomentumInviscidFlux>()
{
  InputParameters params = validParams<NSKernel>();

  // Coupled variables
  params.addRequiredCoupledVar("pressure", "");

  // Required parameters
  params.addRequiredParam<unsigned>("component", "0,1,2 depending on if we are solving the x,y,z component of the momentum equation");

  return params;
}





NSMomentumInviscidFlux::NSMomentumInviscidFlux(const std::string & name, InputParameters parameters)
    : NSKernel(name, parameters),

      // Coupled variables
      _pressure(coupledValue("pressure")),

      // Required parameters
      _component(getParam<unsigned>("component"))
{}



>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

Real
NSMomentumInviscidFlux::computeQpResidual()
{
  // For _component = k,

  // (rho*u) * u_k = (rho*u_k) * u <- we write it this way
<<<<<<< HEAD
  RealVectorValue vec(_u[_qp] * _u_vel[_qp],  // (U_k) * u_1
                      _u[_qp] * _v_vel[_qp],  // (U_k) * u_2
                      _u[_qp] * _w_vel[_qp]); // (U_k) * u_3
=======
  RealVectorValue vec(_u[_qp]*_u_vel[_qp],   // (U_k) * u_1
                      _u[_qp]*_v_vel[_qp],   // (U_k) * u_2
                      _u[_qp]*_w_vel[_qp]);  // (U_k) * u_3
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  // (rho*u_k) * u + e_k * P [ e_k = unit vector in k-direction ]
  vec(_component) += _pressure[_qp];

  // -((rho*u_k) * u + e_k * P) * grad(phi)
<<<<<<< HEAD
  return -(vec * _grad_test[_i][_qp]);
}

=======
  return -(vec*_grad_test[_i][_qp]);
}




>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
Real
NSMomentumInviscidFlux::computeQpJacobian()
{
  // The on-diagonal entry corresponds to variable number _component+1.
<<<<<<< HEAD
  return computeJacobianHelper(_component + 1);
}

Real
NSMomentumInviscidFlux::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (isNSVariable(jvar))
    return computeJacobianHelper(mapVarNumber(jvar));
  else
    return 0.0;
}

Real
NSMomentumInviscidFlux::computeJacobianHelper(unsigned int m)
{
  const RealVectorValue vel(_u_vel[_qp], _v_vel[_qp], _w_vel[_qp]);

  // Ratio of specific heats
  const Real gam = _fp.gamma();

  switch (m)
  {
    case 0: // density
    {
      const Real V2 = vel.norm_sq();
      return vel(_component) * (vel * _grad_test[_i][_qp]) -
             0.5 * (gam - 1.0) * V2 * _grad_test[_i][_qp](_component);
    }

    case 1:
    case 2:
    case 3: // momentums
    {
      // Map m into m_local = {0,1,2}
      unsigned int m_local = m - 1;

      // Kronecker delta
      const Real delta_kl = (_component == m_local ? 1. : 0.);

      return -1.0 *
             (vel(_component) * _grad_test[_i][_qp](m_local) +
              delta_kl * (vel * _grad_test[_i][_qp]) +
              (1. - gam) * vel(m_local) * _grad_test[_i][_qp](_component)) *
             _phi[_j][_qp];
    }

    case 4: // energy
      return -1.0 * (gam - 1.0) * _phi[_j][_qp] * _grad_test[_i][_qp](_component);
  }

  mooseError("Shouldn't get here!");
=======
  return this->compute_jacobian(_component+1);
}





Real
NSMomentumInviscidFlux::computeQpOffDiagJacobian(unsigned int jvar)
{
  // Map jvar into the variable m for our problem, regardless of
  // how Moose has numbered things.
  unsigned m = this->map_var_number(jvar);

  return this->compute_jacobian(m);
}



Real NSMomentumInviscidFlux::compute_jacobian(unsigned m)
{
  RealVectorValue vel(_u_vel[_qp], _v_vel[_qp], _w_vel[_qp]);

  switch ( m )
  {
  case 0: // density
  {
    Real V2 = vel.size_sq();

    return vel(_component) * (vel*_grad_test[_i][_qp]) - 0.5 * (_gamma-1.) * V2 * _grad_test[_i][_qp](_component);
  }

  case 1:
  case 2:
  case 3: // momentums
  {
    // Map m into m_local = {0,1,2}
    unsigned m_local = m - 1;

    // Kronecker delta
    Real delta_kl = (_component == m_local ? 1. : 0.);

    return -1. * (vel(_component) * _grad_test[_i][_qp](m_local)
                  + delta_kl * (vel * _grad_test[_i][_qp])
                  + (1.-_gamma) * vel(m_local) *_grad_test[_i][_qp](_component)) * _phi[_j][_qp];
  }

  case 4: // energy
    return -1. * (_gamma - 1.) * _phi[_j][_qp] * _grad_test[_i][_qp](_component);

  default:
    mooseError("Shouldn't get here!");
    break;
  }

  // Won't get here!
  return 0;

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
