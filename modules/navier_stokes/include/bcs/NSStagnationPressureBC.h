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
#ifndef NSSTAGNATIONPRESSUREBC_H
#define NSSTAGNATIONPRESSUREBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "NSStagnationBC.h"

// Forward Declarations
<<<<<<< HEAD

// Specialization required of all user-level Moose objects
=======
class NSStagnationPressureBC;


// Specialization required of all user-level Moose objects
template<>
InputParameters validParams<NSStagnationPressureBC>();

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * This Dirichlet condition imposes the condition p_0 = p_0_desired,
 * where p_0 is the stagnation pressure, defined as:
 * p_0 = p * (1 + (gam-1)/2 * M^2)^(gam/(gam-1))
 */
class NSStagnationPressureBC : public NSStagnationBC
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  NSStagnationPressureBC(const InputParameters & parameters);

protected:
=======
  // Constructor
  NSStagnationPressureBC(const std::string & name, InputParameters parameters);

  // Destructor, better be virtual
  virtual ~NSStagnationPressureBC(){}

protected:

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  // NodalBC's can (currently) only specialize the computeQpResidual function,
  // the computeQpJacobian() function automatically assembles a "1" onto the main
  // diagonal for this DoF.
  virtual Real computeQpResidual();

  // Coupled variables
<<<<<<< HEAD
  const VariableValue & _pressure;

  // Required paramters
  const Real _desired_stagnation_pressure;
};
=======
  VariableValue& _pressure;

  // Required paramters
  Real _desired_stagnation_pressure;
};


#endif // NSSTAGNATIONPRESSUREBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
