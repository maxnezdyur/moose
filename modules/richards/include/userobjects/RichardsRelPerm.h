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
 * Base class for Richards relative permeability classes
 * that provide relative permeability as a function of effective saturation
 */
class RichardsRelPerm : public GeneralUserObject
{
public:
  static InputParameters validParams();

  RichardsRelPerm(const InputParameters & parameters);
=======
/*****************************************/
/* Written by andrew.wilkins@csiro.au    */
/* Please contact me if you make changes */
/*****************************************/

//  Base class for relative permeability as a function of effective saturation
//
#ifndef RICHARDSRELPERM_H
#define RICHARDSRELPERM_H

#include "GeneralUserObject.h"

class RichardsRelPerm;


template<>
InputParameters validParams<RichardsRelPerm>();

class RichardsRelPerm : public GeneralUserObject
{
 public:
  RichardsRelPerm(const std::string & name, InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  void initialize();
  void execute();
  void finalize();

<<<<<<< HEAD
  /**
   * relative permeability as a function of effective saturation
   * This must be over-ridden in your derived class to provide actual
   * values of relative permeability
   * @param seff effective saturation
   */
  virtual Real relperm(Real seff) const = 0;

  /**
   * derivative of relative permeability wrt effective saturation
   * This must be over-ridden in your derived class to provide actual
   * values of derivative of relative permeability
   * @param seff effective saturation
   */
  virtual Real drelperm(Real seff) const = 0;

  /**
   * second derivative of relative permeability wrt effective saturation
   * This must be over-ridden in your derived class to provide actual
   * values of second derivative of relative permeability
   * @param seff effective saturation
   */
  virtual Real d2relperm(Real seff) const = 0;
};
=======
  // These functions must be over-ridden in the derived class
  // to provide the actual values of relative perm and its derivatives
  virtual Real relperm(Real seff) const = 0;
  virtual Real drelperm(Real seff) const = 0;
  virtual Real d2relperm(Real seff) const = 0;

};

#endif // RICHARDSRELPERM_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
