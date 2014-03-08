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
#ifndef NSSTAGNATIONTEMPERATUREBC_H
#define NSSTAGNATIONTEMPERATUREBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "NSStagnationBC.h"

// Forward Declarations
<<<<<<< HEAD
=======
class NSStagnationTemperatureBC;


// Specialization required of all user-level Moose objects
template<>
InputParameters validParams<NSStagnationTemperatureBC>();

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * This Dirichlet condition imposes the condition T_0 = T_0_desired,
 * where T_0 is the stagnation temperature, defined as:
 * T_0 = T * (1 + (gam-1)/2 * M^2)
 */
class NSStagnationTemperatureBC : public NSStagnationBC
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  NSStagnationTemperatureBC(const InputParameters & parameters);

protected:
=======
  // Constructor
  NSStagnationTemperatureBC(const std::string & name, InputParameters parameters);

  // Destructor, better be virtual
  virtual ~NSStagnationTemperatureBC(){}

protected:

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  // NodalBC's can (currently) only specialize the computeQpResidual function,
  // the computeQpJacobian() function automatically assembles a "1" onto the main
  // diagonal for this DoF.
  virtual Real computeQpResidual();

<<<<<<< HEAD
  const VariableValue & _temperature;

  // Required paramters
  const Real _desired_stagnation_temperature;
};
=======
  // Required paramters
  Real _desired_stagnation_temperature;
};


#endif // NSSTAGNATIONTEMPERATUREBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
