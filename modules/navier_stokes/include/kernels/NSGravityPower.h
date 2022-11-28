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

#include "Kernel.h"

// Forward Declarations
=======
#ifndef NSGRAVITYPOWER_H
#define NSGRAVITYPOWER_H

#include "Kernel.h"

//Forward Declarations
class NSGravityPower;

template<>
InputParameters validParams<NSGravityPower>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

class NSGravityPower : public Kernel
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  NSGravityPower(const InputParameters & parameters);
=======

  NSGravityPower(const std::string & name, InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  unsigned int _momentum_var;
<<<<<<< HEAD
  const VariableValue & _momentum;

  const Real _acceleration;
};
=======
  VariableValue & _momentum;

  Real _acceleration;
};

#endif
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
