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
#ifndef NSENERGYINVISCIDSPECIFIEDBC_H
#define NSENERGYINVISCIDSPECIFIEDBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "NSEnergyInviscidBC.h"

// Forward Declarations
<<<<<<< HEAD
=======
class NSEnergyInviscidSpecifiedBC;

template<>
InputParameters validParams<NSEnergyInviscidSpecifiedBC>();

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * The inviscid energy BC term with specified pressure.
 */
class NSEnergyInviscidSpecifiedBC : public NSEnergyInviscidBC
{
<<<<<<< HEAD
public:
  static InputParameters validParams();

  NSEnergyInviscidSpecifiedBC(const InputParameters & parameters);
=======

public:
  NSEnergyInviscidSpecifiedBC(const std::string & name, InputParameters parameters);

  virtual ~NSEnergyInviscidSpecifiedBC(){}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);

  // Required parameters
<<<<<<< HEAD
  const Real _specified_pressure;
  const Real _un;

private:
  // Helper Jacobian function
  Real computeJacobianHelper(unsigned var_number);
};
=======
  Real _specified_pressure;
  Real _un;

private:
  // Helper Jacobian function
  Real compute_jacobian(unsigned var_number);
};

#endif // NSENERGYINVISCIDSPECIFIEDPRESSUREBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
