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

#include "AuxKernel.h"

// Forward Declarations
=======
#ifndef NSVELOCITYAUX_H
#define NSVELOCITYAUX_H

#include "AuxKernel.h"

//Forward Declarations
class NSVelocityAux;

template<>
InputParameters validParams<NSVelocityAux>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * Velocity auxiliary value
 */
class NSVelocityAux : public AuxKernel
{
public:
<<<<<<< HEAD
=======

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
<<<<<<< HEAD
  static InputParameters validParams();

  NSVelocityAux(const InputParameters & parameters);
=======
  NSVelocityAux(const std::string & name, InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  virtual ~NSVelocityAux() {}

protected:
  virtual Real computeValue();

<<<<<<< HEAD
  const VariableValue & _rho;
  const VariableValue & _momentum;
};
=======
  VariableValue & _rho;
  VariableValue & _momentum;

};

#endif //VELOCITYAUX_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
