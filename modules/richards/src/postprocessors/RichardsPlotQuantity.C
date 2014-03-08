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

#include "RichardsPlotQuantity.h"
#include "RichardsSumQuantity.h"

<<<<<<< HEAD
registerMooseObject("RichardsApp", RichardsPlotQuantity);

InputParameters
RichardsPlotQuantity::validParams()
{
  InputParameters params = GeneralPostprocessor::validParams();
=======
template<>
InputParameters validParams<RichardsPlotQuantity>()
{
  InputParameters params = validParams<GeneralPostprocessor>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  params.addRequiredParam<UserObjectName>("uo", "user object name that has the total mass value");

  return params;
}

<<<<<<< HEAD
RichardsPlotQuantity::RichardsPlotQuantity(const InputParameters & parameters)
  : GeneralPostprocessor(parameters), _total_mass(getUserObject<RichardsSumQuantity>("uo"))
{
}

RichardsPlotQuantity::~RichardsPlotQuantity() {}
=======
RichardsPlotQuantity::RichardsPlotQuantity(const std::string & name, InputParameters parameters) :
    GeneralPostprocessor(name, parameters),
    _total_mass(getUserObject<RichardsSumQuantity>("uo"))
{
}

RichardsPlotQuantity::~RichardsPlotQuantity()
{
}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

void
RichardsPlotQuantity::initialize()
{
}

void
RichardsPlotQuantity::execute()
{
}

PostprocessorValue
RichardsPlotQuantity::getValue()
{
  return _total_mass.getValue();
}
