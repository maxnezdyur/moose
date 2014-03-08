<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ChemicalReactionsApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"

InputParameters
ChemicalReactionsApp::validParams()
{
  InputParameters params = MooseApp::validParams();

  params.set<bool>("automatic_automatic_scaling") = false;
  params.set<bool>("use_legacy_material_output") = false;

  return params;
}

registerKnownLabel("ChemicalReactionsApp");

ChemicalReactionsApp::ChemicalReactionsApp(const InputParameters & parameters)
  : MooseApp(parameters)
{
  ChemicalReactionsApp::registerAll(_factory, _action_factory, _syntax);
}

ChemicalReactionsApp::~ChemicalReactionsApp() {}

static void
associateSyntaxInner(Syntax & syntax, ActionFactory & /*action_factory*/)
{
  registerSyntax("AddPrimarySpeciesAction", "ReactionNetwork/AqueousEquilibriumReactions");
  registerSyntax("AddPrimarySpeciesAction", "ReactionNetwork/SolidKineticReactions");
  registerSyntax("AddSecondarySpeciesAction", "ReactionNetwork/AqueousEquilibriumReactions");
  registerSyntax("AddSecondarySpeciesAction", "ReactionNetwork/SolidKineticReactions");
  registerSyntax("AddCoupledEqSpeciesAction", "ReactionNetwork/AqueousEquilibriumReactions");
  registerSyntax("AddCoupledSolidKinSpeciesAction", "ReactionNetwork/SolidKineticReactions");
}

void
ChemicalReactionsApp::registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  Registry::registerObjectsTo(f, {"ChemicalReactionsApp"});
  Registry::registerActionsTo(af, {"ChemicalReactionsApp"});
  associateSyntaxInner(s, af);
=======
#include "ChemicalReactionsApp.h"
#include "Moose.h"
#include "AppFactory.h"

#include "PrimaryTimeDerivative.h"
#include "PrimaryConvection.h"
#include "PrimaryDiffusion.h"
#include "CoupledBEEquilibriumSub.h"
#include "CoupledConvectionReactionSub.h"
#include "CoupledDiffusionReactionSub.h"
#include "CoupledBEKinetic.h"
#include "DesorptionFromMatrix.h"
#include "DesorptionToPorespace.h"

#include "AqueousEquilibriumRxnAux.h"
#include "KineticDisPreConcAux.h"
#include "KineticDisPreRateAux.h"

#include "AddPrimarySpeciesAction.h"
#include "AddSecondarySpeciesAction.h"
#include "AddCoupledEqSpeciesKernelsAction.h"
#include "AddCoupledEqSpeciesAuxKernelsAction.h"
#include "AddCoupledSolidKinSpeciesKernelsAction.h"
#include "AddCoupledSolidKinSpeciesAuxKernelsAction.h"

#include "ChemicalOutFlowBC.h"

#include "LangmuirMaterial.h"

template<>
InputParameters validParams<ChemicalReactionsApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

ChemicalReactionsApp::ChemicalReactionsApp(const std::string & name, InputParameters parameters) :
    MooseApp(name, parameters)
{
  srand(libMesh::processor_id());

  Moose::registerObjects(_factory);
  ChemicalReactionsApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  ChemicalReactionsApp::associateSyntax(_syntax, _action_factory);
}

ChemicalReactionsApp::~ChemicalReactionsApp()
{
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

void
ChemicalReactionsApp::registerApps()
{
  registerApp(ChemicalReactionsApp);
}

void
ChemicalReactionsApp::registerObjects(Factory & factory)
{
<<<<<<< HEAD
  mooseDeprecated("use registerAll instead of registerObjects");
  Registry::registerObjectsTo(factory, {"ChemicalReactionsApp"});
=======
  registerKernel(PrimaryTimeDerivative);
  registerKernel(PrimaryConvection);
  registerKernel(PrimaryDiffusion);
  registerKernel(CoupledBEEquilibriumSub);
  registerKernel(CoupledConvectionReactionSub);
  registerKernel(CoupledDiffusionReactionSub);
  registerKernel(CoupledBEKinetic);
  registerKernel(DesorptionFromMatrix);
  registerKernel(DesorptionToPorespace);

  registerAux(AqueousEquilibriumRxnAux);
  registerAux(KineticDisPreConcAux);
  registerAux(KineticDisPreRateAux);

  registerBoundaryCondition(ChemicalOutFlowBC);

  registerMaterial(LangmuirMaterial);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

void
ChemicalReactionsApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
<<<<<<< HEAD
  mooseDeprecated("use registerAll instead of associateSyntax");
  Registry::registerActionsTo(action_factory, {"ChemicalReactionsApp"});
  associateSyntaxInner(syntax, action_factory);
}

void
ChemicalReactionsApp::registerExecFlags(Factory & /*factory*/)
{
  mooseDeprecated("Do not use registerExecFlags, apps no longer require flag registration");
}

extern "C" void
ChemicalReactionsApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  ChemicalReactionsApp::registerAll(f, af, s);
}

// External entry point for dynamic application loading
extern "C" void
ChemicalReactionsApp__registerApps()
{
  ChemicalReactionsApp::registerApps();
=======
  syntax.registerActionSyntax("AddPrimarySpeciesAction", "ReactionNetwork");
  syntax.registerActionSyntax("AddSecondarySpeciesAction", "ReactionNetwork/AqueousEquilibriumReactions");
  syntax.registerActionSyntax("AddSecondarySpeciesAction", "ReactionNetwork/SolidKineticReactions");
  syntax.registerActionSyntax("AddCoupledEqSpeciesKernelsAction", "ReactionNetwork/AqueousEquilibriumReactions");
  syntax.registerActionSyntax("AddCoupledEqSpeciesAuxKernelsAction", "ReactionNetwork/AqueousEquilibriumReactions");
  syntax.registerActionSyntax("AddCoupledSolidKinSpeciesKernelsAction", "ReactionNetwork/SolidKineticReactions");
  syntax.registerActionSyntax("AddCoupledSolidKinSpeciesAuxKernelsAction", "ReactionNetwork/SolidKineticReactions");
  registerAction(AddPrimarySpeciesAction, "add_variable");
  registerAction(AddSecondarySpeciesAction, "add_aux_variable");
  registerAction(AddCoupledEqSpeciesKernelsAction, "add_kernel");
  registerAction(AddCoupledEqSpeciesAuxKernelsAction, "add_aux_kernel");
  registerAction(AddCoupledSolidKinSpeciesKernelsAction, "add_kernel");
  registerAction(AddCoupledSolidKinSpeciesAuxKernelsAction, "add_aux_kernel");
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
