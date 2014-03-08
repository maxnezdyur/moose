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
 * Bicrystal using a bounding box
 */
class BicrystalBoundingBoxICAction : public Action
{
public:
  static InputParameters validParams();

  BicrystalBoundingBoxICAction(const InputParameters & params);
=======
#ifndef BICRYSTALBOUNDINGBOXICACTION_H
#define BICRYSTALBOUNDINGBOXICACTION_H

#include "InputParameters.h"
#include "Action.h"
/**
 * Automatically generates all variables to model a polycrystal with crys_num orderparameters
 */

class BicrystalBoundingBoxICAction: public Action
{
public:
  BicrystalBoundingBoxICAction(const std::string & name, InputParameters params);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  virtual void act();

private:
<<<<<<< HEAD
  const std::string _var_name_base;
  const unsigned int _op_num;
};
=======
  static const Real _abs_zero_tol;

  std::string _var_name_base;
  unsigned int _crys_num;

  Real _x1, _y1, _z1;
  Real _x2, _y2, _z2;
};

template<>
InputParameters validParams<BicrystalBoundingBoxICAction>();

#endif //BICRYSTALBOUNDINGBOXICACTION_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
