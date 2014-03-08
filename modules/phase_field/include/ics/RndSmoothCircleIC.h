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

#include "SmoothCircleIC.h"
=======
#ifndef RNDSMOOTHCIRCLEIC_H
#define RNDSMOOTHCIRCLEIC_H

#include "Kernel.h"
#include "InitialCondition.h"

// System includes
#include <string>

// Forward Declarations
class RndSmoothCircleIC;

template<>
InputParameters validParams<RndSmoothCircleIC>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * RndSmoothcircleIC creates a smooth circle with a random distribution
 * of values inside and outside of the circle.
<<<<<<< HEAD
 */
class RndSmoothCircleIC : public SmoothCircleIC
{
public:
  static InputParameters validParams();

  RndSmoothCircleIC(const InputParameters & parameters);

private:
  virtual Real computeCircleValue(const Point & p, const Point & center, const Real & radius);

  const Real _variation_invalue;
  const Real _variation_outvalue;
};
=======
 **/
class RndSmoothCircleIC : public InitialCondition
{
public:

  /**
   * Constructor
   *
   * @param name The name given to the initial condition in the input file.
   * @param parameters The parameters object holding data for the class to use.
   * @param var_name The variable this InitialCondtion is supposed to provide values for.
   */
  RndSmoothCircleIC(const std::string & name,
                 InputParameters parameters);

  /**
   * The value of the variable at a point.
   *
   * This must be overriden by derived classes.
   */
  virtual Real value(const Point & p);

private:
  Real _x1;
  Real _y1;
  Real _z1;
  Real _mx_invalue;
  Real _mn_invalue;
  Real _mx_outvalue;
  Real _mn_outvalue;
  Real _range_invalue;
  Real _range_outvalue;
  Real _radius;

  Point _center;

};

#endif //SMOOTHCIRCLEIC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
