<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

=======
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
#include "DesorptionFromMatrix.h"

#include <iostream>

<<<<<<< HEAD
registerMooseObject("ChemicalReactionsApp", DesorptionFromMatrix);

InputParameters
DesorptionFromMatrix::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addRequiredCoupledVar(
      "pressure_var",
      "Variable representing the porepressure of the fluid adsorbed into the matrix");
  params.addClassDescription("Mass flow rate from the matrix to the porespace.  Add this to "
                             "TimeDerivative kernel to get complete DE for the fluid adsorbed "
                             "in the matrix");
  return params;
}

DesorptionFromMatrix::DesorptionFromMatrix(const InputParameters & parameters)
  : Kernel(parameters),
    _pressure_var(coupled("pressure_var")),
    _mass_rate_from_matrix(getMaterialProperty<Real>("mass_rate_from_matrix")),
    _dmass_rate_from_matrix_dC(getMaterialProperty<Real>("dmass_rate_from_matrix_dC")),
    _dmass_rate_from_matrix_dp(getMaterialProperty<Real>("dmass_rate_from_matrix_dp"))
{
}
=======

template<>
InputParameters validParams<DesorptionFromMatrix>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar("pressure_var", "Variable representing the porepressure of the fluid");
  params.addClassDescription("Mass flow rate from the matrix.  Add this to TimeDerivative kernel to get complete DE");
  return params;
}

DesorptionFromMatrix::DesorptionFromMatrix(const std::string & name,
                                           InputParameters parameters) :
    Kernel(name,parameters),
    _pressure_var(coupled("pressure_var")),
    _desorption_time_const(getMaterialProperty<Real>("desorption_time_const")),
    _adsorption_time_const(getMaterialProperty<Real>("adsorption_time_const")),
    _equilib_conc(getMaterialProperty<Real>("desorption_equilib_conc")),
    _equilib_conc_prime(getMaterialProperty<Real>("desorption_equilib_conc_prime"))
{}

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

Real
DesorptionFromMatrix::computeQpResidual()
{
<<<<<<< HEAD
  return _test[_i][_qp] * _mass_rate_from_matrix[_qp];
=======
  if (_u[_qp] > _equilib_conc[_qp])
  {
    if (_desorption_time_const[_qp] > 0)
      return _test[_i][_qp]*(_u[_qp] - _equilib_conc[_qp])/_desorption_time_const[_qp];
    return 0.0;
  }
  if (_adsorption_time_const[_qp] > 0)
    return _test[_i][_qp]*(_u[_qp] - _equilib_conc[_qp])/_adsorption_time_const[_qp];
  return 0.0;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

Real
DesorptionFromMatrix::computeQpJacobian()
{
<<<<<<< HEAD
  return _test[_i][_qp] * _dmass_rate_from_matrix_dC[_qp] * _phi[_j][_qp];
=======
  if (_u[_qp] > _equilib_conc[_qp])
  {
    if (_desorption_time_const[_qp] > 0)
      return _test[_i][_qp]*_phi[_j][_qp]/_desorption_time_const[_qp];
    return 0.0;
  }
  if (_adsorption_time_const[_qp] > 0)
    return _test[_i][_qp]*_phi[_j][_qp]/_adsorption_time_const[_qp];
  return 0.0;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

Real
DesorptionFromMatrix::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar != _pressure_var)
    return 0.0;
<<<<<<< HEAD
  return _test[_i][_qp] * _dmass_rate_from_matrix_dp[_qp] * _phi[_j][_qp];
=======
  if (_u[_qp] > _equilib_conc[_qp])
  {
    if (_desorption_time_const[_qp] > 0)
      return -_test[_i][_qp]*_equilib_conc_prime[_qp]*_phi[_j][_qp]/_desorption_time_const[_qp];
    return 0.0;
  }
  if (_adsorption_time_const[_qp] > 0)
    return -_test[_i][_qp]*_equilib_conc_prime[_qp]*_phi[_j][_qp]/_adsorption_time_const[_qp];
  return 0.0;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
