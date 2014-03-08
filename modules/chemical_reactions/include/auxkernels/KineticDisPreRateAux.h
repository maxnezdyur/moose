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

/**
 * Calculate the kinetic mineral species kinetic rate according to transient
 * state theory rate law
=======
#ifndef KINETICDISPRERATEAUX_H
#define KINETICDISPRERATEAUX_H

#include "AuxKernel.h"

//Forward Declarations
class KineticDisPreRateAux;

template<>
InputParameters validParams<KineticDisPreRateAux>();

/**
 * Coupled auxiliary value
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
 */
class KineticDisPreRateAux : public AuxKernel
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  KineticDisPreRateAux(const InputParameters & parameters);
=======

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  KineticDisPreRateAux(const std::string & name, InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  virtual ~KineticDisPreRateAux() {}

protected:
<<<<<<< HEAD
  virtual Real computeValue() override;

  /// Equilibrium constant at reference temperature
  const VariableValue & _log_k;
  /// Specific reactive surface area, m^2/L solution
  const Real _r_area;
  /// Reference kinetic rate constant
  const Real _ref_kconst;
  /// Activation energy
  const Real _e_act;
  /// Gas constant, 8.314 J/mol/K
  const Real _gas_const;
  /// Reference temperature
  const Real _ref_temp;
  /// Actual system temperature
  const VariableValue & _sys_temp;
  /// Stoichiometric coefficients for involved primary species
  const std::vector<Real> _sto_v;
  /// Coupled primary species concentrations
  const std::vector<const VariableValue *> _vals;
};
=======
  virtual Real computeValue();

  Real _log_k,_r_area,_ref_kconst,_e_act,_gas_const,_ref_temp,_sys_temp;
  std::vector<Real> _sto_v;

  std::vector<VariableValue *> _vals;
};

#endif //KINETICDISPRERATEAUX_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
