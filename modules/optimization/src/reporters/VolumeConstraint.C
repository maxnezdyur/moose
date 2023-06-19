//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "VolumeConstraint.h"

InputParameters
VolumeConstraint::validParams()
{
  InputParameters params = ElementOptimizationConstraint::validParams();
  params.addRequiredCoupledVar("density", "Density variable used for the constraint.");
  return params;
}

VolumeConstraint::VolumeConstraint(const InputParameters & parameters)
  : ElementOptimizationConstraint(parameters), _density(*getVar("density", 0))

{
}

Real
VolumeConstraint::computeQpConstraint()
{
  return _density.getElementalValue(_current_elem) * _current_elem_volume;
}

Real
VolumeConstraint::computeQpConstraintJacobian()
{
  return _current_elem_volume;
}
