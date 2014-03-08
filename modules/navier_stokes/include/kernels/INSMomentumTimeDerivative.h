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
#ifndef INSMOMENTUMTIMEDERIVATIVE_H
#define INSMOMENTUMTIMEDERIVATIVE_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "TimeDerivative.h"

// Forward Declarations
<<<<<<< HEAD
=======
class INSMomentumTimeDerivative;

template<>
InputParameters validParams<INSMomentumTimeDerivative>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * This class computes the time derivative for the incompressible
 * Navier-Stokes momentum equation.  Could instead use CoefTimeDerivative
 * for this.
 */
class INSMomentumTimeDerivative : public TimeDerivative
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  INSMomentumTimeDerivative(const InputParameters & parameters);

  virtual ~INSMomentumTimeDerivative() {}
=======
  INSMomentumTimeDerivative(const std::string & name, InputParameters parameters);

  virtual ~INSMomentumTimeDerivative(){}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);

  // Parameters
<<<<<<< HEAD
  const MaterialProperty<Real> & _rho;
};
=======
  Real _rho;
};


#endif // INSMOMENTUMTIMEDERIVATIVE_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
