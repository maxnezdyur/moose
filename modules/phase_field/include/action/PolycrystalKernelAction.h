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

#include "Action.h"

/**
 * Action that sets up ACGrGrPoly, ACInterface, TimeDerivative, and ACGBPoly
 * kernels.
 */
class PolycrystalKernelAction : public Action
{
public:
  static InputParameters validParams();

  PolycrystalKernelAction(const InputParameters & params);

  virtual void act();

protected:
  /// number of grains to create
  const unsigned int _op_num;

  /// base name for the order parameter variables
  const std::string _var_name_base;
};
=======
#ifndef POLYCRYSTALKERNELACTION_H
#define POLYCRYSTALKERNELACTION_H

#include "Action.h"

class PolycrystalKernelAction: public Action
{
public:
  PolycrystalKernelAction(const std::string & name, InputParameters params);

  virtual void act();

private:
  unsigned int _crys_num;
  std::string _var_name_base;
  VariableName _c;
  bool _implicit;
  VariableName _T;

};

template<>
InputParameters validParams<PolycrystalKernelAction>();

#endif //POLYCRYSTALKERNELACTION_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
