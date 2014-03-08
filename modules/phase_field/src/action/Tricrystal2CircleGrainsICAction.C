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
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
#include "Tricrystal2CircleGrainsICAction.h"
#include "Factory.h"
#include "Parser.h"
#include "FEProblem.h"
<<<<<<< HEAD
#include "Conversion.h"
=======
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include <sstream>
#include <stdexcept>

<<<<<<< HEAD
=======
// libMesh includes
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
#include "libmesh/libmesh.h"
#include "libmesh/exodusII_io.h"
#include "libmesh/equation_systems.h"
#include "libmesh/nonlinear_implicit_system.h"
#include "libmesh/explicit_system.h"
#include "libmesh/string_to_enum.h"

const Real Tricrystal2CircleGrainsICAction::_abs_zero_tol = 1e-12;

<<<<<<< HEAD
registerMooseAction("PhaseFieldApp", Tricrystal2CircleGrainsICAction, "add_ic");

InputParameters
Tricrystal2CircleGrainsICAction::validParams()
{
  InputParameters params = Action::validParams();
  params.addRequiredParam<unsigned int>("op_num", "number of order parameters to create");
  params.addRequiredParam<std::string>("var_name_base", "specifies the base name of the variables");
  params.addParam<std::vector<SubdomainName>>("block",
                                              "Block restriction for the initial condition");
=======
template<>
InputParameters validParams<Tricrystal2CircleGrainsICAction>()
{
  InputParameters params = validParams<Action>();
  params.addRequiredParam<unsigned int>("crys_num", "number of order parameters to create");
  params.addRequiredParam<std::string>("var_name_base","specifies the base name of the variables");
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  return params;
}

<<<<<<< HEAD
Tricrystal2CircleGrainsICAction::Tricrystal2CircleGrainsICAction(const InputParameters & params)
  : Action(params),
    _var_name_base(getParam<std::string>("var_name_base")),
    _op_num(getParam<unsigned int>("op_num"))
{
}
=======
Tricrystal2CircleGrainsICAction::Tricrystal2CircleGrainsICAction(const std::string & name, InputParameters params)
  :Action(name, params),
   _var_name_base(getParam<std::string>("var_name_base")),
   _crys_num(getParam<unsigned int>("crys_num"))
{}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

void
Tricrystal2CircleGrainsICAction::act()
{
#ifdef DEBUG
<<<<<<< HEAD
  Moose::err << "Inside the Tricrystal2CircleGrainsICAction Object" << std::endl;
#endif

  // Loop through the number of order parameters
  for (unsigned int op = 0; op < _op_num; op++)
  {
    // Create variable names
    std::string var_name = _var_name_base;
    std::stringstream out;
    out << op;
    var_name.append(out.str());

    // Set parameters for BoundingBoxIC
    InputParameters poly_params = _factory.getValidParams("Tricrystal2CircleGrainsIC");
    poly_params.set<VariableName>("variable") = var_name;
    poly_params.set<unsigned int>("op_num") = _op_num;
    poly_params.set<unsigned int>("op_index") = op;
    if (isParamValid("block"))
      poly_params.set<std::vector<SubdomainName>>("block") =
          getParam<std::vector<SubdomainName>>("block");

    // Add initial condition
    _problem->addInitialCondition("Tricrystal2CircleGrainsIC",
                                  "Tricrystal2CircleGrainsIC_" + Moose::stringify(op),
                                  poly_params);
  }
=======
  Moose::err << "Inside the Tricrystal2CircleGrainsICAction Object\n";
#endif

// Loop through the number of order parameters


  for (unsigned int crys = 0; crys<_crys_num; crys++)
  {
    //Create variable names
    std::string var_name = _var_name_base;
    std::stringstream out;
    out << crys;
    var_name.append(out.str());

    //Set parameters for BoundingBoxIC
    InputParameters poly_params = _factory.getValidParams("Tricrystal2CircleGrainsIC");
    poly_params.set<VariableName>("variable") = var_name;
    poly_params.set<unsigned int>("crys_num") = _crys_num;
    poly_params.set<unsigned int>("crys_index") = crys;


    //Add initial condition
    _problem->addInitialCondition("Tricrystal2CircleGrainsIC", "InitialCondition", poly_params);
  }

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
