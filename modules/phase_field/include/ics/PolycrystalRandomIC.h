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

#include "RandomICBase.h"

/**
 * Random initial condition for a polycrystalline material
 */
class PolycrystalRandomIC : public RandomICBase
{
public:
  static InputParameters validParams();

  PolycrystalRandomIC(const InputParameters & parameters);

  virtual Real value(const Point & p);

private:
  const unsigned int _op_num;
  const unsigned int _op_index;
  const unsigned int _random_type;
};
=======
#ifndef POLYCRYSTALRANDOMIC_H
#define POLYCRYSTALRANDOMIC_H

#include "Kernel.h"
#include "InitialCondition.h"

// System includes
#include <string>

// Forward Declarations
class PolycrystalRandomIC;

template<>
InputParameters validParams<PolycrystalRandomIC>();

/**
 * PolycrystalRandomIC allows a random initial condition of a
*/
class PolycrystalRandomIC : public InitialCondition
{
public:

  /**
   * Constructor
   *
   * @param name The name given to the initial condition in the input file.
   * @param parameters The parameters object holding data for the class to use.
   * @param var_name The variable this InitialCondtion is supposed to provide values for.
   */
  PolycrystalRandomIC(const std::string & name,
                InputParameters parameters);

  /**
   * The value of the variable at a point.
   *
   * This must be overriden by derived classes.
   */
  virtual Real value(const Point & p);

private:

  unsigned int _crys_num;
  unsigned int _crys_index;
  unsigned int _typ;

};

#endif //POLYCRYSTALRANDOMIC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
