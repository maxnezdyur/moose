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
#include "NS.h"
#include "NSIntegratedBC.h"

// FluidProperties includes
#include "IdealGasFluidProperties.h"

// MOOSE includes
#include "MooseMesh.h"

InputParameters
NSIntegratedBC::validParams()
{
  InputParameters params = IntegratedBC::validParams();

  params.addClassDescription("This class couples together all the variables for the compressible "
                             "Navier-Stokes equations to allow them to be used in derived "
                             "IntegratedBC classes.");

  params.addRequiredCoupledVar(NS::velocity_x, "x-velocity");
  params.addCoupledVar(NS::velocity_y, "y-velocity"); // only required in >= 2D
  params.addCoupledVar(NS::velocity_z, "z-velocity"); // only required in 3D

  params.addRequiredCoupledVar(NS::density, "density");
  params.addRequiredCoupledVar(NS::momentum_x, "x-momentum");
  params.addCoupledVar(NS::momentum_y, "y-momentum"); // only required in >= 2D
  params.addCoupledVar(NS::momentum_z, "z-momentum"); // only required in 3D
  params.addRequiredCoupledVar(NS::total_energy_density, "total energy");
  params.addRequiredParam<UserObjectName>("fluid_properties",
                                          "The name of the user object for fluid properties");
  params.addParam<Real>("specified_pressure", 0.0, "The specified pressure for this boundary");
=======
#include "NSIntegratedBC.h"

template<>
InputParameters validParams<NSIntegratedBC>()
{
  InputParameters params = validParams<IntegratedBC>();

  // Coupled variables
  params.addRequiredCoupledVar("u", "");
  params.addRequiredCoupledVar("v", "");
  params.addCoupledVar("w", ""); // only required in 3D

  params.addRequiredCoupledVar("rho", "density");
  params.addRequiredCoupledVar("rhou", "x-momentum");
  params.addRequiredCoupledVar("rhov", "y-momentum");
  params.addCoupledVar("rhow", "z-momentum"); // only required in 3D
  params.addRequiredCoupledVar("rhoe", "energy");

  // Required parameters
  params.addRequiredParam<Real>("gamma", "Ratio of specific heats.");
  params.addRequiredParam<Real>("R", "Gas constant.");
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  return params;
}

<<<<<<< HEAD
NSIntegratedBC::NSIntegratedBC(const InputParameters & parameters)
  : IntegratedBC(parameters),
    _u_vel(coupledValue(NS::velocity_x)),
    _v_vel(_mesh.dimension() >= 2 ? coupledValue(NS::velocity_y) : _zero),
    _w_vel(_mesh.dimension() == 3 ? coupledValue(NS::velocity_z) : _zero),

    _rho(coupledValue(NS::density)),
    _rho_u(coupledValue(NS::momentum_x)),
    _rho_v(_mesh.dimension() >= 2 ? coupledValue(NS::momentum_y) : _zero),
    _rho_w(_mesh.dimension() == 3 ? coupledValue(NS::momentum_z) : _zero),
    _rho_et(coupledValue(NS::total_energy_density)),

    _grad_rho(coupledGradient(NS::density)),
    _grad_rho_u(coupledGradient(NS::momentum_x)),
    _grad_rho_v(_mesh.dimension() >= 2 ? coupledGradient(NS::momentum_y) : _grad_zero),
    _grad_rho_w(_mesh.dimension() == 3 ? coupledGradient(NS::momentum_z) : _grad_zero),
    _grad_rho_et(coupledGradient(NS::total_energy_density)),

    // Variable numberings
    _rho_var_number(coupled(NS::density)),
    _rhou_var_number(coupled(NS::momentum_x)),
    _rhov_var_number(_mesh.dimension() >= 2 ? coupled(NS::momentum_y) : libMesh::invalid_uint),
    _rhow_var_number(_mesh.dimension() == 3 ? coupled(NS::momentum_z) : libMesh::invalid_uint),
    _rho_et_var_number(coupled(NS::total_energy_density)),

    _dynamic_viscosity(getMaterialProperty<Real>("dynamic_viscosity")),
    _viscous_stress_tensor(getMaterialProperty<RealTensorValue>("viscous_stress_tensor")),

    // FluidProperties UserObject
    _fp(getUserObject<IdealGasFluidProperties>("fluid_properties"))
{
}

bool
NSIntegratedBC::isNSVariable(unsigned var)
{
  if (var == _rho_var_number || var == _rhou_var_number || var == _rhov_var_number ||
      var == _rhow_var_number || var == _rho_et_var_number)
    return true;
  else
    return false;
}

unsigned
NSIntegratedBC::mapVarNumber(unsigned var)
=======


NSIntegratedBC::NSIntegratedBC(const std::string & name, InputParameters parameters)
    : IntegratedBC(name, parameters),

      // Coupled variables
      _u_vel(coupledValue("u")),
      _v_vel(coupledValue("v")),
      _w_vel(_mesh.dimension() == 3 ? coupledValue("w") : _zero),

      _rho(coupledValue("rho")),
      _rho_u(coupledValue("rhou")),
      _rho_v(coupledValue("rhov")),
      _rho_w(_mesh.dimension() == 3 ? coupledValue("rhow") : _zero),
      _rho_e(coupledValue("rhoe")),

      // Gradients
      _grad_rho(coupledGradient("rho")),
      _grad_rho_u(coupledGradient("rhou")),
      _grad_rho_v(coupledGradient("rhov")),
      _grad_rho_w(_mesh.dimension() == 3 ? coupledGradient("rhow") : _grad_zero),
      _grad_rho_e(coupledGradient("rhoe")),

      // Variable numberings
      _rho_var_number( coupled("rho") ),
      _rhou_var_number( coupled("rhou") ),
      _rhov_var_number( coupled("rhov") ),
      _rhow_var_number(_mesh.dimension() == 3 ? coupled("rhow") : libMesh::invalid_uint),
      _rhoe_var_number( coupled("rhoe") ),

      // Material properties
      _dynamic_viscosity(getMaterialProperty<Real>("dynamic_viscosity")),
      _viscous_stress_tensor(getMaterialProperty<RealTensorValue>("viscous_stress_tensor")),

      // Required parameters
      _gamma(getParam<Real>("gamma")),
      _R(getParam<Real>("R"))
{
}



unsigned NSIntegratedBC::map_var_number(unsigned var)
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
  // Convert the Moose numbering to:
  // 0 for rho
  // 1 for rho*u
  // 2 for rho*v
  // 3 for rho*w
  // 4 for rho*e
  // regardless of the problem dimension, etc.
<<<<<<< HEAD
  unsigned int mapped_var_number;

  if (var == _rho_var_number)
    mapped_var_number = 0;
  else if (var == _rhou_var_number)
    mapped_var_number = 1;
  else if (var == _rhov_var_number)
    mapped_var_number = 2;
  else if (var == _rhow_var_number)
    mapped_var_number = 3;
  else if (var == _rho_et_var_number)
    mapped_var_number = 4;
  else
    mooseError("Invalid var!");
=======
  unsigned mapped_var_number = 99;

  if (var == _rho_var_number)       mapped_var_number = 0;
  else if (var == _rhou_var_number) mapped_var_number = 1;
  else if (var == _rhov_var_number) mapped_var_number = 2;
  else if (var == _rhow_var_number) mapped_var_number = 3;
  else if (var == _rhoe_var_number) mapped_var_number = 4;
  else mooseError("Invalid var!");
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  return mapped_var_number;
}
