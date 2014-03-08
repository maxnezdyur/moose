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
#include "NSEnergyInviscidBC.h"
#include "NS.h"

// FluidProperties includes
#include "IdealGasFluidProperties.h"

InputParameters
NSEnergyInviscidBC::validParams()
{
  InputParameters params = NSIntegratedBC::validParams();
  params.addClassDescription("This class corresponds to the inviscid part of the 'natural' "
                             "boundary condition for the energy equation.");
  params.addRequiredCoupledVar(NS::temperature, "temperature");
  return params;
}

NSEnergyInviscidBC::NSEnergyInviscidBC(const InputParameters & parameters)
  : NSIntegratedBC(parameters),
    _temperature(coupledValue(NS::temperature)),
    // Object for computing deriviatives of pressure
    _pressure_derivs(*this)
{
}

Real
NSEnergyInviscidBC::qpResidualHelper(Real pressure, Real un)
{
  return (_rho_et[_qp] + pressure) * un * _test[_i][_qp];
}

Real
NSEnergyInviscidBC::qpResidualHelper(Real rho, RealVectorValue u, Real /*pressure*/)
{
  // return (rho*(cv*_temperature[_qp] + 0.5*u.norm_sq()) + pressure) * (u*_normals[_qp]) *
  // _test[_i][_qp];
  // We can also expand pressure in terms of rho... does this make a difference?
  // Then we don't use the input pressure value.
  return rho * (_fp.gamma() * _fp.cv() * _temperature[_qp] + 0.5 * u.norm_sq()) *
         (u * _normals[_qp]) * _test[_i][_qp];
}

