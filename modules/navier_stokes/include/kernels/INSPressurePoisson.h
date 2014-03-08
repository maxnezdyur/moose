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
#ifndef INSPRESSUREPOISSON_H
#define INSPRESSUREPOISSON_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "Kernel.h"

// Forward Declarations
<<<<<<< HEAD
=======
class INSPressurePoisson;

template<>
InputParameters validParams<INSPressurePoisson>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * This class computes the pressure Poisson solve which is part of
 * the "split" scheme used for solving the incompressible Navier-Stokes
 * equations.
 *
 * Do not use, USE INSChorinPressurePoisson and related classes instead.
 */
class INSPressurePoisson : public Kernel
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  INSPressurePoisson(const InputParameters & parameters);

  virtual ~INSPressurePoisson() {}
=======
  INSPressurePoisson(const std::string & name, InputParameters parameters);

  virtual ~INSPressurePoisson(){}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);

  // Gradients of the accleration vector, 'a'
<<<<<<< HEAD
  const VariableGradient & _grad_a1;
  const VariableGradient & _grad_a2;
  const VariableGradient & _grad_a3;
=======
  VariableGradient& _grad_a1;
  VariableGradient& _grad_a2;
  VariableGradient& _grad_a3;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  // Variable numberings
  unsigned _a1_var_number;
  unsigned _a2_var_number;
  unsigned _a3_var_number;

  // Material properties
<<<<<<< HEAD
  const MaterialProperty<Real> & _rho;
};
=======
  Real _rho;
};


#endif // INSPRESSUREPOISSON_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
