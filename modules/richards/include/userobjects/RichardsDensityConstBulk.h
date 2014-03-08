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
 * Fluid density assuming constant bulk modulus
 */
class RichardsDensityConstBulk : public RichardsDensity
{
public:
  static InputParameters validParams();

  RichardsDensityConstBulk(const InputParameters & parameters);

  /**
   * fluid density as a function of porepressure
   * @param p porepressure
   */
  Real density(Real p) const;

  /**
   * derivative of fluid density wrt porepressure
   * @param p porepressure
   */
  Real ddensity(Real p) const;

  /**
   * second derivative of fluid density wrt porepressure
   * @param p porepressure
   */
  Real d2density(Real p) const;

protected:
  /// density = _dens0*exp(p/_bulk)
  Real _dens0;

  /// density = _dens0*exp(p/_bulk)
  Real _bulk;
};
=======
/*****************************************/
/* Written by andrew.wilkins@csiro.au    */
/* Please contact me if you make changes */
/*****************************************/

//  Fluid density assuming constant bulk modulus
//
#ifndef RICHARDSDENSITYCONSTBULK_H
#define RICHARDSDENSITYCONSTBULK_H

#include "RichardsDensity.h"

class RichardsDensityConstBulk;


template<>
InputParameters validParams<RichardsDensityConstBulk>();

class RichardsDensityConstBulk : public RichardsDensity
{
 public:
  RichardsDensityConstBulk(const std::string & name, InputParameters parameters);

  Real density(Real p) const;
  Real ddensity(Real p) const;
  Real d2density(Real p) const;

 protected:
  Real _dens0;
  Real _bulk;
};

#endif // RICHARDSDENSITYCONSTBULK_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