// (U4+p) * d(u.n)/dX
Real
NSEnergyInviscidBC::qpJacobianTermA(unsigned var_number, Real pressure)
{
  Real result = 0.0;

  switch (var_number)
  {
    case 0: // density
    {
      // Velocity vector object
      RealVectorValue vel(_u_vel[_qp], _v_vel[_qp], _w_vel[_qp]);

      result = -(vel * _normals[_qp]);
      break;
    }

    case 1:
    case 2:
    case 3: // momentums
      result = _normals[_qp](var_number - 1);
      break;

    case 4: // energy
      result = 0.;
      break;

    default:
      mooseError("Shouldn't get here!");
      break;
=======
#include "NSEnergyInviscidBC.h"

template<>
InputParameters validParams<NSEnergyInviscidBC>()
{
  InputParameters params = validParams<NSIntegratedBC>();

  // Coupled variables
  params.addRequiredCoupledVar("temperature", "");

  return params;
}



NSEnergyInviscidBC::NSEnergyInviscidBC(const std::string & name, InputParameters parameters)
    : NSIntegratedBC(name, parameters),

      // Aux Variables
      _temperature(coupledValue("temperature")),

      // Object for computing deriviatives of pressure
      _pressure_derivs(*this)
{
}




Real NSEnergyInviscidBC::qp_residual(Real pressure, Real un)
{
  return (_rho_e[_qp] + pressure) * un * _test[_i][_qp];
}




Real NSEnergyInviscidBC::qp_residual(Real rho, RealVectorValue u, Real /*pressure*/)
{
  // return (rho*(cv*_temperature[_qp] + 0.5*u.size_sq()) + pressure) * (u*_normals[_qp]) * _test[_i][_qp];

  // We can also expand pressure in terms of rho... does this make a difference?
  // Then we don't use the input pressure value.
  Real cv = _R / (_gamma-1.);
  return rho * (_gamma * cv * _temperature[_qp] + 0.5*u.size_sq()) * (u*_normals[_qp]) * _test[_i][_qp];
}




// (U4+p) * d(u.n)/dX
Real NSEnergyInviscidBC::qp_jacobian_termA(unsigned var_number, Real pressure)
{
  Real result = 0.;

  switch ( var_number )
  {
  case 0: // density
  {
    // Velocity vector object
    RealVectorValue vel(_u_vel[_qp], _v_vel[_qp], _w_vel[_qp]);

    result = -(vel * _normals[_qp]);
    break;
  }

  case 1:
  case 2:
  case 3: // momentums
  {
    result = _normals[_qp](var_number-1);
    break;
  }

  case 4: // energy
  {
    result = 0.;
    break;
  }

  default:
    mooseError("Shouldn't get here!");
    break;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  }

  // Notice the division by _rho[_qp] here.  This comes from taking the
  // derivative wrt to either density or momentum.
<<<<<<< HEAD
  return (_rho_et[_qp] + pressure) / _rho[_qp] * result * _phi[_j][_qp] * _test[_i][_qp];
}

// d(U4)/dX * (u.n)
Real
NSEnergyInviscidBC::qpJacobianTermB(unsigned var_number, Real un)
{
  Real result = 0.0;
  switch (var_number)
  {
    case 0: // density
    case 1:
    case 2:
    case 3: // momentums
    {
      result = 0.;
      break;
    }

    case 4: // energy
    {
      result = _phi[_j][_qp] * un * _test[_i][_qp];
      break;
    }

    default:
      mooseError("Shouldn't get here!");
      break;
=======
  return (_rho_e[_qp] + pressure)/_rho[_qp] * result * _phi[_j][_qp] * _test[_i][_qp];
}




// d(U4)/dX * (u.n)
Real NSEnergyInviscidBC::qp_jacobian_termB(unsigned var_number, Real un)
{
  Real result = 0.;

  switch ( var_number )
  {
  case 0: // density
  case 1:
  case 2:
  case 3: // momentums
  {
    result = 0.;
    break;
  }

  case 4: // energy
  {
    result = _phi[_j][_qp] * un * _test[_i][_qp];
    break;
  }

  default:
    mooseError("Shouldn't get here!");
    break;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  }

  return result;
}

<<<<<<< HEAD
// d(p)/dX * (u.n)
Real
NSEnergyInviscidBC::qpJacobianTermC(unsigned var_number, Real un)
{
  return _pressure_derivs.get_grad(var_number) * _phi[_j][_qp] * un * _test[_i][_qp];
}
=======



// d(p)/dX * (u.n)
Real
NSEnergyInviscidBC::qp_jacobian_termC(unsigned var_number, Real un)
{
  return _pressure_derivs.get_grad(var_number) * _phi[_j][_qp] * un * _test[_i][_qp];
}




// Real NSEnergyInviscidBC::computeQpResidual()
// {
//   // n . (rho*H*u) v
//
//   // Velocity vector object
//   RealVectorValue vel(_u_vel[_qp], _v_vel[_qp], _w_vel[_qp]);
//
//   // rho*H*u = (rho*E + p)*u
//   RealVectorValue conv_vec = (_rho_e[_qp] + _specified_pressure) * vel;
//
//   // Dot with normal, hit with test function.
//   return (conv_vec * _normals[_qp]) * _test[_i][_qp];
// }
//
//
//
//
// Real NSEnergyInviscidBC::computeQpJacobian()
// {
//   // Velocity vector object
//   RealVectorValue vel(_u_vel[_qp], _v_vel[_qp], _w_vel[_qp]);
//
//   // Derivative of convective term (above) wrt U_4
//   Real conv_term = _phi[_j][_qp] * (vel * _normals[_qp]);
//
//   return conv_term * _test[_i][_qp];
// }
//
//
//
//
// Real NSEnergyInviscidBC::computeQpOffDiagJacobian(unsigned jvar)
// {
//   // Convenience variables
//   RealVectorValue U(_rho_u[_qp], _rho_v[_qp], _rho_w[_qp]);
//
//   // H_bar = E + p_bar/rho, the enthalpy evaluated at the specified pressure
//   Real H_bar = (_rho_e[_qp] + _specified_pressure) / _rho[_qp];
//
//   // Map jvar into the variable m for our problem, regardless of
//   // how Moose has numbered things.
//   unsigned m = this->map_var_number(jvar);
//
//   // Velocity vector object
//   RealVectorValue vel(_u_vel[_qp], _v_vel[_qp], _w_vel[_qp]);
//
//   //
//   // Convective term derivatives
//   //
//
//   // See notes for this term, specifically the specified-pressure subsonic outflow section
//   Real conv_term = 0.;
//
//   switch ( m )
//   {
//   case 0: // density
//   {
//     conv_term = -H_bar * (vel * _normals[_qp]) * _phi[_j][_qp];
//     break;
//   }
//
//   case 1:
//   case 2:
//   case 3: // momentums
//   {
//     conv_term = H_bar * _normals[_qp](m-1) * _phi[_j][_qp];
//     break;
//   }
//
//   case 4: // energy
//     mooseError("Shouldn't be computing on-diagonal component!");
//
//   default:
//     mooseError("Shouldn't get here!");
//   }
//
//   // Finally, sum up the different contributions (with appropriate
//   // sign) multiply by the test function, and return.
//   return conv_term * _test[_i][_qp];
// }



>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
