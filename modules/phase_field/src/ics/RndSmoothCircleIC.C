<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "RndSmoothCircleIC.h"

// MOOSE includes
#include "MooseMesh.h"
#include "MooseVariable.h"

registerMooseObject("PhaseFieldApp", RndSmoothCircleIC);

InputParameters
RndSmoothCircleIC::validParams()
{
  InputParameters params = SmoothCircleIC::validParams();
  params.addClassDescription(
      "Random noise with different min/max inside/outside of a smooth circle");
  params.addRequiredParam<Real>("variation_invalue", "Plus or minus this amount on the invalue");
  params.addRequiredParam<Real>("variation_outvalue", "Plus or minus this amount on the outvalue");
  return params;
}

RndSmoothCircleIC::RndSmoothCircleIC(const InputParameters & parameters)
  : SmoothCircleIC(parameters),
    _variation_invalue(parameters.get<Real>("variation_invalue")),
    _variation_outvalue(parameters.get<Real>("variation_outvalue"))
{
  if (_profile == ProfileType::TANH)
    paramError("profile", "Hyperbolic tangent profile is not supported for this IC");
}

Real
RndSmoothCircleIC::computeCircleValue(const Point & p, const Point & center, const Real & radius)
{
  Point l_center = center;
  Point l_p = p;
  if (!_3D_spheres) // Create 3D cylinders instead of spheres
  {
    l_p(2) = 0.0;
    l_center(2) = 0.0;
  }
  // Compute the distance between the current point and the center
  Real dist = _mesh.minPeriodicDistance(_var.number(), l_p, l_center);

  // Return value
  Real value = 0.0;

  if (dist <= radius - _int_width / 2.0) // Random value inside circle
    value = _invalue - _variation_invalue + 2.0 * _random.rand(_tid) * _variation_invalue;
  else if (dist < radius + _int_width / 2.0) // Smooth interface
  {
    Real int_pos = (dist - radius + _int_width / 2.0) / _int_width;
    value = _outvalue + (_invalue - _outvalue) * (1.0 + std::cos(int_pos * libMesh::pi)) / 2.0;
  }
  else // Random value outside circle
    value = _outvalue - _variation_outvalue + 2.0 * _random.rand(_tid) * _variation_outvalue;

  return value;
}
=======
#include "RndSmoothCircleIC.h"
#include "MooseRandom.h"

template<>
InputParameters validParams<RndSmoothCircleIC>()
{
  InputParameters params = validParams<InitialCondition>();
  params.addRequiredParam<Real>("x1", "The x coordinate of the circle center");
  params.addRequiredParam<Real>("y1", "The y coordinate of the circle center");
  params.addParam<Real>("z1", 0.0, "The z coordinate of the circle center");

  params.addRequiredParam<Real>("mx_invalue", "The max variable value inside the circle");
  params.addRequiredParam<Real>("mx_outvalue", "The max variable value outside the circle");
  params.addRequiredParam<Real>("mn_invalue", "The min variable value inside the circle");
  params.addRequiredParam<Real>("mn_outvalue", "The min variable value outside the circle");
  params.addRequiredParam<Real>("radius", "The radius of a circle");

  params.addParam<unsigned int>("seed", 12345, "Seed value for the random number generator");
  return params;
}

RndSmoothCircleIC::RndSmoothCircleIC(const std::string & name,
                               InputParameters parameters)
  :InitialCondition(name, parameters),
   _x1(parameters.get<Real>("x1")),
   _y1(parameters.get<Real>("y1")),
   _z1(parameters.get<Real>("z1")),
   _mx_invalue(parameters.get<Real>("mx_invalue")),
   _mn_invalue(parameters.get<Real>("mn_invalue")),
   _mx_outvalue(parameters.get<Real>("mx_outvalue")),
   _mn_outvalue(parameters.get<Real>("mn_outvalue")),
   _range_invalue(_mx_invalue - _mn_invalue),
   _range_outvalue(_mx_outvalue - _mn_outvalue),
   _radius(parameters.get<Real>("radius")),
   _center(_x1,_y1,_z1)
{
  mooseAssert(_range_invalue >= 0.0, "Inside Min > Max for RndSmoothCircleIC!");
  mooseAssert(_range_outvalue >= 0.0, "Outside Min > Max for RndSmoothCircleIC!");
  MooseRandom::seed(getParam<unsigned int>("seed"));
}

Real
RndSmoothCircleIC::value(const Point & p)
{
  Real value = 0.0;

  Real rad = 0.0;

  for(unsigned int i=0; i<LIBMESH_DIM; i++)
    rad += (p(i)-_center(i)) * (p(i)-_center(i));

  rad = sqrt(rad);

  //Random number between 0 and 1
  Real rand_num = MooseRandom::rand();

  if (rad <= _radius)
    value = _mn_invalue + rand_num*(_range_invalue);
  else if (rad < 1.5*_radius)
  {
    Real av_invalue = (_mn_invalue + _mx_invalue)/2.0;
    Real av_outvalue = (_mn_outvalue + _mx_outvalue)/2.0;
    value = av_outvalue + (av_invalue-av_outvalue)*(1+cos((rad-_radius)/_radius*2.0*3.14159))/2.0;
  }
  else
    value = _mx_outvalue + rand_num*(_range_outvalue);

  return value;

}





>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
