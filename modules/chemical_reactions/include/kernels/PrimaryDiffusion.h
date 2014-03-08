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
#include "Diffusion.h"

// Forward Declarations

/**
 * Define the Kernel for a CoupledConvectionReactionSub operator that looks like:
 * grad (diff * grad_u)
=======
#include "Diffusion.h"

#ifndef PRIMARYDIFFUSION_H
#define PRIMARYDIFFUSION_H

//Forward Declarations
class PrimaryDiffusion;

/**
 * validParams returns the parameters that this Kernel accepts / needs
 * The actual body of the function MUST be in the .C file.
 */
template<>
InputParameters validParams<PrimaryDiffusion>();

/**
 * Define the Kernel for a CoupledConvectionReactionSub operator that looks like:
 *
 * grad (diff * grad_u)
 *
 * This first line is defining the name and inheriting from Kernel.
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
 */
class PrimaryDiffusion : public Diffusion
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  PrimaryDiffusion(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;

  /// Material property of dispersion-diffusion coefficient.
  const MaterialProperty<Real> & _diffusivity;
};
=======

  PrimaryDiffusion(const std::string & name, InputParameters parameters);

protected:
  /**
   * Responsible for computing the residual at one quadrature point
   *
   * This should always be defined in the .C
   * @return The residual of dispersion-diffusion of primary species.
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
   * @return The diagonal jacobian of dispersion-diffusion of primary species.
   */
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  /// Material property of dispersion-diffusion coefficient.
  MaterialProperty<Real> & _diffusivity;
};
#endif //PRIMARYDIFFUSION_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
