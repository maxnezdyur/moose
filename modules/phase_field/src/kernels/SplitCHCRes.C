<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "SplitCHCRes.h"

InputParameters
SplitCHCRes::validParams()
{
  InputParameters params = SplitCHBase::validParams();
  params.addClassDescription("Split formulation Cahn-Hilliard Kernel");
  params.addRequiredCoupledVar("w", "Chemical potential");
  params.addRequiredParam<MaterialPropertyName>("kappa_name", "The kappa used with the kernel");
  return params;
}

SplitCHCRes::SplitCHCRes(const InputParameters & parameters)
  : SplitCHBase(parameters),
    _kappa(getMaterialProperty<Real>("kappa_name")),
    _w_var(coupled("w")),
    _w(coupledValue("w"))
=======
#include "SplitCHCRes.h"
// The couple, SplitCHCRes and SplitCHWRes, splits the CH equation by replacing chemical potential with 'w'.
template<>
InputParameters validParams<SplitCHCRes>()
{
  InputParameters params = validParams<SplitCHBase>();

  params.addRequiredCoupledVar("w","chem poten");
  params.addRequiredParam<std::string>("kappa_name","The kappa used with the kernel");

  return params;
}

SplitCHCRes::SplitCHCRes(const std::string & name, InputParameters parameters)
  :SplitCHBase(name, parameters),
   _kappa_name(getParam<std::string>("kappa_name")),
   _kappa(getMaterialProperty<Real>(_kappa_name)),
   _w_var(coupled("w")),
   _w(coupledValue("w")),
   _grad_w(coupledGradient("w"))
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
}

/*Real
SplitCHCRes::computeDFDC(PFFunctionType type)
{
  switch (type)
  {
  case Residual:
    return _u[_qp]*_u[_qp]*_u[_qp] - _u[_qp]; // return Residual value

  case Jacobian:
    return (3.0*_u[_qp]*_u[_qp] - 1.0)*_phi[_j][_qp]; //return Jacobian value

  }

  mooseError("Invalid type passed in");
<<<<<<< HEAD
}*/
=======
  }*/
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

Real
SplitCHCRes::computeQpResidual()
{
<<<<<<< HEAD
  Real residual =
      SplitCHBase::computeQpResidual(); //(f_prime_zero+e_prime)*_test[_i][_qp] from SplitCHBase

  residual += -_w[_qp] * _test[_i][_qp];
  residual += _kappa[_qp] * _grad_u[_qp] * _grad_test[_i][_qp];

  return residual;
=======
  Real residual = SplitCHBase::computeQpResidual(); //(f_prime_zero+e_prime)*_test[_i][_qp] from SplitCHBase

  residual += -_w[_qp]*_test[_i][_qp];

  residual += _kappa[_qp]*_grad_u[_qp]*_grad_test[_i][_qp];

  return residual;

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

Real
SplitCHCRes::computeQpJacobian()
{
<<<<<<< HEAD
  Real jacobian = SplitCHBase::computeQpJacobian(); //(df_prime_zero_dc+de_prime_dc)*_test[_i][_qp];
                                                    // from SplitCHBase

  jacobian += _kappa[_qp] * _grad_phi[_j][_qp] * _grad_test[_i][_qp];

  return jacobian;
=======
  Real jacobian = SplitCHBase::computeQpJacobian(); //(df_prime_zero_dc+de_prime_dc)*_test[_i][_qp]; from SplitCHBase

  jacobian += _kappa[_qp]*_grad_phi[_j][_qp]*_grad_test[_i][_qp];

  return jacobian;

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

Real
SplitCHCRes::computeQpOffDiagJacobian(unsigned int jvar)
{
<<<<<<< HEAD
  if (jvar == _w_var)
  {
    return -_phi[_j][_qp] * _test[_i][_qp];
=======

  if(jvar == _w_var)
  {


   return -_phi[_j][_qp] * _test[_i][_qp];


>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  }

  return 0.0;
}
<<<<<<< HEAD
=======

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
