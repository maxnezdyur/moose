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
 * Saturation of a phase as a function of
 * effective saturation of that phase,
 * and its derivatives wrt effective saturation
 */
class RichardsSat : public GeneralUserObject
{
public:
  static InputParameters validParams();

  RichardsSat(const InputParameters & parameters);
=======
/*****************************************/
/* Written by andrew.wilkins@csiro.au    */
/* Please contact me if you make changes */
/*****************************************/

//  saturation as a function of effective saturation, and its derivs wrt effective saturation
//
#ifndef RICHARDSSAT_H
#define RICHARDSSAT_H

#include "GeneralUserObject.h"

class RichardsSat;


template<>
InputParameters validParams<RichardsSat>();

class RichardsSat : public GeneralUserObject
{
 public:
  RichardsSat(const std::string & name, InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  void initialize();
  void execute();
  void finalize();

<<<<<<< HEAD
  /**
   * saturation as a function of effective saturation
   * @param seff effective saturation
   */
  Real sat(Real seff) const;

  /// derivative of saturation wrt effective saturation
  Real dsat(Real /*seff*/) const;

protected:
  /// residual saturation for this phase
  Real _s_res;

  /// sum of the residual saturations for every phase
  Real _sum_s_res;
};
=======
  Real sat(Real seff) const;
  Real dsat(Real /*seff*/) const;
  Real d2sat(Real /*seff*/) const;

 protected:

  Real _s_res;
  Real _sum_s_res;

};

#endif // RICHARDSSAT_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
