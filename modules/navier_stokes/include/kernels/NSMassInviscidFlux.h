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

#include "NSKernel.h"

// Forward Declarations
=======
#ifndef NSMASSINVISCIDFLUX_H
#define NSMASSINVISCIDFLUX_H

#include "NSKernel.h"


// Forward Declarations
class NSMassInviscidFlux;

template<>
InputParameters validParams<NSMassInviscidFlux>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

class NSMassInviscidFlux : public NSKernel
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  NSMassInviscidFlux(const InputParameters & parameters);
=======

  NSMassInviscidFlux(const std::string & name, InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);
};
<<<<<<< HEAD
=======

#endif
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
