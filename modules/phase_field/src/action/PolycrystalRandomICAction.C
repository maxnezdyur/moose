<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PolycrystalRandomICAction.h"
#include "Factory.h"
#include "FEProblem.h"
#include "Conversion.h"

registerMooseAction("PhaseFieldApp", PolycrystalRandomICAction, "add_ic");

InputParameters
PolycrystalRandomICAction::validParams()
{
  InputParameters params = Action::validParams();
  params.addRequiredParam<unsigned int>("op_num", "number of order parameters to create");
  params.addRequiredParam<std::string>("var_name_base", "specifies the base name of the variables");
  MooseEnum typ_options("continuous discrete");
  params.addRequiredParam<MooseEnum>("random_type",
                                     typ_options,
                                     "The type of random polycrystal initial condition. Whether "
                                     "one order parameter is chosen to be 1 at each node or if "
                                     "each other parameter continuously varies from 0 to 1");
  params.addParam<std::vector<SubdomainName>>("block",
                                              "Block restriction for the initial condition");
=======
#include "PolycrystalRandomICAction.h"
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

const Real PolycrystalRandomICAction::_abs_zero_tol = 1e-12;

template<>
InputParameters validParams<PolycrystalRandomICAction>()
{
  InputParameters params = validParams<Action>();
  params.addRequiredParam<unsigned int>("crys_num", "number of order parameters to create");
  params.addRequiredParam<std::string>("var_name_base","specifies the base name of the variables");
  MooseEnum typ_options("continuous, discrete");
  params.addParam<MooseEnum>("random_type",typ_options,"The type of random polycrystal initial condition. Whether one order parameter is chosen to be 1 at each node or if each order parameter continuously varies from 0 to 1");
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  return params;
}

<<<<<<< HEAD
PolycrystalRandomICAction::PolycrystalRandomICAction(const InputParameters & params)
  : Action(params),
    _op_num(getParam<unsigned int>("op_num")),
    _var_name_base(getParam<std::string>("var_name_base")),
    _random_type(getParam<MooseEnum>("random_type"))
{
}
=======
PolycrystalRandomICAction::PolycrystalRandomICAction(const std::string & name, InputParameters params)
  :Action(name, params),
   _crys_num(getParam<unsigned int>("crys_num")),
   _var_name_base(getParam<std::string>("var_name_base")),
   _random_type(getParam<MooseEnum>("random_type"))
{}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

void
PolycrystalRandomICAction::act()
{
#ifdef DEBUG
<<<<<<< HEAD
  Moose::err << "Inside the PolycrystalRandomICAction Object" << std::endl;
#endif

  // Loop through the number of order parameters
  for (unsigned int op = 0; op < _op_num; op++)
  {
    // Set parameters for BoundingBoxIC
    InputParameters poly_params = _factory.getValidParams("PolycrystalRandomIC");
    poly_params.set<VariableName>("variable") = _var_name_base + Moose::stringify(op);
    poly_params.set<unsigned int>("op_num") = _op_num;
    poly_params.set<unsigned int>("op_index") = op;
    poly_params.set<unsigned int>("random_type") = _random_type;
    if (isParamValid("block"))
      poly_params.set<std::vector<SubdomainName>>("block") =
          getParam<std::vector<SubdomainName>>("block");

    // Add initial condition
    _problem->addInitialCondition("PolycrystalRandomIC",
                                  "ICs/PolycrystalICs/PolycrystalRandomIC_" + Moose::stringify(op),
                                  poly_params);
  }
=======
  Moose::err << "Inside the PolycrystalRandomICAction Object\n";
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
    InputParameters poly_params = _factory.getValidParams("PolycrystalRandomIC");
    poly_params.set<VariableName>("variable") = var_name;
    poly_params.set<unsigned int>("crys_num") = _crys_num;
    poly_params.set<unsigned int>("crys_index") = crys;
    poly_params.set<unsigned int>("typ") = _random_type;


    //Add initial condition
    _problem->addInitialCondition("PolycrystalRandomIC", "InitialCondition", poly_params);
  }

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
