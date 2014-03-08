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
#ifndef NSTEMPERATUREL2_H
#define NSTEMPERATUREL2_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "Kernel.h"
#include "Material.h"

<<<<<<< HEAD
// Forward Declarations
=======
//Forward Declarations
class NSTemperatureL2;

template<>
InputParameters validParams<NSTemperatureL2>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * This class was originally used to solve for the temperature
 * using an L2-projection.  I'm not sure if anything is still
 * using this.  If not, it could probably be removed.
 */
class NSTemperatureL2 : public Kernel
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  NSTemperatureL2(const InputParameters & parameters);
=======

  NSTemperatureL2(const std::string & name, InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

<<<<<<< HEAD
  unsigned int _rho_var;
  const VariableValue & _rho;

  unsigned int _rhoe_var;
  const VariableValue & _rhoe;

  unsigned int _u_vel_var;
  const VariableValue & _u_vel;

  unsigned int _v_vel_var;
  const VariableValue & _v_vel;

  unsigned int _w_vel_var;
  const VariableValue & _w_vel;

  const MaterialProperty<Real> & _c_v;
};
=======
  unsigned int _p_var;
  VariableValue & _p;

  unsigned int _pe_var;
  VariableValue & _pe;

  unsigned int _u_vel_var;
  VariableValue & _u_vel;

  unsigned int _v_vel_var;
  VariableValue & _v_vel;

  unsigned int _w_vel_var;
  VariableValue & _w_vel;

  MaterialProperty<Real> & _c_v;
};

#endif
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
