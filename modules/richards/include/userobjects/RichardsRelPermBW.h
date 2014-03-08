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
 * "Broadbridge-White" form of relative permeability as a function of effective saturation
 * P Broadbridge and I White ``Constant rate rainfall infiltration: A versatile nonlinear model 1.
 * Analytic Solution'', Water Resources Research 24 (1988) 145-154)
 */
class RichardsRelPermBW : public RichardsRelPerm
{
public:
  static InputParameters validParams();

  RichardsRelPermBW(const InputParameters & parameters);

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
  // BW's initial saturation
  Real _sn;

  // BW's effective saturation where porepressure = 0
  Real _ss;

  // relative perm at Seff = Sn
  Real _kn;

  // relative perm at Seff = Ss
  Real _ks;

  // BW's C parameter
  Real _c;

  // (_ks - _kn)*(_c - 1)
  Real _coef;
};
=======
/*****************************************/
/* Written by andrew.wilkins@csiro.au    */
/* Please contact me if you make changes */
/*****************************************/

//  "Broadbridge-White" form of relative permeability (P Broadbridge and I White ``Constant rate rainfall infiltration: A versatile nonlinear model 1. Analytic Solution'', Water Resources Research 24 (1988) 145-154)
#ifndef RICHARDSRELPERMBW_H
#define RICHARDSRELPERMBW_H

#include "RichardsRelPerm.h"

class RichardsRelPermBW;


template<>
InputParameters validParams<RichardsRelPermBW>();

class RichardsRelPermBW : public RichardsRelPerm
{
 public:
  RichardsRelPermBW(const std::string & name, InputParameters parameters);

  Real relperm(Real seff) const;
  Real drelperm(Real seff) const;
  Real d2relperm(Real seff) const;

 protected:

  Real _sn;
  Real _ss;
  Real _kn;
  Real _ks;
  Real _c;
  Real _coef;

};

#endif // RICHARDSRELPERMBW_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
