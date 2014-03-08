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
 * Bicrystal with a circular grain and an embedding outer grain
 */
class BicrystalCircleGrainICAction : public Action
{
public:
  static InputParameters validParams();

  BicrystalCircleGrainICAction(const InputParameters & params);
=======
#ifndef BICRYSTALCIRCLEGRAINICACTION_H
#define BICRYSTALCIRCLEGRAINICACTION_H

#include "InputParameters.h"
#include "Action.h"
/**
 * Automatically generates all variables to model a polycrystal with crys_num orderparameters
 */

class BicrystalCircleGrainICAction: public Action
{
public:
  BicrystalCircleGrainICAction(const std::string & name, InputParameters params);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  virtual void act();

private:
<<<<<<< HEAD
  const std::string _var_name_base;
  const unsigned int _op_num;

  const Real _radius;
  const Real _x, _y, _z;
  const Real _int_width;

  const bool _3D_sphere;
};
=======
  static const Real _abs_zero_tol;

  std::string _var_name_base;
  unsigned int _crys_num;

  Real _radius;
  Real _x, _y, _z;
  Real _int_width;

  bool _3D_sphere;
};

template<>
InputParameters validParams<BicrystalCircleGrainICAction>();

#endif //BICRYSTALCIRCLEGRAINICACTION_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
