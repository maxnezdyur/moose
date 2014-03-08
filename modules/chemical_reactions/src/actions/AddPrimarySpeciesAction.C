<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "AddPrimarySpeciesAction.h"
#include "FEProblem.h"

registerMooseAction("ChemicalReactionsApp", AddPrimarySpeciesAction, "add_variable");

InputParameters
AddPrimarySpeciesAction::validParams()
{
  InputParameters params = AddVariableAction::validParams();
  params.addRequiredParam<std::vector<NonlinearVariableName>>(
      "primary_species", "The list of primary variables to add");
  params.addClassDescription("Adds Variables for all primary species");
  return params;
}

AddPrimarySpeciesAction::AddPrimarySpeciesAction(const InputParameters & params)
  : AddVariableAction(params),
    _vars(getParam<std::vector<NonlinearVariableName>>("primary_species")),
    _scaling(isParamValid("scaling") ? getParam<std::vector<Real>>("scaling")
                                     : std::vector<Real>(1, 1.0))
=======
#include "AddPrimarySpeciesAction.h"
#include "Parser.h"
#include "FEProblem.h"
#include "Factory.h"
#include "MooseEnum.h"

#include <sstream>
#include <stdexcept>

// libMesh includes
#include "libmesh/libmesh.h"
#include "libmesh/exodusII_io.h"
#include "libmesh/equation_systems.h"
#include "libmesh/nonlinear_implicit_system.h"
#include "libmesh/explicit_system.h"
#include "libmesh/string_to_enum.h"
#include "libmesh/fe.h"


template<>
InputParameters validParams<AddPrimarySpeciesAction>()
{
  InputParameters params = validParams<Action>();
  params.addRequiredParam<std::vector<NonlinearVariableName> >("primary_species", "The list of primary variables to add");

  return params;
}


AddPrimarySpeciesAction::AddPrimarySpeciesAction(const std::string & name, InputParameters params) :
    Action(name, params)
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
}

void
AddPrimarySpeciesAction::act()
{
<<<<<<< HEAD
  auto fe_type = AddVariableAction::feType(_pars);
  auto type = AddVariableAction::variableType(fe_type);
  auto var_params = _factory.getValidParams(type);

  var_params.applySpecificParameters(_pars, {"family", "order", "scaling"});

  for (auto & var : _vars)
    _problem->addVariable(type, var, var_params);
=======
  std::vector<NonlinearVariableName> vars = getParam<std::vector<NonlinearVariableName> >("primary_species");

  for (unsigned int i=0; i < vars.size(); i++)
  {
    FEType fe_type(Utility::string_to_enum<Order>("first"),
                   Utility::string_to_enum<FEFamily>("lagrange"));
    Real scale_factor = 1.0;
    _problem->addVariable(vars[i], fe_type, scale_factor);
  }

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
