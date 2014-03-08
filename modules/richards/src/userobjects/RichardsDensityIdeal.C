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

//  Fluid density ideal gas
//
#include "RichardsDensityIdeal.h"

<<<<<<< HEAD
registerMooseObject("RichardsApp", RichardsDensityIdeal);

InputParameters
RichardsDensityIdeal::validParams()
{
  InputParameters params = RichardsDensity::validParams();
=======
template<>
InputParameters validParams<RichardsDensityIdeal>()
{
  InputParameters params = validParams<RichardsDensity>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  params.addRequiredParam<Real>("slope", "Density = slope*(p - p0)");
  params.addRequiredParam<Real>("p0", "Density = slope*(p - p0)");
  params.addClassDescription("Fluid density of ideal gas.  Density = slope*(p - p0)");
  return params;
}

<<<<<<< HEAD
RichardsDensityIdeal::RichardsDensityIdeal(const InputParameters & parameters)
  : RichardsDensity(parameters), _slope(getParam<Real>("slope")), _p0(getParam<Real>("p0"))
{
}
=======
RichardsDensityIdeal::RichardsDensityIdeal(const std::string & name, InputParameters parameters) :
  RichardsDensity(name, parameters),
  _slope(getParam<Real>("slope")),
  _p0(getParam<Real>("p0"))
{}

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

Real
RichardsDensityIdeal::density(Real p) const
{
<<<<<<< HEAD
  return _slope * (p - _p0);
}

Real RichardsDensityIdeal::ddensity(Real /*p*/) const { return _slope; }

Real RichardsDensityIdeal::d2density(Real /*p*/) const { return 0.0; }
=======
  return _slope*(p - _p0);
}

Real
RichardsDensityIdeal::ddensity(Real /*p*/) const
{
  return _slope;
}

Real
RichardsDensityIdeal::d2density(Real /*p*/) const
{
  return 0.0;
}

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
