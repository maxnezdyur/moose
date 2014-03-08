<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
=======
/*****************************************/
/* Written by andrew.wilkins@csiro.au    */
/* Please contact me if you make changes */
/*****************************************/
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

//  This post processor returns the mass value of a region.  It is used
//  to check that mass is conserved
//
#include "RichardsMass.h"

<<<<<<< HEAD
registerMooseObject("RichardsApp", RichardsMass);

InputParameters
RichardsMass::validParams()
{
  InputParameters params = ElementIntegralVariablePostprocessor::validParams();
  params.addRequiredParam<UserObjectName>(
      "richardsVarNames_UO", "The UserObject that holds the list of Richards variable names.");
=======
template<>
InputParameters validParams<RichardsMass>()
{
  InputParameters params = validParams<ElementIntegralVariablePostprocessor>();
  params.addRequiredParam<UserObjectName>("porepressureNames_UO", "The UserObject that holds the list of porepressure names.");
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  params.addClassDescription("Returns the mass in a region.");
  return params;
}

<<<<<<< HEAD
RichardsMass::RichardsMass(const InputParameters & parameters)
  : ElementIntegralVariablePostprocessor(parameters),

    _richards_name_UO(getUserObject<RichardsVarNames>("richardsVarNames_UO")),
    _pvar(_richards_name_UO.richards_var_num(coupled("variable"))),

    _mass(getMaterialProperty<std::vector<Real>>("mass"))
=======
RichardsMass::RichardsMass(const std::string & name, InputParameters parameters) :
    ElementIntegralVariablePostprocessor(name, parameters),

    _pp_name_UO(getUserObject<RichardsPorepressureNames>("porepressureNames_UO")),
    _pvar(_pp_name_UO.pressure_var_num(_var.index())),

    _porosity(getMaterialProperty<Real>("porosity")),
    _sat(getMaterialProperty<std::vector<Real> >("sat")),
    _density(getMaterialProperty<std::vector<Real> >("density"))
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
}

Real
RichardsMass::computeQpIntegral()
{
<<<<<<< HEAD
  return _mass[_qp][_pvar];
=======
  return _porosity[_qp]*_density[_qp][_pvar]*_sat[_qp][_pvar];
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
