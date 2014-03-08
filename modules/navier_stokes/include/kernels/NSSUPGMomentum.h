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
=======
#ifndef NSSUPGMOMENTUM_H
#define NSSUPGMOMENTUM_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "NSSUPGBase.h"

// Forward Declarations
<<<<<<< HEAD
=======
class NSSUPGMomentum;

template<>
InputParameters validParams<NSSUPGMomentum>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * Compute residual and Jacobian terms form the SUPG
 * terms in the momentum equation.
 */
class NSSUPGMomentum : public NSSUPGBase
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  NSSUPGMomentum(const InputParameters & parameters);
=======
  NSSUPGMomentum(const std::string & name, InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  // This kernel is to be used for the x, y, and z momentum equations.
  // The _component parameter tells you which equation you are currently
  // solving.
<<<<<<< HEAD
  unsigned int _component;
=======
  unsigned _component;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

private:
  // Single function for computing on and off-diagonal Jacobian
  // entries in a single function.  The input index is in Moose
  // variable numbering.
<<<<<<< HEAD
  Real computeJacobianHelper(unsigned int var);
};
=======
  Real compute_jacobian(unsigned var);
};

#endif // NSSUPGMOMENTUM_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
