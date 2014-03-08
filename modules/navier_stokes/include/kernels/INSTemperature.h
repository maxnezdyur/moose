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
#ifndef INSTEMPERATURE_H
#define INSTEMPERATURE_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "Kernel.h"

// Forward Declarations
<<<<<<< HEAD
=======
class INSTemperature;

template<>
InputParameters validParams<INSTemperature>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * This class computes the residual and Jacobian contributions for the
 * incompressible Navier-Stokes temperature (energy) equation.
 */
class INSTemperature : public Kernel
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  INSTemperature(const InputParameters & parameters);

  virtual ~INSTemperature() {}
=======
  INSTemperature(const std::string & name, InputParameters parameters);

  virtual ~INSTemperature(){}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);

  // Coupled variables
<<<<<<< HEAD
  const VariableValue & _u_vel;
  const VariableValue & _v_vel;
  const VariableValue & _w_vel;
=======
  VariableValue& _u_vel;
  VariableValue& _v_vel;
  VariableValue& _w_vel;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  // Variable numberings
  unsigned _u_vel_var_number;
  unsigned _v_vel_var_number;
  unsigned _w_vel_var_number;

  // Required parameters
<<<<<<< HEAD
  const MaterialProperty<Real> & _rho;
  const MaterialProperty<Real> & _k;
  const MaterialProperty<Real> & _cp;
};
=======
  Real _rho;
  Real _k;
  Real _cp;
};


#endif // INSTEMPERATURE_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
