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

#include "GeneralPostprocessor.h"

class RichardsSumQuantity;

/**
 * Extracts the value from RichardsSumQuantity userobject
=======
/*****************************************/
/* Written by andrew.wilkins@csiro.au    */
/* Please contact me if you make changes */
/*****************************************/

#ifndef RICHARDSPLOTQUANTITY_H
#define RICHARDSPLOTQUANTITY_H

#include "GeneralPostprocessor.h"

class RichardsPlotQuantity;
class RichardsSumQuantity;

template<>
InputParameters validParams<RichardsPlotQuantity>();

/**
 * Just extracts the value from RichardsTotalOutflowMass
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
 */
class RichardsPlotQuantity : public GeneralPostprocessor
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  RichardsPlotQuantity(const InputParameters & parameters);
=======
  RichardsPlotQuantity(const std::string & name, InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  virtual ~RichardsPlotQuantity();

  virtual void initialize();
  virtual void execute();
<<<<<<< HEAD

  /// returns the value of the RichardsSumQuantity
  virtual PostprocessorValue getValue();

protected:
  /// the RichardsSumQuantity userobject
  const RichardsSumQuantity & _total_mass;
};
=======
  virtual PostprocessorValue getValue();

protected:
  const RichardsSumQuantity & _total_mass;
};


#endif /* RICHARDSPLOTQUANTITY_H */
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
