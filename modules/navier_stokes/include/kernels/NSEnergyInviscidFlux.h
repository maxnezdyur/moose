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
#ifndef NSENERGYINVISCIDFLUX_H
#define NSENERGYINVISCIDFLUX_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "NSKernel.h"

// Forward Declarations
<<<<<<< HEAD
=======
class NSEnergyInviscidFlux;

template<>
InputParameters validParams<NSEnergyInviscidFlux>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

class NSEnergyInviscidFlux : public NSKernel
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  NSEnergyInviscidFlux(const InputParameters & parameters);
=======

  NSEnergyInviscidFlux(const std::string & name, InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  // Coupled variables
<<<<<<< HEAD
  const VariableValue & _specific_total_enthalpy;
};
=======
  VariableValue & _enthalpy;
};

#endif
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
