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
#ifndef NSIMPOSEDVELOCITYBC_H
#define NSIMPOSEDVELOCITYBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

// The base class definition (part of MOOSE)
#include "NodalBC.h"

<<<<<<< HEAD
// Forward Declarations

// Specialization required of all user-level Moose objects
=======
//Forward Declarations
class NSImposedVelocityBC;


// Specialization required of all user-level Moose objects
template<>
InputParameters validParams<NSImposedVelocityBC>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

class NSImposedVelocityBC : public NodalBC
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  NSImposedVelocityBC(const InputParameters & parameters);

protected:
  // NodalBC's can (currently) only override the computeQpResidual function,
=======
  // Constructor
  NSImposedVelocityBC(const std::string & name, InputParameters parameters);

  // Destructor, better be virtual
  virtual ~NSImposedVelocityBC(){}

protected:

  // NodalBC's can (currently) only specialize the computeQpResidual function,
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  // the computeQpJacobian() function automatically assembles a "1" onto the main
  // diagonal for this DoF.
  virtual Real computeQpResidual();

  // We need the density, since we are actually setting essential values of
  // *momentum* not essential values of velocity.
<<<<<<< HEAD
  const VariableValue & _rho;
=======
  VariableValue & _rho;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  // The desired value for the velocity component
  Real _desired_velocity;
};
<<<<<<< HEAD
=======


#endif // NSIMPOSEDVELOCITYBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
