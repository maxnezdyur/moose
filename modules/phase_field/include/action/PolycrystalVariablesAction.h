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

#include "InputParameters.h"
#include "Action.h"

// Forward declaration

/**
 * Automatically generates all variables to model a polycrystal with op_num orderparameters
 */
class PolycrystalVariablesAction : public Action
{
public:
  static InputParameters validParams();

  PolycrystalVariablesAction(const InputParameters & params);
=======
#ifndef POLYCRYSTALVARIABLESACTION_H
#define POLYCRYSTALVARIABLESACTION_H

#include "InputParameters.h"
#include "Action.h"
/**
 * Automatically generates all variables to model a polycrystal with crys_num orderparameters
 */

class PolycrystalVariablesAction: public Action
{
public:
  PolycrystalVariablesAction(const std::string & name, InputParameters params);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  virtual void act();

private:
<<<<<<< HEAD
  const unsigned int _op_num;
  const std::string _var_name_base;
};
=======
  static const Real _abs_zero_tol;

  unsigned int _crys_num;
  std::string _var_name_base;
};

template<>
InputParameters validParams<PolycrystalVariablesAction>();

#endif //POLYCRYSTALVARIABLESACTION_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
