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
 * Define the Kernel for a PrimaryConvection operator that looks like:
 * cond * grad_pressure * grad_u
 */
class PrimaryConvection : public DerivativeMaterialInterface<Kernel>
{
public:
  static InputParameters validParams();

  PrimaryConvection(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

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
};
=======
#include "Kernel.h"

#ifndef PRIMARYCONVECTION_H
#define PRIMARYCONVECTION_H

// Forward Declaration
class PrimaryConvection;

template<>
InputParameters validParams<PrimaryConvection>();

/**
 * Define the Kernel for a PrimaryConvection operator that looks like:
 *
 * cond * grad_pressure * grad_u
 *
 * This first line is defining the name and inheriting from Kernel.
 */
class PrimaryConvection : public Kernel
{
public:

  /**
   * This is the Constructor declaration AND definition.
   * It is ok to have the definition in the .h if the function body
   * is really small.  Otherwise it should be in the .C
   */
  PrimaryConvection(const std::string & name, InputParameters parameters);

protected:
  /**
   * Responsible for computing the residual at one quadrature point
   *
   * This should always be defined in the .C
   * @return The residual of the convection of primary species.
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
   * @return The diagonal jacobian of the convection of primary species.
   */
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  /// Material property of hydraulic conductivity
  MaterialProperty<Real> & _cond;

private:
  /**
   * Coupled things come through as std::vector _refernces_.
   *
   * Since this is a reference it MUST be set in the Initialization List of the
   * constructor!
   */
  /// Coupled gradient of hydraulic head.
  VariableGradient & _grad_p;
};
#endif //PRIMARYCONVECTION_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
