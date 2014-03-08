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

#include "NodalBC.h"

// Forward Declarations
class IdealGasFluidProperties;
=======
#ifndef NSTHERMALBC_H
#define NSTHERMALBC_H

#include "NodalBC.h"


// Forward Declarations
class NSThermalBC;

template<>
InputParameters validParams<NSThermalBC>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

class NSThermalBC : public NodalBC
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  NSThermalBC(const InputParameters & parameters);
=======

  NSThermalBC(const std::string & name, InputParameters parameters);

  virtual ~NSThermalBC(){}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  // Computes the temperature based on ideal gas equation of state,
  // the total energy, and the velocity: T = e_i/c_v
  virtual Real computeQpResidual();

  unsigned int _rho_var;
<<<<<<< HEAD
  const VariableValue & _rho;
=======
  VariableValue & _rho;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  Real _initial;
  Real _final;
  Real _duration;

<<<<<<< HEAD
  // Fluid properties
  const IdealGasFluidProperties & _fp;
};
=======
  // Specific heat at constant volume, treated as a single
  // constant value.
  Real _R;
  Real _gamma;
};

#endif //THERMALBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
