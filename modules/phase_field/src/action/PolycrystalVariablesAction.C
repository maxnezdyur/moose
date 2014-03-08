<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PolycrystalVariablesAction.h"
#include "AddVariableAction.h"
#include "Conversion.h"
#include "Factory.h"
#include "FEProblem.h"
#include "NonlinearSystemBase.h"

#include "libmesh/string_to_enum.h"

registerMooseAction("PhaseFieldApp", PolycrystalVariablesAction, "add_variable");
registerMooseAction("PhaseFieldApp", PolycrystalVariablesAction, "copy_nodal_vars");
registerMooseAction("PhaseFieldApp", PolycrystalVariablesAction, "check_copy_nodal_vars");

InputParameters
PolycrystalVariablesAction::validParams()
{
  InputParameters params = Action::validParams();
  params.addClassDescription("Set up order parameter variables for a polycrystal simulation");
  // Get MooseEnums for the possible order/family options for this variable
  MooseEnum families(AddVariableAction::getNonlinearVariableFamilies());
  MooseEnum orders(AddVariableAction::getNonlinearVariableOrders());
  params.addParam<MooseEnum>("family",
                             families,
                             "Specifies the family of FE "
                             "shape function to use for the order parameters");
  params.addParam<MooseEnum>("order",
                             orders,
                             "Specifies the order of the FE "
                             "shape function to use for the order parameters");
  params.addParam<bool>(
      "initial_from_file",
      false,
      "Take the initial condition of all polycrystal variables from the mesh file");
  params.addParam<Real>("scaling", 1.0, "Specifies a scaling factor to apply to this variable");
  params.addRequiredParam<unsigned int>("op_num",
                                        "specifies the number of order parameters to create");
  params.addRequiredParam<std::string>("var_name_base", "specifies the base name of the variables");
  return params;
}

PolycrystalVariablesAction::PolycrystalVariablesAction(const InputParameters & params)
  : Action(params),
    _op_num(getParam<unsigned int>("op_num")),
    _var_name_base(getParam<std::string>("var_name_base"))
{
}
=======
#include "PolycrystalVariablesAction.h"
#include "Factory.h"
#include "Parser.h"
#include "FEProblem.h"

#include <sstream>
#include <stdexcept>

// libMesh includes
#include "libmesh/libmesh.h"
#include "libmesh/exodusII_io.h"
#include "libmesh/equation_systems.h"
#include "libmesh/nonlinear_implicit_system.h"
#include "libmesh/explicit_system.h"
#include "libmesh/string_to_enum.h"

const Real PolycrystalVariablesAction::_abs_zero_tol = 1e-12;

template<>
InputParameters validParams<PolycrystalVariablesAction>()
{
  InputParameters params = validParams<Action>();
  params.addParam<std::string>("family", "LAGRANGE", "Specifies the family of FE shape functions to use for this variable");
  params.addParam<std::string>("order", "FIRST",  "Specifies the order of the FE shape function to use for this variable");
  params.addParam<Real>("scaling", 1.0, "Specifies a scaling factor to apply to this variable");
  params.addRequiredParam<unsigned int>("crys_num", "specifies the number of order parameters to create");
  params.addRequiredParam<std::string>("var_name_base","specifies the base name of the variables");

  return params;
}

PolycrystalVariablesAction::PolycrystalVariablesAction(const std::string & name, InputParameters params)
  :Action(name, params),
   _crys_num(getParam<unsigned int>("crys_num")),
   _var_name_base(getParam<std::string>("var_name_base"))
{}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

void
PolycrystalVariablesAction::act()
{
<<<<<<< HEAD
  // take initial values from file?
  bool initial_from_file = getParam<bool>("initial_from_file");

  // Loop through the number of order parameters
  for (unsigned int op = 0; op < _op_num; op++)
  {
    // Create variable names
    std::string var_name = _var_name_base + Moose::stringify(op);

    // Add the variable
    if (_current_task == "add_variable")
    {
      auto fe_type = AddVariableAction::feType(_pars);
      auto type = AddVariableAction::variableType(fe_type);
      auto var_params = _factory.getValidParams(type);

      var_params.applySpecificParameters(_pars, {"order", "family"});
      var_params.set<std::vector<Real>>("scaling") = {_pars.get<Real>("scaling")};
      _problem->addVariable(type, var_name, var_params);
    }

    // Setup initial from file if requested
    if (initial_from_file)
    {
      if (_current_task == "check_copy_nodal_vars")
        _app.setExodusFileRestart(true);

      if (_current_task == "copy_nodal_vars")
      {
        auto * system = &_problem->getNonlinearSystemBase();
        system->addVariableToCopy(var_name, var_name, "LATEST");
      }
    }
=======
#ifdef DEBUG
  Moose::err << "Inside the PolycrystalVariablesAction Object\n";
  Moose::err << "VariableBase: " << _var_name_base
            << "\torder: " << getParam<std::string>("order")
            << "\tfamily: " << getParam<std::string>("family") << std::endl;
#endif

// Loop through the number of order parameters

  for (unsigned int crys = 0; crys<_crys_num; crys++)
  {
    //Create variable names
    std::string var_name = _var_name_base;
    std::stringstream out;
    out << crys;
    var_name.append(out.str());

    _problem->addVariable(var_name,
                          FEType(Utility::string_to_enum<Order>(getParam<std::string>("order")),
                                 Utility::string_to_enum<FEFamily>(getParam<std::string>("family"))),
                          getParam<Real>("scaling"));
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  }
}
