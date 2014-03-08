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
#ifndef INSCOURANT_H
#define INSCOURANT_H

#include "AuxKernel.h"

//Forward Declarations
class INSCourant;

template<>
InputParameters validParams<INSCourant>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * Computes h_min / |u|
 */
class INSCourant : public AuxKernel
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  INSCourant(const InputParameters & parameters);
=======
  INSCourant(const std::string & name, InputParameters parameters);

  virtual ~INSCourant() {}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeValue();

  // Velocity
<<<<<<< HEAD
  const VariableValue & _u_vel;
  const VariableValue & _v_vel;
  const VariableValue & _w_vel;
};
=======
  VariableValue& _u_vel;
  VariableValue& _v_vel;
  VariableValue& _w_vel;
};

#endif //VELOCITYAUX_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
