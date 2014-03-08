<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "NSMassBC.h"

InputParameters
NSMassBC::validParams()
{
  InputParameters params = NSIntegratedBC::validParams();
  params.addClassDescription(
      "This class corresponds to the 'natural' boundary condition for the mass equation.");
  return params;
}

NSMassBC::NSMassBC(const InputParameters & parameters) : NSIntegratedBC(parameters) {}

Real
NSMassBC::qpResidualHelper(Real rhoun)
=======
#include "NSMassBC.h"

template<>
InputParameters validParams<NSMassBC>()
{
  InputParameters params = validParams<NSIntegratedBC>();


  return params;
}



NSMassBC::NSMassBC(const std::string & name, InputParameters parameters)
    : NSIntegratedBC(name, parameters)
{
}




Real NSMassBC::qp_residual(Real rhoun)
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
  return rhoun * _test[_i][_qp];
}

<<<<<<< HEAD
Real
NSMassBC::qpJacobianHelper(unsigned var_number)
{
  switch (var_number)
  {
    case 0: // density
    case 4: // energy
      return 0.0;

    case 1:
    case 2:
    case 3: // momentums
      // If one of the momentums, the derivative is a mass
      // matrix times that normal component...
      return _phi[_j][_qp] * _test[_i][_qp] * _normals[_qp](var_number - 1);

    default:
      mooseError("Should not get here!");
      break;
  }
}
=======



Real NSMassBC::qp_jacobian(unsigned var_number)
{
  switch ( var_number )
  {
  case 0: // density
  case 4: // energy
    return 0.;

  case 1:
  case 2:
  case 3: // momentums
  {
    // If one of the momentums, the derivative is a mass
    // matrix times that normal component...
    return _phi[_j][_qp] * _test[_i][_qp] * _normals[_qp](var_number-1);
  }

  default:
    mooseError("Should not get here!");
    break;
  }

  // won't get here...
  return 0;
}



// Real NSMassBC::computeQpResidual()
// {
//   // (rho*u.n) phi_i
//   RealVectorValue mom(_rho_u[_qp], _rho_v[_qp], _rho_w[_qp]);
//
//   return (mom*_normals[_qp]) * _test[_i][_qp];
// }
//
//
//
// Real NSMassBC::computeQpJacobian()
// {
//   // The derivative wrt rho is zero!
//   return 0.;
// }
//
//
//
// Real NSMassBC::computeQpOffDiagJacobian(unsigned jvar)
// {
//   // Map jvar into the variable m for our problem, regardless of
//   // how Moose has numbered things.
//   unsigned m = this->map_var_number(jvar);
//
//   switch ( m )
//   {
//     // Don't handle the on-diagonal case here
//     // case 0: // density
//
//   case 1:
//   case 2:
//   case 3:
//   {
//     // If jvar is one of the momentums, the derivative is a mass
//     // matrix times that normal component...
//     return _phi[_j][_qp] * _test[_i][_qp] * _normals[_qp](m-1);
//   }
//
//
//   case 4: // energy
//     return 0.;
//
//   default:
//     mooseError("Should not get here!");
//   }
//
//   // won't get here
//   return 0.;
// }
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
