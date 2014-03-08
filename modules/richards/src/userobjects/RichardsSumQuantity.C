<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "RichardsSumQuantity.h"

registerMooseObject("RichardsApp", RichardsSumQuantity);

InputParameters
RichardsSumQuantity::validParams()
{
  InputParameters params = GeneralUserObject::validParams();
=======
/*****************************************/
/* Written by andrew.wilkins@csiro.au    */
/* Please contact me if you make changes */
/*****************************************/

#include "RichardsSumQuantity.h"

template<>
InputParameters validParams<RichardsSumQuantity>()
{
  InputParameters params = validParams<GeneralUserObject>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  return params;
}

<<<<<<< HEAD
RichardsSumQuantity::RichardsSumQuantity(const InputParameters & parameters)
  : GeneralUserObject(parameters), _total(0)
{
}

RichardsSumQuantity::~RichardsSumQuantity() {}
=======
RichardsSumQuantity::RichardsSumQuantity(const std::string & name, InputParameters parameters) :
    GeneralUserObject(name, parameters),
    _total_outflow_mass(0)
{
}

RichardsSumQuantity::~RichardsSumQuantity()
{
}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

void
RichardsSumQuantity::zero()
{
<<<<<<< HEAD
  _total = 0;
=======
  _total_outflow_mass = 0;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

void
RichardsSumQuantity::add(Real contrib)
{
<<<<<<< HEAD
  _total += contrib;
=======
  _total_outflow_mass += contrib;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

void
RichardsSumQuantity::initialize()
{
}

void
RichardsSumQuantity::execute()
{
}

void
RichardsSumQuantity::finalize()
{
<<<<<<< HEAD
  gatherSum(_total);
=======
  gatherSum(_total_outflow_mass);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

Real
RichardsSumQuantity::getValue() const
{
<<<<<<< HEAD
  return _total;
=======
  return _total_outflow_mass;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
