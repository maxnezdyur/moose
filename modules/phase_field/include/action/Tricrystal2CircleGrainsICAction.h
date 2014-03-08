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

/**
 * Automatically generates all variables to model a polycrystal with op_num orderparameters
 */
class Tricrystal2CircleGrainsICAction : public Action
{
public:
  static InputParameters validParams();

  Tricrystal2CircleGrainsICAction(const InputParameters & params);
=======
#ifndef TRICRYSTAL2CIRCLEGRAINSICACTION_H
#define TRICRYSTAL2CIRCLEGRAINSICACTION_H

#include "InputParameters.h"
#include "Action.h"
/**
 * Automatically generates all variables to model a polycrystal with crys_num orderparameters
 */

class Tricrystal2CircleGrainsICAction: public Action
{
public:
  Tricrystal2CircleGrainsICAction(const std::string & name, InputParameters params);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  virtual void act();

private:
  static const Real _abs_zero_tol;

  std::string _var_name_base;
<<<<<<< HEAD
  unsigned int _op_num;
};
=======
  unsigned int _crys_num;

};

template<>
InputParameters validParams<Tricrystal2CircleGrainsICAction>();

#endif //TRICRYSTAL2CIRCLEGRAINSICACTION_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
