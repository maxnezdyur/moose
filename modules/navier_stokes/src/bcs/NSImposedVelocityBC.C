<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

// Navier-Stokes includes
#include "NSImposedVelocityBC.h"
#include "NS.h"

registerMooseObject("NavierStokesApp", NSImposedVelocityBC);

InputParameters
NSImposedVelocityBC::validParams()
{
  InputParameters params = NodalBC::validParams();
  params.addClassDescription("Impose Velocity BC.");
  params.addRequiredCoupledVar(NS::density, "density");
  params.addRequiredParam<Real>("desired_velocity", "");
  return params;
}

NSImposedVelocityBC::NSImposedVelocityBC(const InputParameters & parameters)
  : NodalBC(parameters),
    _rho(coupledValue(NS::density)),
    _desired_velocity(getParam<Real>("desired_velocity"))
{
}

Real
NSImposedVelocityBC::computeQpResidual()
=======
#include "NSImposedVelocityBC.h"

// Full specialization of the validParams function for this object
template<>
InputParameters validParams<NSImposedVelocityBC>()
{
  // Initialize the params object from the base class
  InputParameters params = validParams<NodalBC>();

  // Declare variables as coupled...
  params.addRequiredCoupledVar("rho", "");

  // New style.  Note, addRequiredParam is a templated function, templated
  // on the type of the parameter being added. (Second string argument is optional documentation.)
  params.addRequiredParam<Real>("desired_velocity", "");// make it required!

  return params;
}




// Constructor, be sure to call the base class constructor first!
NSImposedVelocityBC::NSImposedVelocityBC(const std::string & name, InputParameters parameters)
  :NodalBC(name, parameters),
   _rho(coupledValue("rho")),
   _desired_velocity(getParam<Real>("desired_velocity"))
  {}



// Specialization of the computeQpResidual function for this class
Real NSImposedVelocityBC::computeQpResidual()
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
  // Return the difference between the current momentum and the desired value
  // (rho*u) - rho*desired_velocity
  return _u[_qp] - (_rho[_qp] * _desired_velocity);
}
<<<<<<< HEAD
=======

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
