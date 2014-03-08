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
#ifndef NSMOMENTUMCONVECTIVEWEAKSTAGNATIONBC_H
#define NSMOMENTUMCONVECTIVEWEAKSTAGNATIONBC_H

#include "NSWeakStagnationBC.h"

// Forward Declarations
class NSMomentumConvectiveWeakStagnationBC;

template<>
InputParameters validParams<NSMomentumConvectiveWeakStagnationBC>();

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * The convective part (sans pressure term) of the momentum equation
 * boundary integral evaluated at specified stagnation temperature,
 * stagnation pressure, and flow direction values.
 */
<<<<<<< HEAD
class NSMomentumConvectiveWeakStagnationBC : public NSWeakStagnationBaseBC
{
public:
  static InputParameters validParams();

  NSMomentumConvectiveWeakStagnationBC(const InputParameters & parameters);
=======
class NSMomentumConvectiveWeakStagnationBC : public NSWeakStagnationBC
{

public:
  NSMomentumConvectiveWeakStagnationBC(const std::string & name, InputParameters parameters);

  virtual ~NSMomentumConvectiveWeakStagnationBC(){}
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

#endif // NSMOMENTUMCONVECTIVEWEAKSTAGNATIONBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
