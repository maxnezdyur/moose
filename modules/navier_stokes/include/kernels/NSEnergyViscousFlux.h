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
#ifndef NSENERGYVISCOUSFLUX_H
#define NSENERGYVISCOUSFLUX_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "NSKernel.h"
#include "NSViscStressTensorDerivs.h"

<<<<<<< HEAD
// Forward Declarations
=======

// Forward Declarations
class NSEnergyViscousFlux;

template<>
InputParameters validParams<NSEnergyViscousFlux>();

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * Viscous flux terms in energy equation.
 */
class NSEnergyViscousFlux : public NSKernel
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  NSEnergyViscousFlux(const InputParameters & parameters);
=======

  NSEnergyViscousFlux(const std::string & name, InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  // An object for computing viscous stress tensor derivatives.
  // Constructed via a reference to ourself
  NSViscStressTensorDerivs<NSEnergyViscousFlux> _vst_derivs;

  // Declare ourselves friend to the helper class.
  template <class U>
  friend class NSViscStressTensorDerivs;
};
<<<<<<< HEAD
=======

#endif //  NSENERGYVISCOUSFLUX_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
