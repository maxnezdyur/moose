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

#include "Material.h"

// Forward Declarations

/**
 * Holds Langmuir parameters associated with desorption
 * Calculates mass-flow rates and derivatives thereof for use by kernels
 */
class LangmuirMaterial : public Material
{
public:
  static InputParameters validParams();

  LangmuirMaterial(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

private:
  /// reciprocal of desorption time constant
  const VariableValue & _one_over_de_time_const;

  /// reciprocal of adsorption time constant
  const VariableValue & _one_over_ad_time_const;

  /// langmuir density
  const Real _langmuir_dens;

  /// langmuir pressure
  const Real _langmuir_p;

  /// concentration of adsorbed fluid in matrix
  const VariableValue & _conc;

  /// porespace pressure (or partial pressure if multiphase flow scenario)
  const VariableValue & _pressure;

  /// mass flow rate from the matrix = mass flow rate to the porespace
  MaterialProperty<Real> & _mass_rate_from_matrix;

  /// derivative of mass flow rate wrt concentration
  MaterialProperty<Real> & _dmass_rate_from_matrix_dC;

  /// derivative of mass flow rate wrt pressure
  MaterialProperty<Real> & _dmass_rate_from_matrix_dp;
};
=======
#ifndef LANGMUIRMATERIAL_H
#define LANGMUIRMATERIAL_H

#include "Material.h"

//Forward Declarations
class LangmuirMaterial;

template<>
InputParameters validParams<LangmuirMaterial>();

class LangmuirMaterial : public Material
{
public:
  LangmuirMaterial(const std::string & name,
                  InputParameters parameters);

protected:

  virtual void computeQpProperties();


private:

  Real _mat_desorption_time_const;
  Real _mat_adsorption_time_const;
  VariableValue * _desorption_time_const_change;
  VariableValue * _adsorption_time_const_change;
  Real _mat_langmuir_density;
  Real _mat_langmuir_pressure;

  VariableValue * _pressure;

  MaterialProperty<Real> & _desorption_time_const;
  MaterialProperty<Real> & _adsorption_time_const;
  MaterialProperty<Real> & _equilib_conc;
  MaterialProperty<Real> & _equilib_conc_prime;

};

#endif //LANGMUIRMATERIAL_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
