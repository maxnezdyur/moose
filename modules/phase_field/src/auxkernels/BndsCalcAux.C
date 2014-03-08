<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "BndsCalcAux.h"
#include "BndsCalculator.h"

registerMooseObject("PhaseFieldApp", BndsCalcAux);

InputParameters
BndsCalcAux::validParams()
{
  InputParameters params = AuxKernel::validParams();
  params.addClassDescription("Calculate location of grain boundaries in a polycrystalline sample");
  params.addRequiredCoupledVarWithAutoBuild(
      "v", "var_name_base", "op_num", "Array of coupled variables");
  return params;
}

BndsCalcAux::BndsCalcAux(const InputParameters & parameters)
  : AuxKernel(parameters), _op_num(coupledComponents("v")), _vals(coupledValues("v"))
{
=======
#include "BndsCalcAux.h"
#include "AddV.h"

template<>
InputParameters validParams<BndsCalcAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addCoupledVar("v", "Array of coupled variables");
  params.addRequiredParam<unsigned int>("crys_num","number of grains");
  params.addRequiredParam<std::string>("var_name_base","base for variable names");

  return params;
}

BndsCalcAux::BndsCalcAux(const std::string & name, InputParameters parameters)
    :AuxKernel(name, AddV(parameters) )
{
  _ncrys = coupledComponents("v");

  _vals.resize(_ncrys);

  for (unsigned int i=0; i<_ncrys; ++i)
    _vals[i] = &coupledValue("v", i);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

Real
BndsCalcAux::computeValue()
{
<<<<<<< HEAD
  return BndsCalculator::computeBndsVariable(_vals, _qp);
=======
  Real value = 0;

  for (unsigned int i=0; i<_ncrys; ++i)
    value += (*_vals[i])[_qp]*(*_vals[i])[_qp];

  return value;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
