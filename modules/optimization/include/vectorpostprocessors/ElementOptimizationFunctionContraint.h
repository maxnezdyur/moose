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
#include "ElementVariableVectorPostprocessor.h"
#include "ElementVariablePostprocessor.h"

class ElementOptimizationFunctionContraint : public VariableVectorPostprocessor,
                                             public ElementVariablePostprocessor
{
public:
  static InputParameters validParams();

  ElementOptimizationFunctionContraint(const InputParameters & parameters);

  /// Value to hold the constraint
  PostprocessorValue & _constraint;
  /// Value to hold the jacobian
  VectorPostprocessorValue & _constraint_jacobian;
};
