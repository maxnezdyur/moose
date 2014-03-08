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

#ifndef RICHARDSDENSITYAUX_H
#define RICHARDSDENSITYAUX_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "AuxKernel.h"

#include "RichardsDensity.h"

<<<<<<< HEAD
// Forward Declarations

/**
 * Fluid density as a function of porepressure
 */
class RichardsDensityAux : public AuxKernel
{
public:
  static InputParameters validParams();

  RichardsDensityAux(const InputParameters & parameters);
=======
//Forward Declarations
class RichardsDensityAux;

template<>
InputParameters validParams<RichardsDensityAux>();

class RichardsDensityAux: public AuxKernel
{
public:
  RichardsDensityAux(const std::string & name, InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeValue();

<<<<<<< HEAD
  /// porepressure
  const VariableValue & _pressure_var;

  /// userobject that defines density as a fcn of porepressure
  const RichardsDensity & _density_UO;
};
=======
  VariableValue & _pressure_var;
  const RichardsDensity & _density_UO;
};

#endif // RICHARDSDENSITYAUX_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
