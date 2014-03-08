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
#ifndef NSENERGYTHERMALFLUX_H
#define NSENERGYTHERMALFLUX_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "NSKernel.h"
#include "NSTemperatureDerivs.h"

// ForwardDeclarations
<<<<<<< HEAD
=======
class NSEnergyThermalFlux;

template<>
InputParameters validParams<NSEnergyThermalFlux>();

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * This class is responsible for computing residuals and Jacobian
 * terms for the k * grad(T) * grad(phi) term in the Navier-Stokes
 * energy equation.
 */
class NSEnergyThermalFlux : public NSKernel
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  NSEnergyThermalFlux(const InputParameters & parameters);
=======

  NSEnergyThermalFlux(const std::string & name, InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  // Gradients
<<<<<<< HEAD
  const VariableGradient & _grad_temp;

  // Material properties
  const MaterialProperty<Real> & _thermal_conductivity;
=======
  VariableGradient& _grad_temp;

  // Material properties
  MaterialProperty<Real> &_thermal_conductivity;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  // A helper object for computing temperature gradient and Hessians.
  // Constructed via a reference to ourself so we can access all of our data.
  NSTemperatureDerivs<NSEnergyThermalFlux> _temp_derivs;

  // Declare ourselves a friend to the helper class
  template <class U>
  friend class NSTemperatureDerivs;

private:
<<<<<<< HEAD
=======

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  // Computes the Jacobian value (on or off-diagonal) for
  // var_number, which has been mapped to
  // 0 = rho
  // 1 = rho*u
  // 2 = rho*v
  // 3 = rho*w
  // 4 = rho*E
<<<<<<< HEAD
  Real computeJacobianHelper_value(unsigned var_number);
=======
  Real compute_jacobian_value(unsigned var_number);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  // Single vector to refer to all gradients.  We have to store
  // pointers since you can't have a vector<Foo&>.  Initialized in
  // the ctor.
<<<<<<< HEAD
  std::vector<const VariableGradient *> _gradU;
};
=======
  std::vector<VariableGradient*> _gradU;
};


#endif // ENERGYTHERMALFLUX_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
