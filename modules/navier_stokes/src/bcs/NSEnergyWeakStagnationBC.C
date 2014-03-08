<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "NSEnergyWeakStagnationBC.h"

// FluidProperties includes
#include "IdealGasFluidProperties.h"

registerMooseObject("NavierStokesApp", NSEnergyWeakStagnationBC);

InputParameters
NSEnergyWeakStagnationBC::validParams()
{
  InputParameters params = NSWeakStagnationBaseBC::validParams();
  params.addClassDescription("The inviscid energy BC term with specified normal flow.");
  return params;
}

NSEnergyWeakStagnationBC::NSEnergyWeakStagnationBC(const InputParameters & parameters)
  : NSWeakStagnationBaseBC(parameters)
{
}

Real
NSEnergyWeakStagnationBC::computeQpResidual()
{
  // Compute stagnation values
  Real T_s = 0.0, p_s = 0.0, rho_s = 0.0;
  staticValues(T_s, p_s, rho_s);
=======
#include "NSEnergyWeakStagnationBC.h"

template<>
InputParameters validParams<NSEnergyWeakStagnationBC>()
{
  InputParameters params = validParams<NSWeakStagnationBC>();

  return params;
}



NSEnergyWeakStagnationBC::NSEnergyWeakStagnationBC(const std::string & name, InputParameters parameters)
    : NSWeakStagnationBC(name, parameters)
{
}




Real NSEnergyWeakStagnationBC::computeQpResidual()
{
  // Compute stagnation values
  Real T_s = 0., p_s = 0., rho_s = 0.;
  this->static_values(T_s, p_s, rho_s);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  // And get velocity magnitude, squared
  Real velmag2 = this->velmag2();

  // Compute static total energy, E_s
<<<<<<< HEAD
  Real E_s = _fp.cv() * T_s + 0.5 * velmag2;
=======
  Real cv = _R / (_gamma-1.);
  Real E_s = cv*T_s + 0.5*velmag2;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  // Compute the product rho_s * H_s (H_s = static enthalpy)
  Real rhoH_s = rho_s * E_s + p_s;

  // rho_s * H_s * |u| * (s.n) * phi_i
  return rhoH_s * std::sqrt(velmag2) * this->sdotn() * _test[_i][_qp];
}

<<<<<<< HEAD
Real
NSEnergyWeakStagnationBC::computeQpJacobian()
{
  // TODO
  return 0.0;
}

Real
NSEnergyWeakStagnationBC::computeQpOffDiagJacobian(unsigned /*jvar*/)
{
  // TODO
  return 0.0;
}
=======



Real NSEnergyWeakStagnationBC::computeQpJacobian()
{
  // TODO
  return 0.;
}




Real NSEnergyWeakStagnationBC::computeQpOffDiagJacobian(unsigned /*jvar*/)
{
  // TODO
  return 0.;
}




>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
