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

#include "RichardsExcav.h"
#include "Function.h"

#include <iostream>

<<<<<<< HEAD
registerMooseObject("RichardsApp", RichardsExcav);

InputParameters
RichardsExcav::validParams()
{
  InputParameters params = NodalBC::validParams();
  params.addRequiredParam<Real>(
      "p_excav",
      "Value of the variable at the surface of the excavation.  Eg atmospheric pressure");
  params.addRequiredParam<FunctionName>(
      "excav_geom_function",
      "The function describing the excavation geometry (type RichardsExcavGeom)");
  params.addClassDescription("Allows the user to set variable values at the face of an excavation. "
                             " You must have defined the excavation start time, start position, "
                             "etc, through the excav_geom_function");
  return params;
}

RichardsExcav::RichardsExcav(const InputParameters & parameters)
  : NodalBC(parameters),
    _p_excav(getParam<Real>("p_excav")),
    _func(getFunction("excav_geom_function"))
{
}
=======

template<>
InputParameters validParams<RichardsExcav>()
{
  InputParameters params = validParams<NodalBC>();
  params.addRequiredParam<Real>("p_excav", "Porepressure at the surface of the excavation.  Usually this is atmospheric pressure");
  params.addRequiredParam<FunctionName>("excav_geom_function", "The function describing the excavation geometry (type RichardsExcavGeom)");
  params.addClassDescription("Allows the user to set porepressure at the face of an excavation.  You must have defined the excavation start time, start position, etc, through the excav_geom_function");
  return params;
}

RichardsExcav::RichardsExcav(const std::string & name,
                                             InputParameters parameters) :
    NodalBC(name,parameters),
    _p_excav(getParam<Real>("p_excav")),
    _func(getFunction("excav_geom_function"))
{}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

bool
RichardsExcav::shouldApply()
{
  if (_func.value(_t, *_current_node) == 0.0)
    return false;
  else
    return true;
}

Real
RichardsExcav::computeQpResidual()
{
  return _u[_qp] - _p_excav;
}
<<<<<<< HEAD
=======

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
