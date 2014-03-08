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

//  Base class for Richards SUPG
//
#include "RichardsSUPG.h"

<<<<<<< HEAD
InputParameters
RichardsSUPG::validParams()
{
  InputParameters params = GeneralUserObject::validParams();
=======
template<>
InputParameters validParams<RichardsSUPG>()
{
  InputParameters params = validParams<GeneralUserObject>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  params.addClassDescription("Richards SUPG base class.  Override tauSUPG, etc");
  return params;
}

<<<<<<< HEAD
RichardsSUPG::RichardsSUPG(const InputParameters & parameters) : GeneralUserObject(parameters) {}

void
RichardsSUPG::initialize()
{
}

void
RichardsSUPG::execute()
{
}

void
RichardsSUPG::finalize()
{
}
=======
RichardsSUPG::RichardsSUPG(const std::string & name, InputParameters parameters) :
  GeneralUserObject(name, parameters)
{}

void
RichardsSUPG::initialize()
{}

void
RichardsSUPG::execute()
{}

void RichardsSUPG::finalize()
{}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
