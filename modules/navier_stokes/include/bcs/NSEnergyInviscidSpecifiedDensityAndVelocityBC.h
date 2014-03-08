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
#ifndef NSENERGYINVISCIDSPECIFIEDDENSITYANDVELOCITYBC_H
#define NSENERGYINVISCIDSPECIFIEDDENSITYANDVELOCITYBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "NSEnergyInviscidBC.h"

// Forward Declarations
<<<<<<< HEAD
=======
class NSEnergyInviscidSpecifiedDensityAndVelocityBC;

template<>
InputParameters validParams<NSEnergyInviscidSpecifiedDensityAndVelocityBC>();

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * The inviscid energy BC term with specified density and velocity components.
 * This was experimental code and did not really work out, do not use!
 */
class NSEnergyInviscidSpecifiedDensityAndVelocityBC : public NSEnergyInviscidBC
{
<<<<<<< HEAD
public:
  static InputParameters validParams();

  NSEnergyInviscidSpecifiedDensityAndVelocityBC(const InputParameters & parameters);

  virtual ~NSEnergyInviscidSpecifiedDensityAndVelocityBC() {}
=======

public:
  NSEnergyInviscidSpecifiedDensityAndVelocityBC(const std::string & name, InputParameters parameters);

  virtual ~NSEnergyInviscidSpecifiedDensityAndVelocityBC(){}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);

  // Aux Variables
<<<<<<< HEAD
  const VariableValue & _pressure;
=======
  VariableValue& _pressure;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  // Required parameters
  Real _specified_density;

  Real _specified_u; // FIXME: Read these as a single RealVectorValue
  Real _specified_v; // FIXME: Read these as a single RealVectorValue
  Real _specified_w; // FIXME: Read these as a single RealVectorValue
<<<<<<< HEAD
};
=======

private:
  // Helper Jacobian function
  // Real compute_jacobian(unsigned var_number);
};

#endif // NSENERGYINVISCIDSPECIFIEDDENSITYANDVELOCITYBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
