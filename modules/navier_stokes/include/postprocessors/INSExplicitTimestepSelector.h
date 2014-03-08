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

#include "ElementPostprocessor.h"

=======
#ifndef INSEXPLICITTIMESTEPSELECTOR_H
#define INSEXPLICITTIMESTEPSELECTOR_H

#include "ElementPostprocessor.h"

class INSExplicitTimestepSelector;

template<>
InputParameters validParams<INSExplicitTimestepSelector>();

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
/**
 * Postprocessor that computes the minimum value of h_min/|u|,
 * where |u| is coupled in as an aux variable.
 */
class INSExplicitTimestepSelector : public ElementPostprocessor
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  INSExplicitTimestepSelector(const InputParameters & parameters);
=======
  INSExplicitTimestepSelector(const std::string & name, InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  virtual ~INSExplicitTimestepSelector();

  virtual void initialize();
  virtual void execute();
  virtual Real getValue();
<<<<<<< HEAD
  virtual void finalize();
=======
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  virtual void threadJoin(const UserObject & uo);

protected:
  /// The value of dt (NOTE: _dt member variable is already defined)
  Real _value;

  /// Velocity magnitude.  Hint: Use VectorMagnitudeAux in Moose for this
<<<<<<< HEAD
  const VariableValue & _vel_mag;
=======
  VariableValue& _vel_mag;

  /// Material properties:  the explicit time scheme limit for the viscous
  /// problem also depends on the kinematic viscosity.
  Real _mu;
  Real _rho;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  /// We can compute maximum stable timesteps based on the linearized
  /// theory, but even those timesteps are sometimes still too large
  /// for explicit timestepping in a "real" problem.  Therefore, we
  /// provide an additional "fudge" factor, 0 < beta < 1, that can be
  /// used to reduce the selected timestep even further.
  Real _beta;
<<<<<<< HEAD

  /// Material properties:  the explicit time scheme limit for the viscous
  /// problem also depends on the kinematic viscosity.
  const MaterialProperty<Real> & _mu;
  const MaterialProperty<Real> & _rho;
};
=======
};

#endif /* INSEXPLICITTIMESTEPSELECTOR_H */
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
