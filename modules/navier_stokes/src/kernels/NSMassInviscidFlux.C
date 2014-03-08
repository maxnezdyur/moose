<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "NSMassInviscidFlux.h"

registerMooseObject("NavierStokesApp", NSMassInviscidFlux);

InputParameters
NSMassInviscidFlux::validParams()
{
  InputParameters params = NSKernel::validParams();
  params.addClassDescription("This class computes the inviscid flux in the mass equation.");
  return params;
}

NSMassInviscidFlux::NSMassInviscidFlux(const InputParameters & parameters) : NSKernel(parameters) {}
=======
#include "NSMassInviscidFlux.h"

template<>
InputParameters validParams<NSMassInviscidFlux>()
{
  InputParameters params = validParams<NSKernel>();

  return params;
}

NSMassInviscidFlux::NSMassInviscidFlux(const std::string & name, InputParameters parameters)
    : NSKernel(name, parameters)
{}


>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

Real
NSMassInviscidFlux::computeQpResidual()
{
<<<<<<< HEAD
  const RealVectorValue mom(_rho_u[_qp], _rho_v[_qp], _rho_w[_qp]);
=======
  RealVectorValue mom(_rho_u[_qp], _rho_v[_qp], _rho_w[_qp]);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  // -(rho*U) * grad(phi), negative sign comes from integration-by-parts
  return -(mom * _grad_test[_i][_qp]);
}

<<<<<<< HEAD
=======


>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
Real
NSMassInviscidFlux::computeQpJacobian()
{
  // This seems weird at first glance, but remember we have to differentiate
  // wrt the *conserved* variables
  //
  // [ U_0 ] = [ rho       ]
  // [ U_1 ] = [ rho * u_1 ]
  // [ U_2 ] = [ rho * u_2 ]
  // [ U_3 ] = [ rho * u_3 ]
  // [ U_4 ] = [ rho * E   ]
  //
  // and the inviscid mass flux residual, in terms of these variables, is:
  //
  // f(U) = ( U_k * dphi_i/dx_k ), summation over k=1,2,3
  //
  // ie. does not depend on U_0, the on-diagonal Jacobian component.
<<<<<<< HEAD
  return 0.0;
}

Real
NSMassInviscidFlux::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (isNSVariable(jvar))
  {
    // Map jvar into the variable m for our problem, regardless of
    // how Moose has numbered things.
    unsigned int m = mapVarNumber(jvar);

    switch (m)
    {
      // Don't handle the on-diagonal case here
      // case 0: // density
      case 1:
      case 2:
      case 3: // momentums
        return -_phi[_j][_qp] * _grad_test[_i][_qp](m - 1);

      case 4: // energy
        return 0.0;

      default:
        mooseError("Should not get here!");
        break;
    }
  }
  else
    return 0.0;
=======
  return 0.;
}



Real
NSMassInviscidFlux::computeQpOffDiagJacobian(unsigned int jvar)
{
  // Map jvar into the variable m for our problem, regardless of
  // how Moose has numbered things.
  unsigned m = this->map_var_number(jvar);

  switch ( m )
  {
    // Don't handle the on-diagonal case here
    // case 0: // density

  case 1:
  case 2:
  case 3: // momentums
  {
    return -_phi[_j][_qp] * _grad_test[_i][_qp](m-1);
  }

  case 4: // energy
    return 0.;

  default:
    mooseError("Should not get here!");
    break;
  }

  // won't get here
  return 0.;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
