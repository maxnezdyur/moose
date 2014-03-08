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

#ifndef RICHARDSSEFFAUX_H
#define RICHARDSSEFFAUX_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "AuxKernel.h"

#include "RichardsSeff.h"

<<<<<<< HEAD
// Forward Declarations

/**
 * Calculates effective saturation for a specified variable
 */
class RichardsSeffAux : public AuxKernel
{
public:
  static InputParameters validParams();

  RichardsSeffAux(const InputParameters & parameters);
=======
//Forward Declarations
class RichardsSeffAux;

template<>
InputParameters validParams<RichardsSeffAux>();

class RichardsSeffAux: public AuxKernel
{
public:
  RichardsSeffAux(const std::string & name, InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeValue();

<<<<<<< HEAD
  /**
   * The user object that defines effective saturation
   * as function of porepressure (or porepressures in the
   * multiphase situation)
   */
  const RichardsSeff & _seff_UO;

  /**
   * the porepressure values (this will be length N
   * where N is the number of arguments that the _seff_UO requires)
   * Eg, for twophase simulations N=2
   */
  const std::vector<const VariableValue *> _pressure_vals;
};
=======
  const RichardsSeff & _seff_UO;

  std::vector<unsigned int> _pressure_vars;
  std::vector<VariableValue *> _pressure_vals;
};

#endif // RICHARDSSEFFAUX_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
