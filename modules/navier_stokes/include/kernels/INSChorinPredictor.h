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
#ifndef INSCHORINPREDICTOR_H
#define INSCHORINPREDICTOR_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "Kernel.h"

// Forward Declarations
<<<<<<< HEAD
=======
class INSChorinPredictor;

template<>
InputParameters validParams<INSChorinPredictor>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * This class computes the "Chorin" Predictor equation in fully-discrete
 * (both time and space) form.
 */
class INSChorinPredictor : public Kernel
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  INSChorinPredictor(const InputParameters & parameters);

  virtual ~INSChorinPredictor() {}
=======
  INSChorinPredictor(const std::string & name, InputParameters parameters);

  virtual ~INSChorinPredictor(){}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);

  // Velocity
<<<<<<< HEAD
  const VariableValue & _u_vel;
  const VariableValue & _v_vel;
  const VariableValue & _w_vel;

  // Old Velocity
  const VariableValue & _u_vel_old;
  const VariableValue & _v_vel_old;
  const VariableValue & _w_vel_old;

  // Star Velocity
  const VariableValue & _u_vel_star;
  const VariableValue & _v_vel_star;
  const VariableValue & _w_vel_star;

  // Velocity Gradients
  const VariableGradient & _grad_u_vel;
  const VariableGradient & _grad_v_vel;
  const VariableGradient & _grad_w_vel;

  // Old Velocity Gradients
  const VariableGradient & _grad_u_vel_old;
  const VariableGradient & _grad_v_vel_old;
  const VariableGradient & _grad_w_vel_old;

  // Star Velocity Gradients
  const VariableGradient & _grad_u_vel_star;
  const VariableGradient & _grad_v_vel_star;
  const VariableGradient & _grad_w_vel_star;
=======
  VariableValue& _u_vel;
  VariableValue& _v_vel;
  VariableValue& _w_vel;

  // Old Velocity
  VariableValue& _u_vel_old;
  VariableValue& _v_vel_old;
  VariableValue& _w_vel_old;

  // Star Velocity
  VariableValue& _u_vel_star;
  VariableValue& _v_vel_star;
  VariableValue& _w_vel_star;

  // Velocity Gradients
  VariableGradient& _grad_u_vel;
  VariableGradient& _grad_v_vel;
  VariableGradient& _grad_w_vel;

  // Old Velocity Gradients
  VariableGradient& _grad_u_vel_old;
  VariableGradient& _grad_v_vel_old;
  VariableGradient& _grad_w_vel_old;

  // Star Velocity Gradients
  VariableGradient& _grad_u_vel_star;
  VariableGradient& _grad_v_vel_star;
  VariableGradient& _grad_w_vel_star;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  // Variable numberings
  unsigned _u_vel_var_number;
  unsigned _v_vel_var_number;
  unsigned _w_vel_var_number;

  // Star velocity numbers
  unsigned _u_vel_star_var_number;
  unsigned _v_vel_star_var_number;
  unsigned _w_vel_star_var_number;

<<<<<<< HEAD
  // Parameters
  unsigned _component;

  // An enumeration defining which velocity vector is used on the rhs
  // of the Chorin predictor.
=======
  // Material properties
  Real _mu;
  Real _rho;

  // Parameters
  unsigned _component;

  // This is the string that's actually read in from file and used to set the
  // MooseEnum, below.  The options are:
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  // OLD  - Use velocity from the previous timestep, leads to explicit method
  // NEW  - Use velocity from current timestep, this may not be an actual method
  // STAR - Use the "star" velocity.  According to Donea's book, this is the
  //        right way to get an implicit method...
<<<<<<< HEAD
=======
  std::string _predictor_type;

  // An enumeration defining which velocity vector is used on the rhs
  // of the Chorin predictor.
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  MooseEnum _predictor_enum;

  // A C++ enumeration corresponding to the predictor enumeration.
  enum PredictorType
  {
    OLD = 0,
    NEW = 1,
    STAR = 2
  };
<<<<<<< HEAD

  // Material properties
  const MaterialProperty<Real> & _mu;
  const MaterialProperty<Real> & _rho;
};
=======
};


#endif // INSCHORINPREDICTOR_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
