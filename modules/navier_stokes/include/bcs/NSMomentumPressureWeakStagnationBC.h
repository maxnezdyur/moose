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
#ifndef NSMOMENTUMPRESSUREWEAKSTAGNATIONBC_H
#define NSMOMENTUMPRESSUREWEAKSTAGNATIONBC_H

#include "NSWeakStagnationBC.h"

// Forward Declarations
class NSMomentumPressureWeakStagnationBC;

template<>
InputParameters validParams<NSMomentumPressureWeakStagnationBC>();

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * This class implements the pressure term of the momentum
 * equation boundary integral for use in weak stagnation
 * boundary conditions.
 */
<<<<<<< HEAD
class NSMomentumPressureWeakStagnationBC : public NSWeakStagnationBaseBC
{
public:
  static InputParameters validParams();

  NSMomentumPressureWeakStagnationBC(const InputParameters & parameters);
=======
class NSMomentumPressureWeakStagnationBC : public NSWeakStagnationBC
{

public:
  NSMomentumPressureWeakStagnationBC(const std::string & name, InputParameters parameters);

  virtual ~NSMomentumPressureWeakStagnationBC(){}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);

  // Required parameters
<<<<<<< HEAD
  const unsigned int _component;
};
=======
  unsigned _component;
};

#endif // NSMOMENTUMPRESSUREWEAKSTAGNATIONBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
