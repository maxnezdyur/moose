<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "NSIntegratedBC.h"

// Forward Declarations
=======
#ifndef NSMASSBC_H
#define NSMASSBC_H

#include "NSIntegratedBC.h"


// Forward Declarations
class NSMassBC;

template<>
InputParameters validParams<NSMassBC>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * This class corresponds to the "natural" boundary condition
 * for the mass equation, i.e. what you get if you integrate
 * the invsicid flux term by parts:
 *
 * int_{Gamma} (rho*u.n) v
 *
 * While this class implements the residual and jacobian values for
 * this term, it does not itself implement any of the computeQp*
 * functions.  For that, use one of the derived classes:
 * 1.) NSMassSpecifiedNormalFlowBC
 * 2.) NSMassUnspecifiedNormalFlowBC
 */
class NSMassBC : public NSIntegratedBC
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  NSMassBC(const InputParameters & parameters);

protected:
=======

  NSMassBC(const std::string & name, InputParameters parameters);

  virtual ~NSMassBC(){}

protected:

  /**
   * Must be implemented in derived classes.
   */
//  virtual Real computeQpResidual();
//  virtual Real computeQpJacobian();
//  virtual Real computeQpOffDiagJacobian(unsigned jvar);

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  /**
   * Compute the residual contribution for a given value of
   * rho*(u.n).  This value may come from the current nonlinear
   * solution or be specified, depending on the derived class.
   */
<<<<<<< HEAD
  Real qpResidualHelper(Real rhoun);
=======
  Real qp_residual(Real rhoun);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  /**
   * Compute the Jacobian contribution due to variable
   * number 'var_number'.  Note: if this is a specified
   * normal flow boundary, the Jacobian will be zero.
   */
<<<<<<< HEAD
  Real qpJacobianHelper(unsigned var_number);
};
=======
  Real qp_jacobian(unsigned var_number);
};

#endif // MASSBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
