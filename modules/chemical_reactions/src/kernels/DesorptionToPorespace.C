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
#include "DesorptionToPorespace.h"

#include <iostream>

<<<<<<< HEAD
registerMooseObject("ChemicalReactionsApp", DesorptionToPorespace);

InputParameters
DesorptionToPorespace::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addRequiredCoupledVar("conc_var",
                               "Variable representing the concentration (kg/m^3) of "
                               "fluid in the matrix that will be desorped to "
                               "porespace");
  params.addClassDescription("Mass flow rate to the porespace from the matrix.  Add this to the "
                             "other kernels for the porepressure variable to form the complete DE");
  return params;
}

DesorptionToPorespace::DesorptionToPorespace(const InputParameters & parameters)
  : Kernel(parameters),
    _conc_var(coupled("conc_var")),
    _mass_rate_from_matrix(getMaterialProperty<Real>("mass_rate_from_matrix")),
    _dmass_rate_from_matrix_dC(getMaterialProperty<Real>("dmass_rate_from_matrix_dC")),
    _dmass_rate_from_matrix_dp(getMaterialProperty<Real>("dmass_rate_from_matrix_dp"))
{
}

Real
DesorptionToPorespace::computeQpResidual()
{
  return -_test[_i][_qp] * _mass_rate_from_matrix[_qp];
=======

template<>
InputParameters validParams<DesorptionToPorespace>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar("conc_var", "Variable representing the concentration (kg/m^3) of fluid in the matrix that will be desorped to porespace");
  params.addClassDescription("Add this to the variable's equation so fluid contained in the matrix with concentration conc_var will desorb to the variable's porespace mass");
  return params;
}

DesorptionToPorespace::DesorptionToPorespace(const std::string & name,
                                             InputParameters parameters) :
    Kernel(name,parameters),
    _conc_val(&coupledValue("conc_var")),
    _conc_var(coupled("conc_var")),

    _desorption_time_const(getMaterialProperty<Real>("desorption_time_const")),
    _adsorption_time_const(getMaterialProperty<Real>("adsorption_time_const")),
    _equilib_conc(getMaterialProperty<Real>("desorption_equilib_conc")),
    _equilib_conc_prime(getMaterialProperty<Real>("desorption_equilib_conc_prime"))
{
}


Real
DesorptionToPorespace::computeQpResidual()
{
  if ((*_conc_val)[_qp] > _equilib_conc[_qp])
  {
    if (_desorption_time_const[_qp] > 0)
      return -_test[_i][_qp]*((*_conc_val)[_qp] - _equilib_conc[_qp])/_desorption_time_const[_qp];
    return 0.0;
  }
  if (_adsorption_time_const[_qp] > 0)
    return -_test[_i][_qp]*((*_conc_val)[_qp] - _equilib_conc[_qp])/_adsorption_time_const[_qp];
  return 0.0;

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

Real
DesorptionToPorespace::computeQpJacobian()
{
<<<<<<< HEAD
  return -_test[_i][_qp] * _dmass_rate_from_matrix_dp[_qp] * _phi[_j][_qp];
=======
  if ((*_conc_val)[_qp] > _equilib_conc[_qp])
  {
    if (_desorption_time_const[_qp] > 0)
      return _test[_i][_qp]*_equilib_conc_prime[_qp]*_phi[_j][_qp]/_desorption_time_const[_qp];
    return 0.0;
  }
  if (_adsorption_time_const[_qp] > 0)
    return _test[_i][_qp]*_equilib_conc_prime[_qp]*_phi[_j][_qp]/_adsorption_time_const[_qp];
  return 0.0;

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

Real
DesorptionToPorespace::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar != _conc_var)
    return 0.0;
<<<<<<< HEAD
  return -_test[_i][_qp] * _dmass_rate_from_matrix_dC[_qp] * _phi[_j][_qp];
=======
  if ((*_conc_val)[_qp] > _equilib_conc[_qp])
  {
    if (_desorption_time_const[_qp] > 0)
      return -_test[_i][_qp]*_phi[_j][_qp]/_desorption_time_const[_qp];
    return 0.0;
  }
  if (_adsorption_time_const[_qp] > 0)
    return -_test[_i][_qp]*_phi[_j][_qp]/_adsorption_time_const[_qp];
  return 0.0;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
