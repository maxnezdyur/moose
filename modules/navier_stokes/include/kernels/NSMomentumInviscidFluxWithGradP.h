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
#ifndef NSMOMENTUMINVISCIDFLUXWITHGRADP_H
#define NSMOMENTUMINVISCIDFLUXWITHGRADP_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "NSKernel.h"
#include "NSPressureDerivs.h"

// ForwardDeclarations
<<<<<<< HEAD
=======
class NSMomentumInviscidFluxWithGradP;

template<>
InputParameters validParams<NSMomentumInviscidFluxWithGradP>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

class NSMomentumInviscidFluxWithGradP : public NSKernel
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  NSMomentumInviscidFluxWithGradP(const InputParameters & parameters);
=======

  NSMomentumInviscidFluxWithGradP(const std::string & name, InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  // Coupled gradients
<<<<<<< HEAD
  const VariableGradient & _grad_p;

  // Parameters
  const unsigned int _component;
=======
  VariableGradient& _grad_p;

  // Parameters
  int _component;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

private:
  // Computes the Jacobian contribution due to the pressure term,
  // by summing over the appropriate Hessian row.
<<<<<<< HEAD
  Real pressureQpJacobianHelper(unsigned var_number);
=======
  Real compute_pressure_jacobian_value(unsigned var_number);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  // Single vector to refer to all gradients.  We have to store
  // pointers since you can't have a vector<Foo&>.  Initialized in
  // the ctor.
<<<<<<< HEAD
  std::vector<const VariableGradient *> _gradU;
=======
  std::vector<VariableGradient*> _gradU;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  // An object for computing pressure derivatives.
  // Constructed via a reference to ourself
  NSPressureDerivs<NSMomentumInviscidFluxWithGradP> _pressure_derivs;

  // Declare ourselves friend to the helper class.
  template <class U>
  friend class NSPressureDerivs;
};
<<<<<<< HEAD
=======

#endif
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
