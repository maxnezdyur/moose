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
#include "NSTemperatureAux.h"
#include "NS.h"

// FluidProperties includes
#include "IdealGasFluidProperties.h"

// MOOSE includes
#include "MooseMesh.h"

registerMooseObject("NavierStokesApp", NSTemperatureAux);

InputParameters
NSTemperatureAux::validParams()
{
  InputParameters params = AuxKernel::validParams();

  params.addClassDescription("Temperature is an auxiliary value computed from the total energy "
                             "based on the FluidProperties.");
  // Mark variables as required
  params.addRequiredCoupledVar(NS::specific_volume, "specific volume");
  params.addRequiredCoupledVar(NS::specific_internal_energy, "internal energy");
  params.addRequiredParam<UserObjectName>("fluid_properties",
                                          "The name of the user object for fluid properties");
=======
#include "NSTemperatureAux.h"

template<>
InputParameters validParams<NSTemperatureAux>()
{
  InputParameters params = validParams<AuxKernel>();

  // Mark variables as required
  params.addRequiredCoupledVar("rho", "");
  params.addRequiredCoupledVar("u", "");
  params.addRequiredCoupledVar("v", "");
  params.addCoupledVar("w", ""); // Only required in 3D...
  params.addRequiredCoupledVar("rhoe", "");

  // Parameters with default values
  params.addRequiredParam<Real>("R", "Gas constant.");
  params.addRequiredParam<Real>("gamma", "Ratio of specific heats.");
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  return params;
}

<<<<<<< HEAD
NSTemperatureAux::NSTemperatureAux(const InputParameters & parameters)
  : AuxKernel(parameters),
    _specific_volume(coupledValue(NS::specific_volume)),
    _specific_internal_energy(coupledValue(NS::specific_internal_energy)),
    _fp(getUserObject<IdealGasFluidProperties>("fluid_properties"))
{
  mooseDeprecated("The NSTemperatureAux aux kernel has been replaced by the "
                  "TemperatureAux kernel in the fluid properties module.");
}
=======


NSTemperatureAux::NSTemperatureAux(const std::string & name, InputParameters parameters)
    : AuxKernel(name, parameters),
      _rho(coupledValue("rho")),
      _u_vel(coupledValue("u")),
      _v_vel(coupledValue("v")),
      _w_vel(_mesh.dimension() == 3 ? coupledValue("w") : _zero),
      _rhoe(coupledValue("rhoe")),
      _R(getParam<Real>("R")),
      _gamma(getParam<Real>("gamma"))
{}

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

Real
NSTemperatureAux::computeValue()
{
<<<<<<< HEAD
  return _fp.T_from_v_e(_specific_volume[_qp], _specific_internal_energy[_qp]);
=======
  Real V2 =
    _u_vel[_qp]*_u_vel[_qp] +
    _v_vel[_qp]*_v_vel[_qp] +
    _w_vel[_qp]*_w_vel[_qp];

  // Internal Energy = Total Energy - Kinetic
  Real e_i = (_rhoe[_qp] / _rho[_qp]) - 0.5*V2;

  // T = e_i / cv
  Real cv = _R / (_gamma-1.);
  return e_i / cv;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
