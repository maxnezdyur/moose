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
#ifndef INSSPLITMOMENTUM_H
#define INSSPLITMOMENTUM_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "Kernel.h"

// Forward Declarations
<<<<<<< HEAD
=======
class INSSplitMomentum;

template<>
InputParameters validParams<INSSplitMomentum>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * This class computes the "split" momentum equation residual.  In the
 * split method, this is a time-independent vector equation for "a",
 * an intermediate "acceleration" vector.  The pressure is not coupled
 * directly to momentum in the split method.  Note: this equation is
 * divided through by the density, so "nu" appears rather than "mu", for
 * instance.
 *
 * Do not use, USE INSChorinPredictor and related classes instead.
 */
class INSSplitMomentum : public Kernel
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  INSSplitMomentum(const InputParameters & parameters);

  virtual ~INSSplitMomentum() {}
=======
  INSSplitMomentum(const std::string & name, InputParameters parameters);

  virtual ~INSSplitMomentum(){}
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

  // Acceleration vector components
  const VariableValue & _a1;
  const VariableValue & _a2;
  const VariableValue & _a3;

  // Gradients
  const VariableGradient & _grad_u_vel;
  const VariableGradient & _grad_v_vel;
  const VariableGradient & _grad_w_vel;
=======
  VariableValue& _u_vel;
  VariableValue& _v_vel;
  VariableValue& _w_vel;

  // Acceleration vector components
  VariableValue& _a1;
  VariableValue& _a2;
  VariableValue& _a3;

  // Gradients
  VariableGradient& _grad_u_vel;
  VariableGradient& _grad_v_vel;
  VariableGradient& _grad_w_vel;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  // Variable numberings
  unsigned _u_vel_var_number;
  unsigned _v_vel_var_number;
  unsigned _w_vel_var_number;

  unsigned _a1_var_number;
  unsigned _a2_var_number;
  unsigned _a3_var_number;

<<<<<<< HEAD
  // Parameters
  RealVectorValue _gravity;
  unsigned _component;

  // Material properties
  const MaterialProperty<Real> & _mu;
  const MaterialProperty<Real> & _rho;
};
=======
  // Material properties
  Real _mu;
  Real _rho;
  RealVectorValue _gravity;

  // Parameters
  unsigned _component;
};


#endif // INSSPLITMOMENTUM_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
