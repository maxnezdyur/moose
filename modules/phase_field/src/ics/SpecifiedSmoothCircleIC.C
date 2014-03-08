<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "SpecifiedSmoothCircleIC.h"
#include "MooseRandom.h"

registerMooseObject("PhaseFieldApp", SpecifiedSmoothCircleIC);

InputParameters
SpecifiedSmoothCircleIC::validParams()
{
  InputParameters params = SmoothCircleBaseIC::validParams();
  params.addClassDescription(
      "Multiple smooth circles with manually specified radii and center points");
  params.addRequiredParam<std::vector<Real>>("x_positions",
                                             "The x-coordinate for each circle center");
  params.addRequiredParam<std::vector<Real>>("y_positions",
                                             "The y-coordinate for each circle center");
  params.addRequiredParam<std::vector<Real>>("z_positions",
                                             "The z-coordinate for each circle center");
  params.addRequiredParam<std::vector<Real>>("radii", "The radius for each circle");
=======
#include "SpecifiedSmoothCircleIC.h"
#include "MooseRandom.h"

template<>
InputParameters validParams<SpecifiedSmoothCircleIC>()
{
  InputParameters params = validParams<MultiSmoothCircleIC>();
  params.addRequiredParam<std::vector<Real> >("x_positions","The x-coordinate for each circle center");
  params.addRequiredParam<std::vector<Real> >("y_positions","The y-coordinate for each circle center");
  params.addRequiredParam<std::vector<Real> >("z_positions", "The z-coordinate for each circle center");
  params.addRequiredParam<std::vector<Real> >("radii", "The radius for each circle");
  //These are MultiSmoothCircleIC inputs that are not needed here.
  params.set<unsigned int>("numbub") = 0.0;
  params.set<Real>("bubspac") = 0.0;
  params.set<Real>("radius") = 0.0;
  params.set<Real>("Lx") = 0.0;
  params.set<Real>("Ly") = 0.0;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  return params;
}

<<<<<<< HEAD
SpecifiedSmoothCircleIC::SpecifiedSmoothCircleIC(const InputParameters & parameters)
  : SmoothCircleBaseIC(parameters),
    _x_positions(getParam<std::vector<Real>>("x_positions")),
    _y_positions(getParam<std::vector<Real>>("y_positions")),
    _z_positions(getParam<std::vector<Real>>("z_positions")),
    _input_radii(getParam<std::vector<Real>>("radii"))
=======
SpecifiedSmoothCircleIC::SpecifiedSmoothCircleIC(const std::string & name,
                                               InputParameters parameters)
  :MultiSmoothCircleIC(name, parameters),
   _x_positions(getParam<std::vector<Real> >("x_positions")),
   _y_positions(getParam<std::vector<Real> >("y_positions")),
   _z_positions(getParam<std::vector<Real> >("z_positions")),
   _radii(getParam<std::vector<Real> >("radii"))
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
}

void
<<<<<<< HEAD
SpecifiedSmoothCircleIC::computeCircleRadii()
{
  _radii.resize(_input_radii.size());

  for (unsigned int circ = 0; circ < _input_radii.size(); ++circ)
    _radii[circ] = _input_radii[circ];
}

void
SpecifiedSmoothCircleIC::computeCircleCenters()
{
  _centers.resize(_x_positions.size());

  for (unsigned int circ = 0; circ < _x_positions.size(); ++circ)
  {
    _centers[circ](0) = _x_positions[circ];
    _centers[circ](1) = _y_positions[circ];
    _centers[circ](2) = _z_positions[circ];
  }
=======
SpecifiedSmoothCircleIC::initialSetup()
{
  MultiSmoothCircleIC::initialSetup();

  unsigned int y_size, z_size, radii_size;
  _numbub = _x_positions.size();
  //Moose::out << "check 1" << "\n";
  y_size = _y_positions.size();
  z_size = _z_positions.size();
  radii_size = _radii.size();

  // check to make sure the input file is set up correctly
  if((_numbub != y_size)||(_numbub != z_size)||(_numbub != radii_size))
    mooseError("Please match the number of radii to the size of the position vectors.");

  //Moose::out << "check 2" << "\n";

  //resize the vector of Points
   _bubcent.resize(_numbub);
   _bubradi.resize(_numbub);

  // fill in the vector of center points
  for(unsigned int i=0; i<_numbub; i++)
  {
    _bubcent[i](0) = _x_positions[i];
    _bubcent[i](1) = _y_positions[i];
    _bubcent[i](2) = _z_positions[i];
    _bubradi[i] = _radii[i];
  }
  //Moose::out << "check 3" << "\n";
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
