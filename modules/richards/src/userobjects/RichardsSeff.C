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

//  Base class for effective saturation as a function of pressure(s)
//
#include "RichardsSeff.h"

<<<<<<< HEAD
InputParameters
RichardsSeff::validParams()
{
  InputParameters params = GeneralUserObject::validParams();
  params.addClassDescription(
      "Fluid seff base class.  Override seff, dseff and d2seff in your class");
  return params;
}

RichardsSeff::RichardsSeff(const InputParameters & parameters) : GeneralUserObject(parameters) {}

void
RichardsSeff::initialize()
{
}

void
RichardsSeff::execute()
{
}

void
RichardsSeff::finalize()
{
}
=======
template<>
InputParameters validParams<RichardsSeff>()
{
  InputParameters params = validParams<GeneralUserObject>();
  params.addClassDescription("Fluid seff base class.  Override seff, dseff and d2seff in your class");
  return params;
}

RichardsSeff::RichardsSeff(const std::string & name, InputParameters parameters) :
  GeneralUserObject(name, parameters)
{}

void
RichardsSeff::initialize()
{}

void
RichardsSeff::execute()
{}

void RichardsSeff::finalize()
{}

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
