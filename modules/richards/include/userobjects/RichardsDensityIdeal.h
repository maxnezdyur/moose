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

#include "RichardsDensity.h"

/**
 * Fluid density of an ideal gas
 */
class RichardsDensityIdeal : public RichardsDensity
{
public:
  static InputParameters validParams();

  RichardsDensityIdeal(const InputParameters & parameters);

  /**
   * fluid density as a function of porepressure
   * @param p porepressure
   */
  Real density(Real p) const;

  /**
   * derivative of fluid density wrt porepressure
   */
  Real ddensity(Real /*p*/) const;

  /**
   * second derivative of fluid density wrt porepressure
   */
  Real d2density(Real /*p*/) const;

protected:
  /// density = _slope*(p - _p0)
  Real _slope;

  /// density = _slope*(p - _p0)
  Real _p0;
};
=======
/*****************************************/
/* Written by andrew.wilkins@csiro.au    */
/* Please contact me if you make changes */
/*****************************************/

//  Fluid density ideal gas
//
#ifndef RICHARDSDENSITYIDEAL_H
#define RICHARDSDENSITYIDEAL_H

#include "RichardsDensity.h"

class RichardsDensityIdeal;


template<>
InputParameters validParams<RichardsDensityIdeal>();

class RichardsDensityIdeal : public RichardsDensity
{
 public:
  RichardsDensityIdeal(const std::string & name, InputParameters parameters);

  Real density(Real p) const;
  Real ddensity(Real /*p*/) const;
  Real d2density(Real /*p*/) const;

 protected:
  Real _slope;
  Real _p0;
};

#endif // RICHARDSDENSITYIDEAL_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
