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
#ifndef NSMASSWEAKSTAGNATIONBC_H
#define NSMASSWEAKSTAGNATIONBC_H

#include "NSWeakStagnationBC.h"

// Forward Declarations
class NSMassWeakStagnationBC;

template<>
InputParameters validParams<NSMassWeakStagnationBC>();

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * The inviscid energy BC term with specified normal flow.
 */
<<<<<<< HEAD
class NSMassWeakStagnationBC : public NSWeakStagnationBaseBC
{
public:
  static InputParameters validParams();

  NSMassWeakStagnationBC(const InputParameters & parameters);
=======
class NSMassWeakStagnationBC : public NSWeakStagnationBC
{

public:
  NSMassWeakStagnationBC(const std::string & name, InputParameters parameters);

  virtual ~NSMassWeakStagnationBC(){}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);
};
<<<<<<< HEAD
=======

#endif // NSMASSWEAKSTAGNATIONBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
