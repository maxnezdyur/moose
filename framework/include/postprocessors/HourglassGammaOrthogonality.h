//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "ElementPostprocessor.h"

/**
 * Verification diagnostic: the worst violation, over all QUAD4 elements, of
 * the hourglass-gamma orthogonality conditions gamma.1 = gamma.x = gamma.y = 0
 * (normalized by |gamma|). Must be at machine precision on any mesh,
 * including distorted quads; a wrong projection passes on rectangles and
 * fails here.
 */
class HourglassGammaOrthogonality : public ElementPostprocessor
{
public:
  static InputParameters validParams();

  HourglassGammaOrthogonality(const InputParameters & parameters);

  void initialize() override { _max_violation = 0; }
  void execute() override;
  void finalize() override;
  Real getValue() const override { return _max_violation; }
  void threadJoin(const UserObject & y) override;

protected:
  Real _max_violation;
};
