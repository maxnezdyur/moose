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

#include "Kernel.h"
#include "RichardsVarNames.h"

// Forward Declarations

/**
 * Kernel = grad(permeability*relativepermeability/viscosity * (grad(pressure) - density*gravity))
 * This is mass flow according to the Richards equation
 */
class RichardsFlux : public Kernel
{
public:
  static InputParameters validParams();

  RichardsFlux(const InputParameters & parameters);
=======
/*****************************************/
/* Written by andrew.wilkins@csiro.au    */
/* Please contact me if you make changes */
/*****************************************/

#ifndef RICHARDSFLUX
#define RICHARDSFLUX

#include "Kernel.h"
#include "RichardsPorepressureNames.h"

// Forward Declarations
class RichardsFlux;

template<>
InputParameters validParams<RichardsFlux>();

class RichardsFlux : public Kernel
{
public:

  RichardsFlux(const std::string & name,
                        InputParameters parameters);

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

<<<<<<< HEAD
  /**
   * holds info regarding the names of the Richards variables
   * and methods for extracting values of these variables
   */
  const RichardsVarNames & _richards_name_UO;

  /**
   * the index of this variable in the list of Richards variables
   * held by _richards_name_UO.  Eg
   * if richards_vars = 'pwater pgas poil' in the _richards_name_UO
   * and this kernel has variable = pgas, then _pvar = 1
   * This is used to index correctly into _viscosity, _seff, etc
   */
  unsigned int _pvar;

  /// Richards flux
  const MaterialProperty<std::vector<RealVectorValue>> & _flux;

  /// d(Richards flux_i)/d(variable_j), here flux_i is the i_th flux, which is itself a RealVectorValue
  const MaterialProperty<std::vector<std::vector<RealVectorValue>>> & _dflux_dv;

  /// d(Richards flux_i)/d(grad(variable_j)), here flux_i is the i_th flux, which is itself a RealVectorValue
  const MaterialProperty<std::vector<std::vector<RealTensorValue>>> & _dflux_dgradv;

  /// d^2(Richards flux_i)/d(variable_j)/d(variable_k), here flux_i is the i_th flux, which is itself a RealVectorValue
  const MaterialProperty<std::vector<std::vector<std::vector<RealVectorValue>>>> & _d2flux_dvdv;

  /// d^2(Richards flux_i)/d(grad(variable_j))/d(variable_k), here flux_i is the i_th flux, which is itself a RealVectorValue
  const MaterialProperty<std::vector<std::vector<std::vector<RealTensorValue>>>> & _d2flux_dgradvdv;

  /// d^2(Richards flux_i)/d(variable_j)/d(grad(variable_k)), here flux_i is the i_th flux, which is itself a RealVectorValue
  const MaterialProperty<std::vector<std::vector<std::vector<RealTensorValue>>>> & _d2flux_dvdgradv;

  /// grad_i grad_j porepressure.  This is used in SUPG
  const VariableSecond & _second_u;

  /// interpolation function for the _second_u
  const VariablePhiSecond & _second_phi;

  /// SUPGtau*SUPGvel (a vector of these if multiphase)
  const MaterialProperty<std::vector<RealVectorValue>> & _tauvel_SUPG;

  /// derivative of SUPGtau*SUPGvel_i wrt grad(variable_j)
  const MaterialProperty<std::vector<std::vector<RealTensorValue>>> & _dtauvel_SUPG_dgradv;

  /// derivative of SUPGtau*SUPGvel_i wrt variable_j
  const MaterialProperty<std::vector<std::vector<RealVectorValue>>> & _dtauvel_SUPG_dv;

  /**
   * Computes diagonal and off-diagonal jacobian entries.
   * Since the code is almost identical for both cases it's
   * better to use just one function
   * @param wrt_num take the derivative of the residual wrt this richards variable
   */
  Real computeQpJac(unsigned int wrt_num);
};
=======
  const RichardsPorepressureNames & _pp_name_UO;
  unsigned int _pvar;

  MaterialProperty<std::vector<Real> > &_viscosity;
  MaterialProperty<RealVectorValue> &_gravity;
  MaterialProperty<RealTensorValue> & _permeability;

  MaterialProperty<std::vector<Real> > &_seff;
  MaterialProperty<std::vector<std::vector<Real> > > &_dseff;
  MaterialProperty<std::vector<std::vector<std::vector<Real> > > > &_d2seff;

  MaterialProperty<std::vector<Real> > &_rel_perm;
  MaterialProperty<std::vector<Real> > &_drel_perm;
  MaterialProperty<std::vector<Real> > &_d2rel_perm;

  MaterialProperty<std::vector<Real> > &_density;
  MaterialProperty<std::vector<Real> > &_ddensity;
  MaterialProperty<std::vector<Real> > &_d2density;

  VariableSecond & _second_u;
  VariablePhiSecond & _second_phi;

  MaterialProperty<std::vector<RealVectorValue> >&_tauvel_SUPG;
  MaterialProperty<std::vector<RealTensorValue> >&_dtauvel_SUPG_dgradp;
  MaterialProperty<std::vector<RealVectorValue> >&_dtauvel_SUPG_dp;

 private:
  Real mobility(Real density, Real relperm);
  Real dmobility_dp(Real density, Real ddensity_dp, Real relperm, Real drelperm_dp);
  Real d2mobility_dp2(Real density, Real ddensity_dp, Real d2density_dp2, Real relperm, Real drelperm_dp, Real d2relperm_dp2);
};

#endif //RICHARDSFLUX
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
