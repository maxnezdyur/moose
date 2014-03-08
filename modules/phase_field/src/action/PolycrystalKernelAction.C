<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PolycrystalKernelAction.h"
#include "Factory.h"
#include "Conversion.h"
#include "FEProblem.h"

registerMooseAction("PhaseFieldApp", PolycrystalKernelAction, "add_kernel");

InputParameters
PolycrystalKernelAction::validParams()
{
  InputParameters params = Action::validParams();
  params.addClassDescription(
      "Set up ACGrGrPoly, ACInterface, TimeDerivative, and ACGBPoly kernels");
  params.addRequiredParam<unsigned int>(
      "op_num", "specifies the total number of grains (deformed + recrystallized) to create");
  params.addRequiredParam<std::string>("var_name_base", "specifies the base name of the variables");
  params.addParam<VariableName>("c", "Name of coupled concentration variable");
  params.addParam<Real>("en_ratio", 1.0, "Ratio of surface to GB energy");
  params.addParam<unsigned int>("ndef", 0, "specifies the number of deformed grains to create");
  params.addParam<bool>("implicit", true, "Whether kernels are implicit or not");
  params.addParam<bool>(
      "use_displaced_mesh", false, "Whether to use displaced mesh in the kernels");
  params.addParam<bool>("variable_mobility",
                        true,
                        "The mobility is a function of any MOOSE variable (if "
                        "this is set to false, L must be constant over the "
                        "entire domain!)");
  params.addParam<std::vector<VariableName>>("args", "Vector of variable arguments L depends on");
  return params;
}

PolycrystalKernelAction::PolycrystalKernelAction(const InputParameters & params)
  : Action(params),
    _op_num(getParam<unsigned int>("op_num")),
    _var_name_base(getParam<std::string>("var_name_base"))
{
}
=======
#include "PolycrystalKernelAction.h"
#include "Factory.h"
#include "Parser.h"
#include "FEProblem.h"

template<>
InputParameters validParams<PolycrystalKernelAction>()
{
  InputParameters params = validParams<Action>();

  params.addRequiredParam<unsigned int>("crys_num", "specifies the number of grains to create");
  params.addRequiredParam<std::string>("var_name_base", "specifies the base name of the variables");
  params.addParam<VariableName>("c","NONE","Name of coupled concentration variable");
  params.addParam<Real>("en_ratio",1.0,"Ratio of surface to GB energy");
  params.addParam<bool>("implicit",true,"Whether kernels are implicit or not");
  params.addParam<VariableName>("T","Name of temperature variable");

  return params;
}

PolycrystalKernelAction::PolycrystalKernelAction(const std::string & name, InputParameters params)
  :Action(name, params),
   _crys_num(getParam<unsigned int>("crys_num")),
   _var_name_base(getParam<std::string>("var_name_base")),
   _c(getParam<VariableName>("c")),
   _implicit(getParam<bool>("implicit")),
   _T(getParam<VariableName>("T"))
{}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

