<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "GradParsedFunction.h"
#include "MooseParsedFunctionWrapper.h"

registerMooseObject("RichardsApp", GradParsedFunction);

InputParameters
GradParsedFunction::validParams()
{
  InputParameters params = MooseParsedFunction::validParams();
  params += MooseParsedFunction::validParams();
  params.addRequiredParam<RealVectorValue>(
      "direction",
      "The direction in which to take the derivative.  This must not be a zero-length vector");
  return params;
}

GradParsedFunction::GradParsedFunction(const InputParameters & parameters)
  : MooseParsedFunction(parameters), _direction(getParam<RealVectorValue>("direction"))
{
  _len = _direction.norm();
=======
/*****************************************/
/* Written by andrew.wilkins@csiro.au    */
/* Please contact me if you make changes */
/*****************************************/

#include "GradParsedFunction.h"

template<>
InputParameters validParams<GradParsedFunction>()
{
  InputParameters params = validParams<MooseParsedFunction>();
  params += validParams<MooseParsedFunction>();
  params.addRequiredParam<RealVectorValue>("direction", "The direction in which to take the derivative.  This must not be a zero-length vector");
  return params;
}

GradParsedFunction::GradParsedFunction(const std::string & name, InputParameters parameters) :
    MooseParsedFunction(name, parameters),
    _direction(getParam<RealVectorValue>("direction"))
{
  _len = std::pow(_direction*_direction, 0.5);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  if (_len == 0)
    mooseError("The direction in the GradParsedFunction must have positive length.");
  _direction /= 2.0; // note - so we can do central differences
}

Real
<<<<<<< HEAD
GradParsedFunction::value(Real t, const Point & p) const
{
  return (_function_ptr->evaluate<Real>(t, p + _direction) -
          _function_ptr->evaluate<Real>(t, p - _direction)) /
         _len;
=======
GradParsedFunction::value(Real t, const Point & p)
{
  return (_function_ptr->evaluate<Real>(t, p + _direction) - _function_ptr->evaluate<Real>(t, p - _direction))/_len;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
