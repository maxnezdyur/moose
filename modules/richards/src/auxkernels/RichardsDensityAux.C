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

//  This post processor returns the density of a region.
//
#include "RichardsDensityAux.h"

<<<<<<< HEAD
registerMooseObject("RichardsApp", RichardsDensityAux);

InputParameters
RichardsDensityAux::validParams()
{
  InputParameters params = AuxKernel::validParams();
  params.addRequiredCoupledVar("pressure_var", "The variable that represents the pressure");
  params.addRequiredParam<UserObjectName>("density_UO",
                                          "Name of user object that defines the density.");
=======
template<>
InputParameters validParams<RichardsDensityAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("pressure_var", "The variable that represents the pressure");
  params.addRequiredParam<UserObjectName>("density_UO", "Name of user object that defines the density.");
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  params.addClassDescription("auxillary variable which is fluid density");
  return params;
}

<<<<<<< HEAD
RichardsDensityAux::RichardsDensityAux(const InputParameters & parameters)
  : AuxKernel(parameters),
    _pressure_var(coupledValue("pressure_var")),
    _density_UO(getUserObject<RichardsDensity>("density_UO"))
{
}
=======
RichardsDensityAux::RichardsDensityAux(const std::string & name, InputParameters parameters) :
  AuxKernel(name, parameters),
  _pressure_var(coupledValue("pressure_var")),
  _density_UO(getUserObject<RichardsDensity>("density_UO"))
{}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

Real
RichardsDensityAux::computeValue()
{
  return _density_UO.density(_pressure_var[_qp]);
}
