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
#ifndef INSTEMPERATURETIMEDERIVATIVE_H
#define INSTEMPERATURETIMEDERIVATIVE_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "TimeDerivative.h"

// Forward Declarations
<<<<<<< HEAD
=======
class INSTemperatureTimeDerivative;

template<>
InputParameters validParams<INSTemperatureTimeDerivative>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * This class computes the time derivative for the incompressible
 * Navier-Stokes momentum equation.  Could instead use CoefTimeDerivative
 * for this.
 */
class INSTemperatureTimeDerivative : public TimeDerivative
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  INSTemperatureTimeDerivative(const InputParameters & parameters);

  virtual ~INSTemperatureTimeDerivative() {}
=======
  INSTemperatureTimeDerivative(const std::string & name, InputParameters parameters);

  virtual ~INSTemperatureTimeDerivative(){}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);

  // Parameters
<<<<<<< HEAD
  const MaterialProperty<Real> & _rho;
  const MaterialProperty<Real> & _cp;
};
=======
  Real _rho;
  Real _cp;
};


#endif // INSTEMPERATURETIMEDERIVATIVE_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
