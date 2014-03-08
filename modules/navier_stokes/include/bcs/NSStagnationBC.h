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
#ifndef NSSTAGNATIONBC_H
#define NSSTAGNATIONBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "NodalBC.h"

// Forward Declarations
<<<<<<< HEAD
class IdealGasFluidProperties;

// Specialization required of all user-level Moose objects
=======
class NSStagnationBC;


// Specialization required of all user-level Moose objects
template<>
InputParameters validParams<NSStagnationBC>();

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * This is the base class for the "imposed stagnation" value boundary
 * conditions.  Derived classes impose specified stagnation pressure
 * and temperature BCs as Dirichlet terms in the governing equations.
 */
class NSStagnationBC : public NodalBC
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  NSStagnationBC(const InputParameters & parameters);

protected:
  const VariableValue & _mach;

  // Fluid properties
  const IdealGasFluidProperties & _fp;
};
=======
  // Constructor
  NSStagnationBC(const std::string & name, InputParameters parameters);

  // Destructor, better be virtual
  virtual ~NSStagnationBC(){}

protected:

  /**
   * Must be implemented in derived class.
   */
  // virtual Real computeQpResidual();

  // Coupled variables
  VariableValue& _u_vel;
  VariableValue& _v_vel;
  VariableValue& _w_vel;

  VariableValue& _temperature;

  // Required paramters
  Real _gamma;
  Real _R;
};


#endif // NSSTAGNATIONBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
