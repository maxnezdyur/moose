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
#ifndef INSCHORINCORRECTOR_H
#define INSCHORINCORRECTOR_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "Kernel.h"

// Forward Declarations
<<<<<<< HEAD
=======
class INSChorinCorrector;

template<>
InputParameters validParams<INSChorinCorrector>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * This class computes the "Chorin" Corrector equation in fully-discrete
 * (both time and space) form.
 */
class INSChorinCorrector : public Kernel
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  INSChorinCorrector(const InputParameters & parameters);

  virtual ~INSChorinCorrector() {}
=======
  INSChorinCorrector(const std::string & name, InputParameters parameters);

  virtual ~INSChorinCorrector(){}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);

  // "Star" velocity components
<<<<<<< HEAD
  const VariableValue & _u_vel_star;
  const VariableValue & _v_vel_star;
  const VariableValue & _w_vel_star;

  // Pressure gradients
  const VariableGradient & _grad_p;
=======
  VariableValue& _u_vel_star;
  VariableValue& _v_vel_star;
  VariableValue& _w_vel_star;

  // Pressure gradients
  VariableGradient& _grad_p;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  // Variable numberings
  unsigned _u_vel_star_var_number;
  unsigned _v_vel_star_var_number;
  unsigned _w_vel_star_var_number;
  unsigned _p_var_number;

<<<<<<< HEAD
  // Parameters
  unsigned _component;

  // Material properties
  const MaterialProperty<Real> & _rho;
};
=======
  // Material properties
  Real _rho;

  // Parameters
  unsigned _component;
};


#endif // INSCHORINCORRECTOR_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
