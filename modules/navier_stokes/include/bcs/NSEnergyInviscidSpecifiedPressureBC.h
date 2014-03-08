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
#ifndef NSENERGYINVISCIDSPECIFIEDPRESSUREBC_H
#define NSENERGYINVISCIDSPECIFIEDPRESSUREBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "NSEnergyInviscidBC.h"

// Forward Declarations
<<<<<<< HEAD
=======
class NSEnergyInviscidSpecifiedPressureBC;

template<>
InputParameters validParams<NSEnergyInviscidSpecifiedPressureBC>();

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * The inviscid energy BC term with specified pressure.
 */
class NSEnergyInviscidSpecifiedPressureBC : public NSEnergyInviscidBC
{
<<<<<<< HEAD
public:
  static InputParameters validParams();

  NSEnergyInviscidSpecifiedPressureBC(const InputParameters & parameters);
=======

public:
  NSEnergyInviscidSpecifiedPressureBC(const std::string & name, InputParameters parameters);

  virtual ~NSEnergyInviscidSpecifiedPressureBC(){}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);

  // Required parameters
  Real _specified_pressure;

private:
  // Helper Jacobian function
<<<<<<< HEAD
  Real computeJacobianHelper(unsigned var_number);
};
=======
  Real compute_jacobian(unsigned var_number);
};

#endif // NSENERGYINVISCIDSPECIFIEDPRESSUREBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
