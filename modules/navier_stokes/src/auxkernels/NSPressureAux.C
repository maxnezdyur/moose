<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

// Navier-Stokes inclues
#include "NSPressureAux.h"
#include "NS.h"

// FluidProperties includes
#include "IdealGasFluidProperties.h"

// MOOSE includes
#include "MooseMesh.h"

registerMooseObject("NavierStokesApp", NSPressureAux);

InputParameters
NSPressureAux::validParams()
{
  InputParameters params = AuxKernel::validParams();

  params.addClassDescription("Nodal auxiliary variable, for computing pressure at the nodes.");
  // Mark variables as required
  params.addRequiredCoupledVar(NS::specific_volume, "specific volume");
  params.addRequiredCoupledVar(NS::specific_internal_energy, "internal energy");
  params.addRequiredParam<UserObjectName>("fluid_properties",
                                          "The name of the user object for fluid properties");
=======
#include "NSPressureAux.h"

template<>
InputParameters validParams<NSPressureAux>()
{
  InputParameters params = validParams<AuxKernel>();

  // Mark variables as required
  params.addRequiredCoupledVar("rho", "");
  params.addRequiredCoupledVar("u", "");
  params.addRequiredCoupledVar("v", "");
  params.addCoupledVar("w", ""); // Only required in 3D...
  params.addRequiredCoupledVar("rhoe", "");

  // Parameters with default values
  params.addRequiredParam<Real>("gamma", "Ratio of specific heats");
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  return params;
}

<<<<<<< HEAD
NSPressureAux::NSPressureAux(const InputParameters & parameters)
  : AuxKernel(parameters),
    _specific_volume(coupledValue(NS::specific_volume)),
    _specific_internal_energy(coupledValue(NS::specific_internal_energy)),
    _fp(getUserObject<IdealGasFluidProperties>("fluid_properties"))
{
  mooseDeprecated("The NSPressureAux aux kernel has been replaced by the "
                  "PressureAux kernel in the fluid properties module.");
}
=======
NSPressureAux::NSPressureAux(const std::string & name, InputParameters parameters)
  :AuxKernel(name, parameters),
   _rho(coupledValue("rho")),
   _u_vel(coupledValue("u")),
   _v_vel(coupledValue("v")),
   _w_vel(_mesh.dimension() == 3 ? coupledValue("w") : _zero),
   _rhoe(coupledValue("rhoe")),
   _gamma(getParam<Real>("gamma")) // can't use Material properties in Nodal Aux...
{}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

Real
NSPressureAux::computeValue()
{
<<<<<<< HEAD
  return _fp.p_from_v_e(_specific_volume[_qp], _specific_internal_energy[_qp]);
=======
  // Velocity vector, squared
  Real V2 = _u_vel[_qp]*_u_vel[_qp] + _v_vel[_qp]*_v_vel[_qp] + _w_vel[_qp]*_w_vel[_qp];

  // P = (gam-1) * ( rho*e_t - 1/2 * rho * V^2)
  return (_gamma - 1)*(_rhoe[_qp] - 0.5 * _rho[_qp] * V2);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
