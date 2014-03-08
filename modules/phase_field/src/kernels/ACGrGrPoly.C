<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ACGrGrPoly.h"

registerMooseObject("PhaseFieldApp", ACGrGrPoly);

InputParameters
ACGrGrPoly::validParams()
{
  InputParameters params = ACGrGrBase::validParams();
  params.addClassDescription("Grain-Boundary model poly-crystalline interface Allen-Cahn Kernel");
  return params;
}

ACGrGrPoly::ACGrGrPoly(const InputParameters & parameters)
  : ACGrGrBase(parameters), _gamma(getMaterialProperty<Real>("gamma_asymm"))
{
=======
#include "ACGrGrPoly.h"

//#include "Material.h"

template<>
InputParameters validParams<ACGrGrPoly>()
{
  InputParameters params = validParams<ACBulk>();

  params.addRequiredCoupledVar("v", "Array of coupled variable names");
  params.addCoupledVar("T","temperature");

  return params;
}

ACGrGrPoly::ACGrGrPoly(const std::string & name, InputParameters parameters)
  :ACBulk(name,parameters),
   _mu(getMaterialProperty<Real>("mu")),
   _gamma(getMaterialProperty<Real>("gamma_asymm")),
   _tgrad_corr_mult(getMaterialProperty<Real>("tgrad_corr_mult")),
   _has_T(isCoupled("T")),
   _grad_T(_has_T ? &coupledGradient("T") : NULL)
{
  //Array of coupled variables is created in the constructor
  _ncrys = coupledComponents("v"); //determine number of grains from the number of names passed in.  Note this is the actual number -1
  _vals.resize(_ncrys); //Size variable arrays
  _vals_var.resize(_ncrys);
//  _gamma = 1.5;

  for (unsigned int i=0; i<_ncrys; ++i)
  {
    //Loop through grains and load coupled variables into the arrays
    _vals[i] = &coupledValue("v", i);
    _vals_var[i] = coupled("v",i);
  }
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

Real
ACGrGrPoly::computeDFDOP(PFFunctionType type)
{
<<<<<<< HEAD
  // Sum all other order parameters
  Real SumEtaj = 0.0;
  for (unsigned int i = 0; i < _op_num; ++i)
    SumEtaj += (*_vals[i])[_qp] * (*_vals[i])[_qp];

  // Calculate either the residual or Jacobian of the grain growth free energy
  switch (type)
  {
    case Residual:
    {
      return _mu[_qp] *
             (_u[_qp] * _u[_qp] * _u[_qp] - _u[_qp] + 2.0 * _gamma[_qp] * _u[_qp] * SumEtaj);
    }

    case Jacobian:
    {
      return _mu[_qp] *
             (_phi[_j][_qp] * (3.0 * _u[_qp] * _u[_qp] - 1.0 + 2.0 * _gamma[_qp] * SumEtaj));
    }

    default:
      mooseError("Invalid type passed in");
  }
=======
  Real SumEtaj = 0.0;
    for (unsigned int i=0; i<_ncrys; ++i)
      SumEtaj += (*_vals[i])[_qp]*(*_vals[i])[_qp]; //Sum all other order parameters

  Real tgrad_correction = 0.0;

  //Calcualte either the residual or jacobian of the grain growth free energy
  switch (type)
  {
  case Residual:
  {
    if (_has_T)
      tgrad_correction = _tgrad_corr_mult[_qp]*_grad_u[_qp]*(*_grad_T)[_qp];

    return _mu[_qp]*(_u[_qp]*_u[_qp]*_u[_qp] - _u[_qp] + 2.0*_gamma[_qp]*_u[_qp]*SumEtaj) + tgrad_correction;
  }

  case Jacobian:
  {
    if (_has_T)
      tgrad_correction = _tgrad_corr_mult[_qp]*_grad_phi[_j][_qp]*(*_grad_T)[_qp];

    return _mu[_qp]*(_phi[_j][_qp]*(3*_u[_qp]*_u[_qp] - 1.0 + 2.0*_gamma[_qp]*SumEtaj)) + tgrad_correction;
  }

  }

  mooseError("Invalid type passed in");
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

Real
ACGrGrPoly::computeQpOffDiagJacobian(unsigned int jvar)
{
<<<<<<< HEAD
  for (unsigned int i = 0; i < _op_num; ++i)
    if (jvar == _vals_var[i])
    {
      // Derivative of SumEtaj
      const Real dSumEtaj = 2.0 * (*_vals[i])[_qp] * _phi[_j][_qp];
      const Real dDFDOP = _mu[_qp] * 2.0 * _gamma[_qp] * _u[_qp] * dSumEtaj;

      return _L[_qp] * _test[_i][_qp] * dDFDOP;
=======
  for (unsigned int i=0; i<_ncrys; ++i)
    if(jvar == _vals_var[i])
    {
      Real dSumEtaj = 2.0*(*_vals[i])[_qp]*_phi[_j][_qp]; //Derivative of SumEtaj

      Real dDFDOP = _mu[_qp]*2.0*_gamma[_qp]*_u[_qp]*dSumEtaj;

      return _L[_qp]*_test[_i][_qp]*dDFDOP;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
    }

  return 0.0;
}
