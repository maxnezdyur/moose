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

//  saturation as a function of effective saturation, and its derivs wrt effective saturation
//
#include "RichardsSat.h"

<<<<<<< HEAD
registerMooseObject("RichardsApp", RichardsSat);

InputParameters
RichardsSat::validParams()
{
  InputParameters params = GeneralUserObject::validParams();
  params.addRequiredRangeCheckedParam<Real>(
      "s_res",
      "s_res >= 0 & s_res < 1",
      "Residual fluid saturation for the phase.  0 <= s_res < 1.");
  params.addRequiredRangeCheckedParam<Real>(
      "sum_s_res",
      "sum_s_res < 1",
      "Sum of s_res over all phases.  s_res <= sum_s_res < 1.  It is "
      "up to you to ensure the sum is done correctly.");
  params.addClassDescription("User object yielding saturation for a phase as a function of "
                             "effective saturation of that phase");
  return params;
}

RichardsSat::RichardsSat(const InputParameters & parameters)
  : GeneralUserObject(parameters),
    _s_res(getParam<Real>("s_res")),
    _sum_s_res(getParam<Real>("sum_s_res"))
{
  if (_sum_s_res < _s_res)
    mooseError("sum_s_res set to ", _sum_s_res, " but it must obey s_res <= sum_s_res < 1");
=======
template<>
InputParameters validParams<RichardsSat>()
{
  InputParameters params = validParams<GeneralUserObject>();
  params.addClassDescription("User object yielding saturation as a function of effective saturation");
  params.addRequiredParam<Real>("s_res", "Residual fluid saturation.  0 <= s_res < 1.");
  params.addRequiredParam<Real>("sum_s_res", "Sum of s_res over all phases.  s_res <= sum_s_res < 1.  It is up to you to ensure the sum is done correctly.");
  return params;
}

RichardsSat::RichardsSat(const std::string & name, InputParameters parameters) :
  GeneralUserObject(name, parameters),
  _s_res(getParam<Real>("s_res")),
  _sum_s_res(getParam<Real>("sum_s_res"))
{
  if (_s_res < 0 || _s_res >= 1)
    mooseError("Residual saturation set to " << _s_res << " but it must obey 0 <= s_res < 1");
  if (_sum_s_res < _s_res || _sum_s_res >= 1)
    mooseError("sum_s_res set to " << _sum_s_res << " but it must obey s_res <= sum_s_res < 1");
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

void
RichardsSat::initialize()
<<<<<<< HEAD
{
}

void
RichardsSat::execute()
{
}

void
RichardsSat::finalize()
{
}
=======
{}

void
RichardsSat::execute()
{}

void RichardsSat::finalize()
{}

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

Real
RichardsSat::sat(Real seff) const
{
<<<<<<< HEAD
  return _s_res + seff * (1.0 - _sum_s_res);
}

Real RichardsSat::dsat(Real /*seff*/) const { return 1.0 - _sum_s_res; }
=======
  return _s_res + seff*(1.0 - _sum_s_res);
}

Real
RichardsSat::dsat(Real /*seff*/) const
{
  return 1.0 - _sum_s_res;
}

Real
RichardsSat::d2sat(Real /*seff*/) const
{
  return 0.0;
}

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
