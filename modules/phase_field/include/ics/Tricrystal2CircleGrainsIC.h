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

#include "InitialCondition.h"

/**
 * Tricrystal2CircleGrainsIC creates a 3 grain structure with 2 circle grains and one matrix grain
 */
class Tricrystal2CircleGrainsIC : public InitialCondition
{
public:
  static InputParameters validParams();

  Tricrystal2CircleGrainsIC(const InputParameters & parameters);

  virtual Real value(const Point & p);

protected:
  const MooseMesh & _mesh;

  const unsigned int _op_num;
  const unsigned int _op_index;
=======
#ifndef TRICRYSTAL2CIRCLEGRAINSIC_H
#define TRICRYSTAL2CIRCLEGRAINSIC_H

#include "Kernel.h"
#include "InitialCondition.h"

// System includes
#include <string>

// Forward Declarations
class Tricrystal2CircleGrainsIC;

template<>
InputParameters validParams<Tricrystal2CircleGrainsIC>();

/**
 * Tricrystal2CircleGrainsIC creates a 3 grain structure with 2 circle grains and one matrix grain
*/
class Tricrystal2CircleGrainsIC : public InitialCondition
{
public:

  /**
   * Constructor
   *
   * @param name The name given to the initial condition in the input file.
   * @param parameters The parameters object holding data for the class to use.
   * @param var_name The variable this InitialCondtion is supposed to provide values for.
   */
  Tricrystal2CircleGrainsIC(const std::string & name,
                InputParameters parameters);

  /**
   * The value of the variable at a point.
   *
   * This must be overriden by derived classes.
   */
  virtual Real value(const Point & p);

protected:
  MooseMesh & _mesh;
  /// A reference to the nonlinear system
  NonlinearSystem & _nl;

  unsigned int _crys_num;
  unsigned int _crys_index;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  Point _bottom_left;
  Point _top_right;
  Point _range;
<<<<<<< HEAD
};
=======

};

#endif //TRICRYSTAL2CIRCLEGRAINSIC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
