<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ThumbIC.h"

registerMooseObject("PhaseFieldApp", ThumbIC);

InputParameters
ThumbIC::validParams()
{
  InputParameters params = InitialCondition::validParams();
  params.addClassDescription("Thumb shaped bicrystal for grain boundary mobility tests");
=======
#include "ThumbIC.h"

template<>
InputParameters validParams<ThumbIC>()
{
  InputParameters params = validParams<InitialCondition>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  params.addRequiredParam<Real>("xcoord", "The x coordinate of the circle center");
  params.addRequiredParam<Real>("width", "The y coordinate of the circle center");
  params.addRequiredParam<Real>("height", "The z coordinate of the circle center");
  params.addRequiredParam<Real>("invalue", "The variable value inside the circle");
  params.addRequiredParam<Real>("outvalue", "The variable value outside the circle");
<<<<<<< HEAD
  return params;
}

ThumbIC::ThumbIC(const InputParameters & parameters)
  : InitialCondition(parameters),
    _xcoord(parameters.get<Real>("xcoord")),
    _width(parameters.get<Real>("width")),
    _height(parameters.get<Real>("height")),
    _invalue(parameters.get<Real>("invalue")),
    _outvalue(parameters.get<Real>("outvalue"))
{
}
=======

  return params;
}

ThumbIC::ThumbIC(const std::string & name,
                               InputParameters parameters)
  :InitialCondition(name, parameters),
   _xcoord(parameters.get<Real>("xcoord")),
   _width(parameters.get<Real>("width")),
   _height(parameters.get<Real>("height")),
   _invalue(parameters.get<Real>("invalue")),
   _outvalue(parameters.get<Real>("outvalue"))
{}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

Real
ThumbIC::value(const Point & p)
{
  Real value = 0.0;

  if (p(1) > _height)
  {
    Real rad = 0.0;
<<<<<<< HEAD
    Point center(_xcoord, _height, 0.0);
    for (unsigned int i = 0; i < 2; ++i)
      rad += (p(i) - center(i)) * (p(i) - center(i));

    rad = sqrt(rad);

    if (rad <= _width / 2.0)
=======
    Point center(_xcoord,_height,0.0);
    for(unsigned int i=0; i<2; i++)
      rad += (p(i)-center(i)) * (p(i)-center(i));

    rad = sqrt(rad);

    if (rad <= _width/2.0)
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
      value = _invalue;
    else
      value = _outvalue;
  }
  else
<<<<<<< HEAD
  {
    if (p(0) > _xcoord - _width / 2.0 && p(0) < _xcoord + _width / 2.0)
      value = _invalue;
    else
      value = _outvalue;
  }

  return value;
}
=======
    if (p(0) > _xcoord - _width/2.0 && p(0) < _xcoord + _width/2.0)
      value = _invalue;
    else
      value = _outvalue;

  return value;

}

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
