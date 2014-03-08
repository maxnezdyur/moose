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

#include "KineticDisPreRateAux.h"

/**
 * Calculate the kinetic mineral species concentrations according to
 * transient state theory rate law.
 */
class KineticDisPreConcAux : public KineticDisPreRateAux
{
public:
  static InputParameters validParams();

  KineticDisPreConcAux(const InputParameters & parameters);
=======
#ifndef KINETICDISPRECONCAUX_H
#define KINETICDISPRECONCAUX_H

#include "AuxKernel.h"


//Forward Declarations
class KineticDisPreConcAux;

/**
 * validParams returns the parameters that this Kernel accepts / needs
 * The actual body of the function MUST be in the .C file.
 */
template<>
InputParameters validParams<KineticDisPreConcAux>();

/**
 * Define the AuxKernel for the kinetic mineral species concentrations
 * according to transient state theory rate law.
 */
class KineticDisPreConcAux : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  KineticDisPreConcAux(const std::string & name, InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  virtual ~KineticDisPreConcAux() {}

protected:
<<<<<<< HEAD
  virtual Real computeValue() override;

  const VariableValue & _u_old;
};
=======
  /**
   * Conputes the kinetic mineral sepecies concentration.
   * @return The concentration of a kinetic mineral species.
   */
  virtual Real computeValue();

  /// Equilibrium constant at reference temperature
  Real _log_k;
  /// Specific reactive surface area, m^2/L solution
  Real _r_area;
  /// Reference kinetic rate constant
  Real _ref_kconst;
  /// Activation energy
  Real _e_act;
  /// Gas constant, 8.314 J/mol/K
  Real _gas_const;
  /// Reference temperature
  Real _ref_temp;
  /// Actual system temperature
  Real _sys_temp;
  /// Stochiometric coefficients for involved primary species
  std::vector<Real> _sto_v;

  /// Coupled primary species concentrations
  std::vector<VariableValue *> _vals;
};

#endif //KINETICDISPRECONCAUX_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