void
PolycrystalKernelAction::act()
{
<<<<<<< HEAD
  for (unsigned int op = 0; op < _op_num; ++op)
  {
    //
    // Create variable names
    //

    std::string var_name = _var_name_base + Moose::stringify(op);
    std::vector<VariableName> v;
    v.resize(_op_num - 1);

    unsigned int ind = 0;
    for (unsigned int j = 0; j < _op_num; ++j)
      if (j != op)
        v[ind++] = _var_name_base + Moose::stringify(j);

    //
    // Set up ACGrGrPoly kernels
    //

    {
      InputParameters params = _factory.getValidParams("ACGrGrPoly");
      params.set<NonlinearVariableName>("variable") = var_name;
      params.set<std::vector<VariableName>>("v") = v;
      params.applyParameters(parameters());

      std::string kernel_name = "ACBulk_" + var_name;
      _problem->addKernel("ACGrGrPoly", kernel_name, params);
    }

    //
    // Set up ACInterface kernels
    //

    {
      InputParameters params = _factory.getValidParams("ACInterface");
      params.set<NonlinearVariableName>("variable") = var_name;
      params.applyParameters(parameters());

      std::string kernel_name = "ACInt_" + var_name;
      _problem->addKernel("ACInterface", kernel_name, params);
    }

    //
    // Set up TimeDerivative kernels
    //

    {
      InputParameters params = _factory.getValidParams("TimeDerivative");
      params.set<NonlinearVariableName>("variable") = var_name;
      params.set<bool>("implicit") = true;
      params.applyParameters(parameters());

      std::string kernel_name = "IE_" + var_name;
      _problem->addKernel("TimeDerivative", kernel_name, params);
    }

    //
    // Set up optional ACGBPoly bubble interaction kernels
    //

    if (isParamValid("c"))
    {
      InputParameters params = _factory.getValidParams("ACGBPoly");
      params.set<NonlinearVariableName>("variable") = var_name;
      params.set<std::vector<VariableName>>("c") = {getParam<VariableName>("c")};
      params.applyParameters(parameters());

      std::string kernel_name = "ACBubInteraction_" + var_name;
      _problem->addKernel("ACGBPoly", kernel_name, params);
    }
  }
=======
#ifdef DEBUG
  Moose::err << "Inside the PolyCrystalKernelAction Object\n";
  Moose::err << "var name base:" << _var_name_base;
#endif
  // Moose::out << "Implicit = " << _implicit << Moose::out;


  for (unsigned int crys = 0; crys<_crys_num; crys++)
  {
    //Create variable names
    std::string var_name = _var_name_base;
    std::stringstream out;
    out << crys;
    var_name.append(out.str());

    std::vector<VariableName> v;
    v.resize(_crys_num - 1);

    unsigned int ind = 0;

    for (unsigned int j = 0; j<_crys_num; j++)
    {

      if (j != crys)
      {
        std::string coupled_var_name = _var_name_base;
        std::stringstream out2;
        out2 << j;
        coupled_var_name.append(out2.str());
        v[ind] = coupled_var_name;
        ind++;
      }
    }

    InputParameters poly_params = _factory.getValidParams("ACGrGrPoly");
    poly_params.set<NonlinearVariableName>("variable") = var_name;
    poly_params.set<std::vector<VariableName> >("v") = v;
    poly_params.set<bool>("implicit")=_implicit;
    if (!_T.empty())
      poly_params.set<std::vector<VariableName> >("T").push_back(_T);

    std::string kernel_name = "ACBulk_";
    kernel_name.append(var_name);

    _problem->addKernel("ACGrGrPoly", kernel_name, poly_params);

    /************/

    poly_params = _factory.getValidParams("ACInterface");
    poly_params.set<NonlinearVariableName>("variable") = var_name;
    poly_params.set<bool>("implicit")=getParam<bool>("implicit");

    kernel_name = "ACInt_";
    kernel_name.append(var_name);

    _problem->addKernel("ACInterface", kernel_name, poly_params);
    //*******************

    poly_params = _factory.getValidParams("TimeDerivative");
    poly_params.set<NonlinearVariableName>("variable") = var_name;
    poly_params.set<bool>("implicit") = true;

    kernel_name = "IE_";
    kernel_name.append(var_name);

    _problem->addKernel("TimeDerivative", kernel_name, poly_params);
    /************/
    if (_c != "NONE")//Add in Bubble interaction kernel, if using bubbles
    {
      poly_params = _factory.getValidParams("ACGBPoly");
      poly_params.set<NonlinearVariableName>("variable") = var_name;
      poly_params.set<std::vector<VariableName> >("c").push_back(_c);
      poly_params.set<Real>("en_ratio") = getParam<Real>("en_ratio");
      poly_params.set<bool>("implicit")=getParam<bool>("implicit");

      kernel_name = "ACBubInteraction_";
      kernel_name.append(var_name);

      _problem->addKernel("ACGBPoly", kernel_name, poly_params);
    }
  }


>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
