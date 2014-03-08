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
#ifndef PRIMARYTIMEDERIVATIVE
#define PRIMARYTIMEDERIVATIVE
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "TimeDerivative.h"

// Forward Declaration
<<<<<<< HEAD

/**
 * Define the Kernel for a CoupledConvectionReactionSub operator that looks like:
 * storage * delta pressure / delta t
=======
class PrimaryTimeDerivative;

/**
 * validParams returns the parameters that this Kernel accepts / needs
 * The actual body of the function MUST be in the .C file.
 */
template<>
InputParameters validParams<PrimaryTimeDerivative>();

/**
 * Define the Kernel for a CoupledConvectionReactionSub operator that looks like:
 *
 * storage * delta pressure / delta t
 *
 * This first line is defining the name and inheriting from Kernel.
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
 */
class PrimaryTimeDerivative : public TimeDerivative
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  PrimaryTimeDerivative(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;

  /// Material property of porosity
  const MaterialProperty<Real> & _porosity;
};
=======

  PrimaryTimeDerivative(const std::string & name, InputParameters parameters);

protected:
  /**
   * Responsible for computing the residual at one quadrature point
   *
   * This should always be defined in the .C
   * @return The residual of mass accumulation of primary species concentration.
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
   * @return The diagonal jacobian of mass accumulation of primary species concentration.
   */
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  /// Material property of porosity
  MaterialProperty<Real> & _porosity;
};

#endif // PRIMARYTIMEDERIVATIVE
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
