//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "ElementIntegralVariablePostprocessor.h"

/**
 * Total hourglass stabilization energy sum_e eps (gamma . u)^2 with
 * eps = penalty * diffusivity * area / h^2. Zero (to machine precision) for
 * affine fields; a nonzero value measures how much non-affine content the
 * stabilization is penalizing.
 */
class HourglassEnergy : public ElementIntegralVariablePostprocessor
{
public:
  static InputParameters validParams();

  HourglassEnergy(const InputParameters & parameters);

  void execute() override;

protected:
  Real computeQpIntegral() override { return 0; }

  const Real _penalty;
  const ADMaterialProperty<Real> & _diff;
  const VariableValue & _v_nodal;
};
