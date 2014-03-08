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

//  Base class for relative permeability as a function of effective saturation
//
#include "RichardsRelPerm.h"

<<<<<<< HEAD
InputParameters
RichardsRelPerm::validParams()
{
  InputParameters params = GeneralUserObject::validParams();
  params.addClassDescription(
      "Relative permeability base class.  Override relperm, drelperm and d2relperm in your class");
  return params;
}

RichardsRelPerm::RichardsRelPerm(const InputParameters & parameters) : GeneralUserObject(parameters)
{
}

void
RichardsRelPerm::initialize()
{
}

void
RichardsRelPerm::execute()
{
}

void
RichardsRelPerm::finalize()
{
}
=======
template<>
InputParameters validParams<RichardsRelPerm>()
{
  InputParameters params = validParams<GeneralUserObject>();
  params.addClassDescription("Relative permeability base class.  Override relperm, drelperm and d2relperm in your class");
  return params;
}

RichardsRelPerm::RichardsRelPerm(const std::string & name, InputParameters parameters) :
  GeneralUserObject(name, parameters)
{}

void
RichardsRelPerm::initialize()
{}

void
RichardsRelPerm::execute()
{}

void RichardsRelPerm::finalize()
{}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
