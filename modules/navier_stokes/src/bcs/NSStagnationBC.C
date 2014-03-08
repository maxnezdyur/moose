<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "NSStagnationBC.h"

// FluidProperties includes
#include "IdealGasFluidProperties.h"

// MOOSE includes
#include "MooseMesh.h"

InputParameters
NSStagnationBC::validParams()
{
  InputParameters params = NodalBC::validParams();
  params.addClassDescription(
      "This is the base class for the 'imposed stagnation' value boundary conditions.");
  params.addRequiredCoupledVar("mach", "Mach number");
  params.addRequiredParam<UserObjectName>("fluid_properties",
                                          "The name of the user object for fluid properties");
  return params;
}

NSStagnationBC::NSStagnationBC(const InputParameters & parameters)
  : NodalBC(parameters),
    _mach(coupledValue("mach")),
    _fp(getUserObject<IdealGasFluidProperties>("fluid_properties"))
{
}
=======
#include "NSStagnationBC.h"

// Full specialization of the validParams function for this object
template<>
InputParameters validParams<NSStagnationBC>()
{
  // Initialize the params object from the base class
  InputParameters params = validParams<NodalBC>();

  // Declare variables as coupled...
  params.addRequiredCoupledVar("u", "");
  params.addRequiredCoupledVar("v", "");
  params.addCoupledVar("w", ""); // only required in 3D
  params.addRequiredCoupledVar("temperature", "");

  // Required parameters
  params.addRequiredParam<Real>("gamma", "Ratio of specific heats");
  params.addRequiredParam<Real>("R", "Gas constant.");

  return params;
}




// Constructor, be sure to call the base class constructor first!
NSStagnationBC::NSStagnationBC(const std::string & name, InputParameters parameters)
    : NodalBC(name, parameters),

      // Coupled variables
      _u_vel(coupledValue("u")),
      _v_vel(coupledValue("v")),
      _w_vel(_mesh.dimension() == 3 ? coupledValue("w") : _zero),

      _temperature(coupledValue("temperature")),

      // Required parameters
      _gamma(getParam<Real>("gamma")),
      _R(getParam<Real>("R"))
{}



>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
