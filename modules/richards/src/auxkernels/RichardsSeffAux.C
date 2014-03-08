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
/*****************************************/
/* Written by andrew.wilkins@csiro.au    */
/* Please contact me if you make changes */
/*****************************************/
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

//  This post processor returns the effective saturation of a region.
//
#include "RichardsSeffAux.h"

<<<<<<< HEAD
registerMooseObject("RichardsApp", RichardsSeffAux);

InputParameters
RichardsSeffAux::validParams()
{
  InputParameters params = AuxKernel::validParams();
  params.addRequiredCoupledVar("pressure_vars", "List of variables that represent the pressure");
  params.addRequiredParam<UserObjectName>("seff_UO",
                                          "Name of user object that defines effective saturation.");
=======
template<>
InputParameters validParams<RichardsSeffAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("pressure_vars", "List of variables that represent the pressure");
  params.addRequiredParam<UserObjectName>("seff_UO", "Name of user object that defines effective saturation.");
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  params.addClassDescription("auxillary variable which is effective saturation");
  return params;
}

<<<<<<< HEAD
RichardsSeffAux::RichardsSeffAux(const InputParameters & parameters)
  : AuxKernel(parameters),
    _seff_UO(getUserObject<RichardsSeff>("seff_UO")),
    _pressure_vals(coupledValues("pressure_vars"))
{
}

Real
RichardsSeffAux::computeValue()
{
=======
RichardsSeffAux::RichardsSeffAux(const std::string & name, InputParameters parameters) :
  AuxKernel(name, parameters),
  _seff_UO(getUserObject<RichardsSeff>("seff_UO"))
{
  int n = coupledComponents("pressure_vars");
  _pressure_vars.resize(n);
  _pressure_vals.resize(n);

  for (int i=0 ; i<n; ++i)
    {
      _pressure_vars[i] = coupled("pressure_vars", i);
      _pressure_vals[i] = &coupledValue("pressure_vars", i);
    }
}


Real
RichardsSeffAux::computeValue()
{
  //Moose::out << "aux value " << _seff_UO.seff(_pressure_vals, _qp) << "\n";
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  return _seff_UO.seff(_pressure_vals, _qp);
}
