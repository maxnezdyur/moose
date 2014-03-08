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
#ifndef DESORPTIONFROMMATRIX
#define DESORPTIONFROMMATRIX
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "Kernel.h"
#include "LangmuirMaterial.h"

// Forward Declarations
<<<<<<< HEAD

/**
 * Mass flow rate of adsorbed fluid from matrix
 * Add this to TimeDerivative to form the entire DE for desorption of fluid-in-the-matrix
 */
class DesorptionFromMatrix : public Kernel
{
public:
  static InputParameters validParams();

  DesorptionFromMatrix(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

  /// MOOSE internal variable number corresponding to the porepressure (need this of OffDiagJacobian)
  const unsigned int _pressure_var;

  /// mass flow rate from matrix = mass flow rate to porespace
  const MaterialProperty<Real> & _mass_rate_from_matrix;

  /// derivative of mass flow rate from matrix wrt concentration
  const MaterialProperty<Real> & _dmass_rate_from_matrix_dC;

  /// derivative of mass flow rate from matrix wrt pressure
  const MaterialProperty<Real> & _dmass_rate_from_matrix_dp;
};
=======
class DesorptionFromMatrix;

template<>
InputParameters validParams<DesorptionFromMatrix>();

class DesorptionFromMatrix : public Kernel
{
public:

  DesorptionFromMatrix(const std::string & name,
                        InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  unsigned int _pressure_var;

  MaterialProperty<Real> &_desorption_time_const;
  MaterialProperty<Real> &_adsorption_time_const;
  MaterialProperty<Real> &_equilib_conc;
  MaterialProperty<Real> &_equilib_conc_prime;

};

#endif //DESORPTIONFROMMATRIX
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
