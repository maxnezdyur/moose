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

#include "SmoothCircleBaseIC.h"
=======
#ifndef LATTICESMOOTHCIRCLEIC_H
#define LATTICESMOOTHCIRCLEIC_H

#include "Kernel.h"
#include "MultiSmoothCircleIC.h"

// System includes
#include <string>

// Forward Declarations
class LatticeSmoothCircleIC;

template<>
InputParameters validParams<LatticeSmoothCircleIC>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * LatticeSmoothcircleIC creates a lattice of smoothcircles as an initial condition.
 * They are either directly on the lattice or randomly perturbed from the lattice.
<<<<<<< HEAD
 */
class LatticeSmoothCircleIC : public SmoothCircleBaseIC
{
public:
  static InputParameters validParams();

  LatticeSmoothCircleIC(const InputParameters & parameters);
=======
 **/
class LatticeSmoothCircleIC : public MultiSmoothCircleIC
{
public:

  /**
   * Constructor
   *
   * @param name The name given to the initial condition in the input file.
   * @param parameters The parameters object holding data for the class to use.
   * @param var_name The variable this InitialCondtion is supposed to provide values for.
   */
  LatticeSmoothCircleIC(const std::string & name,
                         InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  virtual void initialSetup();

protected:
<<<<<<< HEAD
  virtual void computeCircleRadii();
  virtual void computeCircleCenters();

  Real _lattice_variation;
  std::vector<unsigned int> _circles_per_side;
  unsigned int _numbub;

  Real _radius;
  Real _radius_variation;
  MooseEnum _radius_variation_type;

  bool _avoid_bounds;

  Point _bottom_left;
  Point _top_right;
  Point _range;
};
=======
  Real _Rnd_variation;
  std::vector<unsigned int> _circles_per_side;

// private:

};

#endif //LATTICESMOOTHCIRCLEIC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
