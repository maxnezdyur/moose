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
#ifndef INSPROJECTION_H
#define INSPROJECTION_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "Kernel.h"

// Forward Declarations
<<<<<<< HEAD
=======
class INSProjection;

template<>
InputParameters validParams<INSProjection>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * This class computes the "projection" part of the "split" method for
 * solving incompressible Navier-Stokes.  This is a time-varying equation
 * for u that is coupled to both the acceleration "a" and the pressue.
 *
 * Do not use, USE INSChorinCorrector and related classes instead.
 */
class INSProjection : public Kernel
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  INSProjection(const InputParameters & parameters);

  virtual ~INSProjection() {}
=======
  INSProjection(const std::string & name, InputParameters parameters);

  virtual ~INSProjection(){}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);

  // Coupled variables
<<<<<<< HEAD
  const VariableValue & _a1;
  const VariableValue & _a2;
  const VariableValue & _a3;

  // Gradients
  const VariableGradient & _grad_p;
=======
  VariableValue& _a1;
  VariableValue& _a2;
  VariableValue& _a3;

  // Gradients
  VariableGradient& _grad_p;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  // Variable numberings
  unsigned _a1_var_number;
  unsigned _a2_var_number;
  unsigned _a3_var_number;
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


#endif // INSPROJECTION_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
