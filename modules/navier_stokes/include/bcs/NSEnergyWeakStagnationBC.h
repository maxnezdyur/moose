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

#include "NSWeakStagnationBaseBC.h"

// Forward Declarations
=======
#ifndef NSENERGYWEAKSTAGNATIONBC_H
#define NSENERGYWEAKSTAGNATIONBC_H

#include "NSWeakStagnationBC.h"

// Forward Declarations
class NSEnergyWeakStagnationBC;

template<>
InputParameters validParams<NSEnergyWeakStagnationBC>();

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * The inviscid energy BC term with specified normal flow.
 */
<<<<<<< HEAD
class NSEnergyWeakStagnationBC : public NSWeakStagnationBaseBC
{
public:
  static InputParameters validParams();

  NSEnergyWeakStagnationBC(const InputParameters & parameters);
=======
class NSEnergyWeakStagnationBC : public NSWeakStagnationBC
{

public:
  NSEnergyWeakStagnationBC(const std::string & name, InputParameters parameters);

  virtual ~NSEnergyWeakStagnationBC(){}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);
};
<<<<<<< HEAD
=======

#endif // NSENERGYWEAKSTAGNATIONBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
