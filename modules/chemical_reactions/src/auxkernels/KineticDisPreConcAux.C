<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "KineticDisPreConcAux.h"

registerMooseObject("ChemicalReactionsApp", KineticDisPreConcAux);

InputParameters
KineticDisPreConcAux::validParams()
{
  InputParameters params = KineticDisPreRateAux::validParams();
  params.addClassDescription("Concentration of secondary kinetic species");
  return params;
}

KineticDisPreConcAux::KineticDisPreConcAux(const InputParameters & parameters)
  : KineticDisPreRateAux(parameters), _u_old(uOld())
{
}

Real
KineticDisPreConcAux::computeValue()
{
  const Real kinetic_rate = KineticDisPreRateAux::computeValue();

  Real u_new_aux = _u_old[_qp] + kinetic_rate * _dt;

  // Bound concentration for the dissolution case
  if (u_new_aux < 0.0)
    u_new_aux = 0.0;
=======
#include "KineticDisPreConcAux.h"

template<>
InputParameters validParams<KineticDisPreConcAux>()
{
  InputParameters params = validParams<AuxKernel>();

  params.addParam<Real>("log_k",0.0,"The equilibrium constant of the dissolution reaction");
  params.addRequiredParam<std::vector<Real> >("sto_v","The stochiometric coefficients of reactant species");
  params.addParam<Real>("r_area",0.1,"Specific reactive surface area in m^2/L solution");
  params.addParam<Real>("ref_kconst",6.456542e-8,"Kinetic rate constant in mol/m^2 s");
  params.addParam<Real>("e_act",2.91e4,"Activation energy, J/mol");
  params.addParam<Real>("gas_const",8.31434,"Gas constant, in J/mol K");
  params.addParam<Real>("ref_temp",298.15,"Reference temperature, K");
  params.addParam<Real>("sys_temp",298.15,"System temperature at simulation, K");

  params.addCoupledVar("v", "The list of reactant species");

  return params;
}

KineticDisPreConcAux::KineticDisPreConcAux(const std::string & name, InputParameters parameters)
  :AuxKernel(name, parameters),
   _log_k(getParam<Real>("log_k")),
   _r_area(getParam<Real>("r_area")),
   _ref_kconst(getParam<Real>("ref_kconst")),
   _e_act(getParam<Real>("e_act")),
   _gas_const(getParam<Real>("gas_const")),
   _ref_temp(getParam<Real>("ref_temp")),
   _sys_temp(getParam<Real>("sys_temp")),
   _sto_v(getParam<std::vector<Real> >("sto_v"))
{
  int n = coupledComponents("v");
  _vals.resize(n);
  for (unsigned int i=0; i<_vals.size(); ++i)
    _vals[i] = &coupledValue("v", i);
}


Real
KineticDisPreConcAux::computeValue()
{
  Real kconst = _ref_kconst*exp(-_e_act*(1/_ref_temp-1/_sys_temp)/_gas_const);
  Real omega = 1.0;

  if (_vals.size())
  {
    for (unsigned int i=0; i<_vals.size(); ++i)
    {
      if ((*_vals[i])[_qp] < 0.0) (*_vals[i])[_qp] =0.0;
      omega *= std::pow((*_vals[i])[_qp],_sto_v[i]);
    }

  }

  Real saturation_SI=omega/std::pow(10.0,_log_k);
  Real kinetic_rate=_r_area*kconst*(1.0-saturation_SI);

  if (std::abs(kinetic_rate) <= 1.0e-12)
    kinetic_rate =0.0;

  Real u_new_aux = _u_old[_qp]-kinetic_rate*_dt;

  if (u_new_aux < 0.0)  //dissolution case
    u_new_aux= 0.0;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  return u_new_aux;
}
