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

#include "RichardsRelPerm.h"

/**
 * Van-Genuchten form of relative permeability
 * as a function of effective saturation
 */
class RichardsRelPermVG : public RichardsRelPerm
{
public:
  static InputParameters validParams();

  RichardsRelPermVG(const InputParameters & parameters);

  /**
   * relative permeability as a function of effective saturation
   * @param seff effective saturation
   */
  Real relperm(Real seff) const;

  /**
   * derivative of relative permeability wrt effective saturation
   * @param seff effective saturation
   */
  Real drelperm(Real seff) const;

  /**
   * second derivative of relative permeability wrt effective saturation
   * @param seff effective saturation
   */
  Real d2relperm(Real seff) const;

protected:
  /// immobile saturation
  Real _simm;

  /// van Genuchten m parameter
  Real _m;
};
=======
/*****************************************/
/* Written by andrew.wilkins@csiro.au    */
/* Please contact me if you make changes */
/*****************************************/

//  van-Genuchten form of relative permeability
//
#ifndef RICHARDSRELPERMVG_H
#define RICHARDSRELPERMVG_H

#include "RichardsRelPerm.h"

class RichardsRelPermVG;


template<>
InputParameters validParams<RichardsRelPermVG>();

class RichardsRelPermVG : public RichardsRelPerm
{
 public:
  RichardsRelPermVG(const std::string & name, InputParameters parameters);

  Real relperm(Real seff) const;
  Real drelperm(Real seff) const;
  Real d2relperm(Real seff) const;

 protected:

  Real _simm;
  Real _m;

};

#endif // RICHARDSRELPERMVG_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
