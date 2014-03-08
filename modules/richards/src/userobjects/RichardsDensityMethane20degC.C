<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

//  Methane density - a quadratic fit to expressions in:
// "Results of (pressure, density, temperature) measurements on methane and on nitrogen in the
// temperature range from 273.15K to 323.15K at pressures up to 12MPa using new apparatus for
// accurate gas-density"
//
// This is only valid for p>=0, which is the physical region.  I extend to the p>0 domain with an
// exponential, which will probably be sampled as the newton interative process converges towards
// the solution.
#include "RichardsDensityMethane20degC.h"

registerMooseObject("RichardsApp", RichardsDensityMethane20degC);

InputParameters
RichardsDensityMethane20degC::validParams()
{
  InputParameters params = RichardsDensity::validParams();
  params.addParam<Real>(
      "p_unit",
      1,
      "Set to 1 for pressure measured in Pascals.  Set to 1E6 for pressure measured in MPa.  Etc.");
  params.addClassDescription("Methane density (kg/m^3) at 20degC.  Pressure is assumed to be "
                             "measured in Pascals.  NOTE: this expression is only valid to about "
                             "P=20MPa.  Use van der Waals (RichardsDensityVDW) for higher "
                             "pressures.");
  return params;
}

RichardsDensityMethane20degC::RichardsDensityMethane20degC(const InputParameters & parameters)
  : RichardsDensity(parameters), _p_unit(getParam<Real>("p_unit"))
{
}
=======
/*****************************************/
/* Written by andrew.wilkins@csiro.au    */
/* Please contact me if you make changes */
/*****************************************/

//  Methane density - a quadratic fit to expressions in:
// "Results of (pressure, density, temperature) measurements on methane and on nitrogen in the temperature range from 273.15K to 323.15K at pressures up to 12MPa using new apparatus for accurate gas-density"
//
// This is only valid for p>=0, which is the physical region.  I extend to the p>0 domain with an exponential, which will probably be sampled as the newton interative process converges towards the solution.
#include "RichardsDensityMethane20degC.h"

template<>
InputParameters validParams<RichardsDensityMethane20degC>()
{
  InputParameters params = validParams<RichardsDensity>();
  params.addClassDescription("Methane density (kg/m^3) at 20degC.  Pressure is assumed to be measured in Pascals.");
  return params;
}

RichardsDensityMethane20degC::RichardsDensityMethane20degC(const std::string & name, InputParameters parameters) :
  RichardsDensity(name, parameters)
{}

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

Real
RichardsDensityMethane20degC::density(Real p) const
{
  if (p >= 0)
<<<<<<< HEAD
    return 0.00654576947608E-3 * p * _p_unit + 1.04357716547E-13 * std::pow(p * _p_unit, 2);
  else
    return 0.1 * (std::exp(6.54576947608E-5 * p * _p_unit) - 1);
=======
    return 0.00654576947608E-3*p + 1.04357716547E-13*std::pow(p, 2);
  else
    return 0.1*(std::exp(6.54576947608E-5*p) - 1);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

Real
RichardsDensityMethane20degC::ddensity(Real p) const
{
  if (p >= 0)
<<<<<<< HEAD
    return (0.00654576947608E-3 + 2.08715433094E-13 * p * _p_unit) * _p_unit;
  else
    return 0.1 * 6.54576947608E-5 * _p_unit * std::exp(6.54576947608E-5 * p * _p_unit);
=======
    return 0.00654576947608E-3 + 2.08715433094E-13*p;
  else
    return 0.1*6.54576947608E-5*std::exp(6.54576947608E-5*p);

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

Real
RichardsDensityMethane20degC::d2density(Real p) const
{
  if (p >= 0)
<<<<<<< HEAD
    return 2.08715433094E-13 * std::pow(_p_unit, 2);
  else
    return 0.1 * std::pow(6.54576947608E-5 * _p_unit, 2) * std::exp(6.54576947608E-5 * p * _p_unit);
}
=======
    return 2.08715433094E-13;
  else
    return 0.1*std::pow(6.54576947608E-5, 2)*std::exp(6.54576947608E-5*p);
}

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
