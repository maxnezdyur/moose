//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "ADRayKernel.h"

class Function;

/**
 * Newton-cooling line heat sink: contributes h(x,t) * (T - T_sink(x,t)) along each
 * segment of a ray traced through the mesh by a RepeatableRayStudy. The trace fires
 * per crossed element, so the spatial distribution of the residual is mesh-independent
 * by construction. Both h and T_sink are Functions (use ConstantFunction for constants).
 */
class ADLineHeatSinkRayKernel : public ADRayKernel
{
public:
  static InputParameters validParams();

  ADLineHeatSinkRayKernel(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual() override;

  /// Heat transfer coefficient h(x, t) for the line sink
  const Function & _h;

  /// Temperature of the line sink fluid T_sink(x, t)
  const Function & _T_sink;
};
