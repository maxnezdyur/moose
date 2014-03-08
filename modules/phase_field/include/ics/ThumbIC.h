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

=======
#ifndef THUMBIC_H
#define THUMBIC_H

#include "Kernel.h"
#include "InitialCondition.h"

// System includes
#include <string>

// Forward Declarations
class ThumbIC;

template<>
InputParameters validParams<ThumbIC>();

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
/**
 * ThumbIC creates a rectangle with a half circle on top
 */
class ThumbIC : public InitialCondition
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  ThumbIC(const InputParameters & parameters);

  virtual Real value(const Point & p);

protected:
  const Real _xcoord;
  const Real _width;
  const Real _height;
  const Real _invalue;
  const Real _outvalue;
};
=======

  /**
   * Constructor
   *
   * @param name The name given to the initial condition in the input file.
   * @param parameters The parameters object holding data for the class to use.
   * @param var_name The variable this InitialCondtion is supposed to provide values for.
   */
  ThumbIC(const std::string & name,
                 InputParameters parameters);

  /**
   * The value of the variable at a point.
   *
   * This must be overriden by derived classes.
   */
  virtual Real value(const Point & p);

protected:

  Real _xcoord;
  Real _width;
  Real _height;
  Real _invalue;
  Real _outvalue;

};

#endif //THUMBIC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
