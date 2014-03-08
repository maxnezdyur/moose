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

#include "Kernel.h"
#include "DerivativeMaterialInterface.h"

/**
 * Convection of primary species in given equilibrium species
 */
class CoupledConvectionReactionSub : public DerivativeMaterialInterface<Kernel>
{
public:
  static InputParameters validParams();

  CoupledConvectionReactionSub(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

  /// Weight of the equilibrium species concentration in the total primary species concentration
  const Real _weight;
  /// Equilibrium constant for the equilibrium species in association form
  const VariableValue & _log_k;
  /// Stoichiometric coefficient of the primary species
  const Real _sto_u;
  /// Stoichiometric coefficients of the coupled primary species
  const std::vector<Real> _sto_v;
  /// Hydraulic conductivity
  const MaterialProperty<Real> & _cond;
  /// Gravity
  const RealVectorValue _gravity;
  /// Fluid density
  const MaterialProperty<Real> & _density;
  /// Pressure gradient
  const VariableGradient & _grad_p;
  /// Pressure variable number
  const unsigned int _pvar;
  /// Coupled primary species variable numbers
  const std::vector<unsigned int> _vars;
  /// Coupled primary species concentrations
  const std::vector<const VariableValue *> _vals;
  /// Coupled gradients of primary species concentrations
  const std::vector<const VariableGradient *> _grad_vals;
  /// Activity coefficient of primary species in the equilibrium species
  const VariableValue & _gamma_u;
  /// Activity coefficients of coupled primary species in the equilibrium species
  const std::vector<const VariableValue *> _gamma_v;
  /// Activity coefficient of equilibrium species
  const VariableValue & _gamma_eq;
};
=======
#include "Kernel.h"

#ifndef COUPLEDCONVECTIONREACTIONSUB_H
#define COUPLEDCONVECTIONREACTIONSUB_H

/**
 * The forward declaration is so that we can declare the validParams function
 * before we actually define the class... that way the definition isn't lost
 * at the bottom of the file.
 */

//Forward Declarations
class CoupledConvectionReactionSub;

/**
 * validParams returns the parameters that this Kernel accepts / needs
 * The actual body of the function MUST be in the .C file.
 */
template<>
InputParameters validParams<CoupledConvectionReactionSub>();

/**
 * Define the Kernel for a CoupledConvectionReactionSub operator that looks like:
 *
 * weight * cond * grad_pressure * 10^log_k * u^sto_u * v^sto_v
 *
 * This first line is defining the name and inheriting from Kernel.
 */
class CoupledConvectionReactionSub : public Kernel
{
public:

  /**
   * This is the Constructor declaration AND definition.
   * It is ok to have the definition in the .h if the function body
   * is really small.  Otherwise it should be in the .C
   */
  CoupledConvectionReactionSub(const std::string & name, InputParameters parameters);

protected:
  /**
   * Responsible for computing the residual at one quadrature point
   *
   * This should always be defined in the .C
   * @return The residual of convection of the coupled equilibrium species.
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
   * @return The diagonal jacobian of convection of the coupled equilibrium species.
   */
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  /**
   * Coupled things come through as std::vector _refernces_.
   *
   * Since this is a reference it MUST be set in the Initialization List of the
   * constructor!
   */
  /// Weight of the equilibrium species concentration in the total primary species concentration.
  Real _weight;
  /// Equilibrium constant for the equilibrium species in association form.
  Real _log_k;
  /// Stochiometric coefficient of the primary species.
  Real _sto_u;
  /// Stochiometric coefficiets of the coupled primary species.
  std::vector<Real> _sto_v;

  /// Material property of hydraulic conductivity.
  MaterialProperty<Real> & _cond;
  /// Coupled gradient of hydraulic head.
  VariableGradient & _grad_p;

  std::vector<unsigned int> _vars;
  /// Coupled primary species concentrations.
  std::vector<VariableValue *> _vals;
  /// Coupled gradients of primary species concentrations.
  std::vector<VariableGradient *> _grad_vals;

};
#endif //COUPLEDCONVECTIONREACTIONSUB_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
