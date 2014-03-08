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

#include "ElementIntegralVariablePostprocessor.h"
#include "RichardsVarNames.h"

// Forward Declarations
=======
/*****************************************/
/* Written by andrew.wilkins@csiro.au    */
/* Please contact me if you make changes */
/*****************************************/

#ifndef RICHARDSMASS_H
#define RICHARDSMASS_H

#include "ElementIntegralVariablePostprocessor.h"
#include "RichardsPorepressureNames.h"

//Forward Declarations
class RichardsMass;

template<>
InputParameters validParams<RichardsMass>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * This postprocessor computes the fluid mass by integrating the density over the volume
 *
 */
<<<<<<< HEAD
class RichardsMass : public ElementIntegralVariablePostprocessor
{
public:
  static InputParameters validParams();

  RichardsMass(const InputParameters & parameters);
=======
class RichardsMass: public ElementIntegralVariablePostprocessor
{
public:
  RichardsMass(const std::string & name, InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpIntegral();

<<<<<<< HEAD
  /// userobject that holds Richards variable names
  const RichardsVarNames & _richards_name_UO;

  /// Richards variable number that we want the mass for
  unsigned int _pvar;

  /// Mass, or vector of masses in multicomponent situation
  const MaterialProperty<std::vector<Real>> & _mass;
};
=======
  const RichardsPorepressureNames & _pp_name_UO;
  unsigned int _pvar;

  MaterialProperty<Real> &_porosity;
  MaterialProperty<std::vector<Real> > &_sat;
  MaterialProperty<std::vector<Real> > &_density;
};

#endif
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
