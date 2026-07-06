//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "ADKernel.h"

/**
 * Implicit hourglass stabilization for a scalar (diffusion-type) field on
 * underintegrated QUAD4/HEX8 elements: K_stab = eps * gamma * gamma^T with the
 * Flanagan-Belytschko gamma (orthogonal to affine fields by construction) and
 * the physically-based scale eps = penalty * D * V / h^2 evaluated with the
 * diffusivity at the single quadrature point. Automatic differentiation
 * provides the exact consistent tangent, including every dependence of the
 * diffusivity on the stabilized variable or coupled variables - no derivative
 * declarations required.
 */
class HourglassCorrectionDiffusion : public ADKernel
{
public:
  static InputParameters validParams();

  HourglassCorrectionDiffusion(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual() override;

  /// Base penalty; the default 1/6 reproduces the fully-integrated hourglass
  /// stiffness exactly on rectangles (QUAD4) and rectangular parallelepipeds
  /// (HEX8, with mode weights 1/2, 1/2, 1/2, 1/4)
  const Real _penalty;

  /// Diffusivity (e.g. thermal conductivity), evaluated at the single qp
  const ADMaterialProperty<Real> & _diff;

  /// Nodal values of the stabilized variable (AD)
  const MooseArray<ADReal> & _v_nodal;
};
