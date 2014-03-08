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
#ifndef NSENERGYINVISCIDSPECIFIEDNORMALFLOWBC_H
#define NSENERGYINVISCIDSPECIFIEDNORMALFLOWBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "NSEnergyInviscidBC.h"

// Forward Declarations
<<<<<<< HEAD
=======
class NSEnergyInviscidSpecifiedNormalFlowBC;

template<>
InputParameters validParams<NSEnergyInviscidSpecifiedNormalFlowBC>();

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * The inviscid energy BC term with specified normal flow.
 */
class NSEnergyInviscidSpecifiedNormalFlowBC : public NSEnergyInviscidBC
{
<<<<<<< HEAD
public:
  static InputParameters validParams();

  NSEnergyInviscidSpecifiedNormalFlowBC(const InputParameters & parameters);
=======

public:
  NSEnergyInviscidSpecifiedNormalFlowBC(const std::string & name, InputParameters parameters);

  virtual ~NSEnergyInviscidSpecifiedNormalFlowBC(){}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);

  // Aux Variables
<<<<<<< HEAD
  const VariableValue & _pressure;

  // Required parameters
  const Real _un;

private:
  Real computeJacobianHelper(unsigned var_number);
};
=======
  VariableValue& _pressure;

  // Required parameters
  Real _un;

private:
  // Helper Jacobian function
  Real compute_jacobian(unsigned var_number);
};

#endif // NSENERGYINVISCIDSPECIFIEDNORMALFLOWBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
