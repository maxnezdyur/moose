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

//  Fluid density assuming constant bulk modulus
//
#include "RichardsDensityConstBulk.h"

<<<<<<< HEAD
registerMooseObject("RichardsApp", RichardsDensityConstBulk);

InputParameters
RichardsDensityConstBulk::validParams()
{
  InputParameters params = RichardsDensity::validParams();
  params.addRequiredParam<Real>("dens0", "Reference density of fluid.  Eg 1000");
  params.addRequiredParam<Real>("bulk_mod", "Bulk modulus of fluid.  Eg 2E9");
  params.addClassDescription(
      "Fluid density assuming constant bulk modulus.  dens0 * Exp(pressure/bulk)");
  return params;
}

RichardsDensityConstBulk::RichardsDensityConstBulk(const InputParameters & parameters)
  : RichardsDensity(parameters), _dens0(getParam<Real>("dens0")), _bulk(getParam<Real>("bulk_mod"))
{
}
=======
template<>
InputParameters validParams<RichardsDensityConstBulk>()
{
  InputParameters params = validParams<RichardsDensity>();
  params.addRequiredParam<Real>("dens0", "Reference density of fluid.  Eg 1000");
  params.addRequiredParam<Real>("bulk_mod", "Bulk modulus of fluid.  Eg 2E9");
  params.addClassDescription("Fluid density assuming constant bulk modulus.  dens0 * Exp(pressure/bulk)");
  return params;
}

RichardsDensityConstBulk::RichardsDensityConstBulk(const std::string & name, InputParameters parameters) :
  RichardsDensity(name, parameters),
  _dens0(getParam<Real>("dens0")),
  _bulk(getParam<Real>("bulk_mod"))
{}

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

Real
RichardsDensityConstBulk::density(Real p) const
{
<<<<<<< HEAD
  return _dens0 * std::exp(p / _bulk);
=======
  return _dens0*std::exp(p/_bulk);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

Real
RichardsDensityConstBulk::ddensity(Real p) const
{
<<<<<<< HEAD
  return density(p) / _bulk;
=======
  return density(p)/_bulk;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

Real
RichardsDensityConstBulk::d2density(Real p) const
{
<<<<<<< HEAD
  return density(p) / _bulk / _bulk;
}
=======
  return density(p)/_bulk/_bulk;
}

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
