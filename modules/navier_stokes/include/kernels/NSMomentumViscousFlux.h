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
#ifndef NSMOMENTUMVISCOUSFLUX_H
#define NSMOMENTUMVISCOUSFLUX_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "NSKernel.h"
#include "NSViscStressTensorDerivs.h"

<<<<<<< HEAD
// ForwardDeclarations
=======

// ForwardDeclarations
class NSMomentumViscousFlux;

template<>
InputParameters validParams<NSMomentumViscousFlux>();

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * Derived instance of the NSViscousFluxBase class
 * for the momentum equations.
 */
class NSMomentumViscousFlux : public NSKernel
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  NSMomentumViscousFlux(const InputParameters & parameters);
=======

  NSMomentumViscousFlux(const std::string & name, InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  // Required parameter
<<<<<<< HEAD
  const unsigned int _component;
=======
  unsigned _component;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  // An object for computing viscous stress tensor derivatives.
  // Constructed via a reference to ourself
  NSViscStressTensorDerivs<NSMomentumViscousFlux> _vst_derivs;

  // Declare ourselves friend to the helper class.
  template <class U>
  friend class NSViscStressTensorDerivs;
};
<<<<<<< HEAD
=======

#endif //  NSMOMENTUMVISCOUSFLUX_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
