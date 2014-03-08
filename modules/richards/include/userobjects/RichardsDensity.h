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

#include "GeneralUserObject.h"

/**
 * Base class for fluid density as a function of porepressure
 * The functions density, ddensity and d2density must be
 * over-ridden in derived classes to provide actual values
 */
class RichardsDensity : public GeneralUserObject
{
public:
  static InputParameters validParams();

  RichardsDensity(const InputParameters & parameters);
=======
/*****************************************/
/* Written by andrew.wilkins@csiro.au    */
/* Please contact me if you make changes */
/*****************************************/

//  Base class for fluid density as a function of pressure
//
#ifndef RICHARDSDENSITY_H
#define RICHARDSDENSITY_H

#include "GeneralUserObject.h"

class RichardsDensity;


template<>
InputParameters validParams<RichardsDensity>();

class RichardsDensity : public GeneralUserObject
{
 public:
  RichardsDensity(const std::string & name, InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  void initialize();
  void execute();
  void finalize();

<<<<<<< HEAD
  /**
   * fluid density as a function of porepressure
   * This must be over-ridden in derived classes to provide an actual value
   * @param p porepressure
   */
  virtual Real density(Real p) const = 0;

  /**
   * derivative of fluid density wrt porepressure
   * This must be over-ridden in derived classes to provide an actual value
   * @param p porepressure
   */
  virtual Real ddensity(Real p) const = 0;

  /**
   * second derivative of fluid density wrt porepressure
   * This must be over-ridden in derived classes to provide an actual value
   * @param p porepressure
   */
  virtual Real d2density(Real p) const = 0;
};
=======
  // These functions must be over-ridden in the derived class
  // to provide the actual values of density and its derivatives
  virtual Real density(Real p) const = 0;
  virtual Real ddensity(Real p) const = 0;
  virtual Real d2density(Real p) const = 0;

};

#endif // RICHARDSDENSITY_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
