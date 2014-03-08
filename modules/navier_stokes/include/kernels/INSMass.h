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

#include "INSBase.h"

// Forward Declarations
=======
#ifndef INSMASS_H
#define INSMASS_H

#include "Kernel.h"

// Forward Declarations
class INSMass;

template<>
InputParameters validParams<INSMass>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * This class computes the mass equation residual and Jacobian
 * contributions for the incompressible Navier-Stokes momentum
 * equation.
 */
<<<<<<< HEAD
class INSMass : public INSBase
{
public:
  static InputParameters validParams();

  INSMass(const InputParameters & parameters);

  virtual ~INSMass() {}
=======
class INSMass : public Kernel
{
public:
  INSMass(const std::string & name, InputParameters parameters);

  virtual ~INSMass(){}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);

<<<<<<< HEAD
  virtual Real computeQpPGResidual();
  virtual Real computeQpPGJacobian();
  virtual Real computeQpPGOffDiagJacobian(unsigned comp);

  bool _pspg;
  const Function & _x_ffn;
  const Function & _y_ffn;
  const Function & _z_ffn;
};
=======
  // Coupled Gradients
  VariableGradient& _grad_u_vel;
  VariableGradient& _grad_v_vel;
  VariableGradient& _grad_w_vel;

  // Variable numberings
  unsigned _u_vel_var_number;
  unsigned _v_vel_var_number;
  unsigned _w_vel_var_number;
  unsigned _p_var_number;
};


#endif // INSMASS_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
