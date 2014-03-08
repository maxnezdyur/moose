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

#include "AuxKernel.h"

// Forward Declarations
class IdealGasFluidProperties;
=======
#ifndef NSPRESSUREAUX_H
#define NSPRESSUREAUX_H

#include "AuxKernel.h"

//Forward Declarations
class NSPressureAux;

template<>
InputParameters validParams<NSPressureAux>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * Nodal auxiliary variable, for computing pressure at the nodes
 */
class NSPressureAux : public AuxKernel
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  NSPressureAux(const InputParameters & parameters);
=======

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  NSPressureAux(const std::string & name, InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  virtual ~NSPressureAux() {}

protected:
  virtual Real computeValue();

<<<<<<< HEAD
  const VariableValue & _specific_volume;
  const VariableValue & _specific_internal_energy;

  // Fluid properties
  const IdealGasFluidProperties & _fp;
};
=======
  VariableValue & _rho;
  VariableValue & _u_vel;
  VariableValue & _v_vel;
  VariableValue & _w_vel;
  VariableValue & _rhoe;

  Real _gamma;
};

#endif //VELOCITYAUX_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
