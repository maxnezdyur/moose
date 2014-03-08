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
#ifndef NSTEMPERATUREAUX_H
#define NSTEMPERATUREAUX_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "AuxKernel.h"

// Forward Declarations
<<<<<<< HEAD
class IdealGasFluidProperties;

/**
 * Temperature is an auxiliary value computed from the total energy
 * based on the FluidProperties.
=======
class NSTemperatureAux;

template<>
InputParameters validParams<NSTemperatureAux>();

/**
 * Temperature is an auxiliary value computed from the total energy
 * and the velocity magnitude (e_i = internal energy, e_t = total energy):
 * T = e_i / c_v
 *   = (e_t - |u|^2/2) / c_v
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
 */
class NSTemperatureAux : public AuxKernel
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  NSTemperatureAux(const InputParameters & parameters);
=======

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  NSTemperatureAux(const std::string & name, InputParameters parameters);

  virtual ~NSTemperatureAux() {}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeValue();

<<<<<<< HEAD
  const VariableValue & _specific_volume;
  const VariableValue & _specific_internal_energy;

  // Fluid properties
  const IdealGasFluidProperties & _fp;
};
=======
  // The temperature depends on velocities and total energy
  VariableValue & _rho;
  VariableValue & _u_vel;
  VariableValue & _v_vel;
  VariableValue & _w_vel;
  VariableValue & _rhoe;

  // Specific heat at constant volume, treated as a single
  // constant value.
  Real _R;
  Real _gamma;
};

#endif // NSTEMPERATUREAUX_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
