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

#ifndef RICHARDSSEFFPRIMEAUX_H
#define RICHARDSSEFFPRIMEAUX_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "AuxKernel.h"

#include "RichardsSeff.h"

<<<<<<< HEAD
// Forward Declarations

/**
 * Calculates derivative of effective saturation wrt a specified porepressure
 */
class RichardsSeffPrimeAux : public AuxKernel
{
public:
  static InputParameters validParams();

  RichardsSeffPrimeAux(const InputParameters & parameters);
=======
//Forward Declarations
class RichardsSeffPrimeAux;

template<>
InputParameters validParams<RichardsSeffPrimeAux>();

class RichardsSeffPrimeAux: public AuxKernel
{
public:
  RichardsSeffPrimeAux(const std::string & name, InputParameters parameters);
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
   * AuxKernel calculates d^2(Seff)/dP_wrt1
   * so wrt1 is the porepressure number to differentiate wrt to
   */
  int _wrt1;

  /**
   * the porepressure values (this will be length N
   * where N is the number of arguments that the _seff_UO requires)
   * Eg, for twophase simulations N=2
   */
  const std::vector<const VariableValue *> _pressure_vals;

  /// array of derivtives: This auxkernel returns _mat[_wrt1]
  std::vector<Real> _mat;
};
=======
  const RichardsSeff & _seff_UO;

  int _wrt1;

  std::vector<unsigned int> _pressure_vars;
  std::vector<VariableValue *> _pressure_vals;
};

#endif // RICHARDSSEFFPRIMEAUX_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
