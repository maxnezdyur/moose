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

#ifndef RICHARDSRELPERMPRIMEPRIMEAUX_H
#define RICHARDSRELPERMPRIMEPRIMEAUX_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "AuxKernel.h"

#include "RichardsRelPerm.h"

<<<<<<< HEAD
// Forward Declarations

/**
 * Relative Permeability as a function of effective saturation
 */
class RichardsRelPermPrimePrimeAux : public AuxKernel
{
public:
  static InputParameters validParams();

  RichardsRelPermPrimePrimeAux(const InputParameters & parameters);
=======
//Forward Declarations
class RichardsRelPermPrimePrimeAux;

template<>
InputParameters validParams<RichardsRelPermPrimePrimeAux>();

class RichardsRelPermPrimePrimeAux: public AuxKernel
{
public:
  RichardsRelPermPrimePrimeAux(const std::string & name, InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeValue();

<<<<<<< HEAD
  /// effective saturation
  const VariableValue & _seff_var;

  /// userobject that defines relative permeability function
  const RichardsRelPerm & _relperm_UO;
};
=======
  VariableValue & _seff_var;
  const RichardsRelPerm & _relperm_UO;
};

#endif // RICHARDSRELPERMPRIMEPRIMEAUX_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
