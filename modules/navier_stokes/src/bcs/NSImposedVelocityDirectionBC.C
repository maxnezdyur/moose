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
#include "NSImposedVelocityDirectionBC.h"
#include "NS.h"

// MOOSE includes
#include "MooseMesh.h"

// Full specialization of the validParams function for this object
registerMooseObject("NavierStokesApp", NSImposedVelocityDirectionBC);

InputParameters
NSImposedVelocityDirectionBC::validParams()
{
  // Initialize the params object from the base class
  InputParameters params = NodalBC::validParams();

  params.addClassDescription("This class imposes a velocity direction component as a Dirichlet "
                             "condition on the appropriate momentum equation.");
  // Coupled variables
  params.addRequiredCoupledVar(NS::density, "density");
  params.addRequiredCoupledVar(NS::velocity_x, "x-velocity");
  params.addCoupledVar(NS::velocity_y, "y-velocity"); // only required in >= 2D
  params.addCoupledVar(NS::velocity_z, "z-velocity"); // only required in 3D
=======
#include "NSImposedVelocityDirectionBC.h"

// Full specialization of the validParams function for this object
template<>
InputParameters validParams<NSImposedVelocityDirectionBC>()
{
  // Initialize the params object from the base class
  InputParameters params = validParams<NodalBC>();

  // Coupled variables
  params.addRequiredCoupledVar("rho", "");
  params.addRequiredCoupledVar("u", "");
  params.addRequiredCoupledVar("v", "");
  params.addCoupledVar("w", ""); // only required in 3D
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  // Coupled parameters
  params.addRequiredParam<Real>("desired_unit_velocity_component", "");

  return params;
}

<<<<<<< HEAD
NSImposedVelocityDirectionBC::NSImposedVelocityDirectionBC(const InputParameters & parameters)
  : NodalBC(parameters),
    _rho(coupledValue(NS::density)),
    _u_vel(coupledValue(NS::velocity_x)),
    _v_vel(_mesh.dimension() == 2 ? coupledValue(NS::velocity_y) : _zero),
    _w_vel(_mesh.dimension() == 3 ? coupledValue(NS::velocity_z) : _zero),
    _desired_unit_velocity_component(getParam<Real>("desired_unit_velocity_component"))
{
}

Real
NSImposedVelocityDirectionBC::computeQpResidual()
=======



// Constructor, be sure to call the base class constructor first!
NSImposedVelocityDirectionBC::NSImposedVelocityDirectionBC(const std::string & name, InputParameters parameters)
    : NodalBC(name, parameters),

      // Coupled variables
      _rho(coupledValue("rho")),
      _u_vel(coupledValue("u")),
      _v_vel(coupledValue("v")),
      _w_vel(_mesh.dimension() == 3 ? coupledValue("w") : _zero),

      _desired_unit_velocity_component(getParam<Real>("desired_unit_velocity_component"))
{}



// Specialization of the computeQpResidual function for this class
Real NSImposedVelocityDirectionBC::computeQpResidual()
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
  // The velocity vector
  RealVectorValue vel(_u_vel[_qp], _v_vel[_qp], _w_vel[_qp]);

  // Specify desired velocity component
<<<<<<< HEAD
  return _u[_qp] - _rho[_qp] * _desired_unit_velocity_component * vel.norm();
}
=======
  return _u[_qp] - _rho[_qp] * _desired_unit_velocity_component * vel.size();
}

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
