//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "MooseTypes.h"
#include "ElementReporter.h"
#include "ElementMapUO.h"
#include <vector>

class ElementOptimizationConstraint : public ElementReporter
{
public:
  static InputParameters validParams();

  ElementOptimizationConstraint(const InputParameters & parameters);
  virtual void initialize() override;
  virtual void execute() override;
  virtual void threadJoin(const UserObject & /*uo*/) override{};
  virtual void finalize() override;

  virtual Real computeQpConstraint() = 0;
  virtual Real computeQpConstraintJacobian() = 0;

private:
  // Name of the constraint
  std::string _base_name;
  /// Value to hold the constraint
  Real & _constraint;
  /// Value to hold the jacobian
  std::vector<Real> & _constraint_jacobian;
  // Map needed to keep the gradient data consistent
  const ElementMapUO::ElementVectorMap & _elem_vec_map;
  // Upper or equality bound for the constraint.
  const Real _bound;
};
