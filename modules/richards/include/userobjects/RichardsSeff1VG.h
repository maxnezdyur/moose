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
=======
/*****************************************/
/* Written by andrew.wilkins@csiro.au    */
/* Please contact me if you make changes */
/*****************************************/

//  van-Genuchten effective saturation as a function of single pressure, and its derivs wrt to that pressure
//
#ifndef RICHARDSSEFF1VG_H
#define RICHARDSSEFF1VG_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "RichardsSeff.h"
#include "RichardsSeffVG.h"

<<<<<<< HEAD
/**
 * Effective saturation as a function of porepressure
 * using the van Genuchten formula.  Note this is not a function
 * of capillary pressure: i use porepressure instead, so
 * seff = 1 for p >= 0.
 */
class RichardsSeff1VG : public RichardsSeff
{
public:
  static InputParameters validParams();

  RichardsSeff1VG(const InputParameters & parameters);

  /**
   * effective saturation as a function of porepressure
   * @param p porepressure in the element.  Note that (*p[0])[qp] is the porepressure at quadpoint
   * qp
   * @param qp the quad point to evaluate effective saturation at
   */
  Real seff(std::vector<const VariableValue *> p, unsigned int qp) const;

  /**
   * derivative of effective saturation as a function of porepressure
   * @param p porepressure in the element.  Note that (*p[0])[qp] is the porepressure at quadpoint
   * qp
   * @param qp the quad point to evaluate effective saturation at
   * @param result the derivtives will be placed in this array
   */
  void
  dseff(std::vector<const VariableValue *> p, unsigned int qp, std::vector<Real> & result) const;

  /**
   * second derivative of effective saturation as a function of porepressure
   * @param p porepressure in the element.  Note that (*p[0])[qp] is the porepressure at quadpoint
   * qp
   * @param qp the quad point to evaluate effective saturation at
   * @param result the derivtives will be placed in this array
   */
  void d2seff(std::vector<const VariableValue *> p,
              unsigned int qp,
              std::vector<std::vector<Real>> & result) const;

protected:
  /// van Genuchten alpha parameter
  Real _al;

  /// van Genuchten m parameter
  Real _m;
};
=======
class RichardsSeff1VG;


template<>
InputParameters validParams<RichardsSeff1VG>();

class RichardsSeff1VG : public RichardsSeff
{
 public:
  RichardsSeff1VG(const std::string & name, InputParameters parameters);

  Real seff(std::vector<VariableValue *> p, unsigned int qp) const;
  std::vector<Real> dseff(std::vector<VariableValue *> p, unsigned int qp) const;
  std::vector<std::vector<Real> > d2seff(std::vector<VariableValue *> p, unsigned int qp) const;

 protected:

  Real _al;
  Real _m;

};

#endif // RICHARDSSEFF1VG_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
