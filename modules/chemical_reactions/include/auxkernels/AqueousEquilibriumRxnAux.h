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
 * Calculates equilibrium species concentration according to the mass action law
=======
#ifndef AQUEOUSEQUILIBRIUMRXNAUX_H
#define AQUEOUSEQUILIBRIUMRXNAUX_H

#include "AuxKernel.h"


//Forward Declarations
class AqueousEquilibriumRxnAux;

/**
 * validParams returns the parameters that this Kernel accepts / needs
 * The actual body of the function MUST be in the .C file.
 */
template<>
InputParameters validParams<AqueousEquilibriumRxnAux>();

/**
 * Define the AuxKernel for the output of equilibrium species concentrations
 * according to mass action law.
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
 */
class AqueousEquilibriumRxnAux : public AuxKernel
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  AqueousEquilibriumRxnAux(const InputParameters & parameters);

protected:
  virtual Real computeValue() override;

  /// Equilibrium constant
  const VariableValue & _log_k;
  /// Stoichiometric coefficients of coupled primary species
  const std::vector<Real> _sto_v;
  /// Activity coefficient of equilibrium species
  const VariableValue & _gamma_eq;
  /// Coupled primary species
  const std::vector<const VariableValue *> _vals;
  /// Activity coefficients of coupled primary species
  const std::vector<const VariableValue *> _gamma_v;
};
=======

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  AqueousEquilibriumRxnAux(const std::string & name, InputParameters parameters);

  virtual ~AqueousEquilibriumRxnAux() {}

protected:
  /**
   * Conputes the equilibrium sepecies concentration.
   * @return The concentration of an equilibrium species.
   */
  virtual Real computeValue();

  /// Equilibrium constant
  Real _log_k;
  /// Stochiometric coefficients for coupled primary species
  std::vector<Real> _sto_v;

  /// Coupled primary species
  std::vector<VariableValue *>  _vals;
};

#endif //AQUEOUSEQUILIBRIUMRXNAUX_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
