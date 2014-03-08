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

#include "TimeDerivative.h"

/**
 * Time derivative of primary species in given equilibrium species
 */
class CoupledBEEquilibriumSub : public TimeDerivative
{
public:
  static InputParameters validParams();

  CoupledBEEquilibriumSub(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

private:
  /// Weight of the equilibrium species in the total primary species
  const Real _weight;
  /// Equilibrium constant for the equilibrium species
  const VariableValue & _log_k;
  /// Stoichiometric coefficient of the primary species in the equilibrium species
  const Real _sto_u;
  /// Stoichiometric coefficients of the coupled primary species in the equilibrium species
  const std::vector<Real> _sto_v;
  /// Activity coefficient of primary species in the equilibrium species
  const VariableValue & _gamma_u;
  /// Old activity coefficient of primary species in the equilibrium species
  const VariableValue & _gamma_u_old;
  /// Activity coefficients of coupled primary species in the equilibrium species
  const std::vector<const VariableValue *> _gamma_v;
  /// Old activity coefficients of coupled primary species in the equilibrium species
  const std::vector<const VariableValue *> _gamma_v_old;
  /// Activity coefficient of equilibrium species
  const VariableValue & _gamma_eq;
  /// Old activity coefficient of equilibrium species
  const VariableValue & _gamma_eq_old;
  /// Porosity
  const MaterialProperty<Real> & _porosity;
  /// Coupled primary species variable numbers
  const std::vector<unsigned int> _vars;
  /// Coupled primary species concentrations
  const std::vector<const VariableValue *> _v_vals;
  /// Old values of coupled primary species concentrations
  const std::vector<const VariableValue *> _v_vals_old;
  /// Old value of the primary species concentration.
  const VariableValue & _u_old;
};
=======
#include "Kernel.h"

#ifndef COUPLEDBEEQUILIBRIUMSUB_H
#define COUPLEDBEEQUILIBRIUMSUB_H

/**
 * The forward declaration is so that we can declare the validParams function
 * before we actually define the class... that way the definition isn't lost
 * at the bottom of the file.
 */

//Forward Declarations
class CoupledBEEquilibriumSub;

/**
 * validParams returns the parameters that this Kernel accepts / needs
 * The actual body of the function MUST be in the .C file.
 */
template<>
InputParameters validParams<CoupledBEEquilibriumSub>();

/**
 * Define the Kernel for a CoupledBEEquilibriumSub operator that looks like:
 *
 * delta (weight * 10^log_k * u^sto_u * v^sto_v) / delta t.
 */
class CoupledBEEquilibriumSub : public Kernel
{
public:

  /**
   * This is the Constructor declaration AND definition.
   * It is ok to have the definition in the .h if the function body
   * is really small.  Otherwise it should be in the .C
   */
  CoupledBEEquilibriumSub(const std::string & name, InputParameters parameters);

protected:
  /**
   * Responsible for computing the residual at one quadrature point
   * This should always be defined in the .C
   * @return The residual of mass accumulation of the coupled equilibrium species concentration.
   */
  virtual Real computeQpResidual();

  /**
   * Responsible for computing the diagonal block of the preconditioning matrix.
   * This is essentially the partial derivative of the residual with respect to
   * the variable this kernel operates on ("u").
   *
   * Note that this can be an approximation or linearization.  In this case it's
   * not because the Jacobian of this operator is easy to calculate.
   *
   * This should always be defined in the .C
   * @return The diagonal jacobian of mass accumulation of the coupled equilibrium species concentration.
   */
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  /// Weight of the equilibrium species concentration in the total primary species concentration.
  Real _weight;
  /// Equilibrium constant for the equilibrium species in association form.
  Real _log_k;
  /// Stochiometric coefficient of the primary species.
  Real _sto_u;
  /// Stochiometric coefficiets of the coupled primary species.
  std::vector<Real> _sto_v;

  /// Material property of porosity.
  MaterialProperty<Real> & _porosity;

  std::vector<unsigned int> _vars;
  /// Coupled primary species concentrations.
  std::vector<VariableValue *> _v_vals;
  /// Coupled old values of primary species concentrations.
  std::vector<VariableValue *> _v_vals_old;
  /// The old values of the primary species concentration.
  VariableValue & _u_old;
};
#endif //COUPLEDBEEQUILIBRIUMSUB_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
