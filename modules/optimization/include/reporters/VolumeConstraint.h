//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ElementOptimizationConstraint.h"

class VolumeConstraint : public ElementOptimizationConstraint
{
public:
  static InputParameters validParams();

  VolumeConstraint(const InputParameters & parameters);

  virtual Real computeQpConstraint() override;
  virtual Real computeQpConstraintJacobian() override;

private:
  const MooseVariable & _density;
};
