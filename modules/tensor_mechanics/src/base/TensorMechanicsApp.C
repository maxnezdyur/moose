<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "TensorMechanicsApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"

InputParameters
TensorMechanicsApp::validParams()
{
  InputParameters params = MooseApp::validParams();

  params.set<bool>("automatic_automatic_scaling") = false;

  params.set<bool>("use_legacy_material_output") = false;

  return params;
}

registerKnownLabel("TensorMechanicsApp");

TensorMechanicsApp::TensorMechanicsApp(const InputParameters & parameters) : MooseApp(parameters)
{
  TensorMechanicsApp::registerAll(_factory, _action_factory, _syntax);
}

TensorMechanicsApp::~TensorMechanicsApp() {}

static void
associateSyntaxInner(Syntax & syntax, ActionFactory & /*action_factory*/)
{
  registerSyntax("EmptyAction", "BCs/CavityPressure");
  registerSyntax("CavityPressureAction", "BCs/CavityPressure/*");
  registerSyntax("CavityPressurePPAction", "BCs/CavityPressure/*");
  registerSyntax("CavityPressureUOAction", "BCs/CavityPressure/*");

  registerSyntax("LegacyTensorMechanicsAction", "Kernels/TensorMechanics");
  registerSyntax("LegacyDynamicTensorMechanicsAction", "Kernels/DynamicTensorMechanics");
  registerSyntax("PoroMechanicsAction", "Kernels/PoroMechanics");

  registerSyntax("EmptyAction", "BCs/Pressure");
  registerSyntax("PressureAction", "BCs/Pressure/*");
  registerSyntax("EmptyAction", "BCs/InclinedNoDisplacementBC");
  registerSyntax("InclinedNoDisplacementBCAction", "BCs/InclinedNoDisplacementBC/*");
  registerSyntax("EmptyAction", "BCs/CoupledPressure");
  registerSyntax("CoupledPressureAction", "BCs/CoupledPressure/*");

  registerSyntax("GeneralizedPlaneStrainAction",
                 "Modules/TensorMechanics/GeneralizedPlaneStrain/*");
  registerSyntax("GlobalStrainAction", "Modules/TensorMechanics/GlobalStrain/*");
  registerSyntax("CommonTensorMechanicsAction", "Modules/TensorMechanics/Master");
  registerSyntax("CommonTensorMechanicsAction", "Modules/TensorMechanics/DynamicMaster");
  registerSyntax("TensorMechanicsAction", "Modules/TensorMechanics/Master/*");
  registerSyntax("DynamicTensorMechanicsAction", "Modules/TensorMechanics/DynamicMaster/*");

  registerSyntax("CommonLineElementAction", "Modules/TensorMechanics/LineElementMaster");
  registerSyntax("LineElementAction", "Modules/TensorMechanics/LineElementMaster/*");

  registerSyntaxTask("DomainIntegralAction", "DomainIntegral", "add_user_object");
  registerSyntaxTask("DomainIntegralAction", "DomainIntegral", "add_aux_variable");
  registerSyntaxTask("DomainIntegralAction", "DomainIntegral", "add_aux_kernel");
  registerSyntaxTask("DomainIntegralAction", "DomainIntegral", "add_postprocessor");
  registerSyntaxTask("DomainIntegralAction", "DomainIntegral", "add_vector_postprocessor");
  registerSyntaxTask("DomainIntegralAction", "DomainIntegral", "add_material");

  registerSyntax("CommonCohesiveZoneAction", "Modules/TensorMechanics/CohesiveZoneMaster");
  registerSyntax("CohesiveZoneAction", "Modules/TensorMechanics/CohesiveZoneMaster/*");

  registerSyntax("EmptyAction", "Modules/TensorMechanics/MaterialVectorBodyForce");
  registerSyntax("MaterialVectorBodyForceAction",
                 "Modules/TensorMechanics/MaterialVectorBodyForce/*");

  registerTask("validate_coordinate_systems", /*is_required=*/false);
  addTaskDependency("validate_coordinate_systems", "create_problem_complete");
  addTaskDependency("setup_postprocessor_data", "validate_coordinate_systems");
}

void
TensorMechanicsApp::registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  Registry::registerObjectsTo(f, {"TensorMechanicsApp"});
  Registry::registerActionsTo(af, {"TensorMechanicsApp"});
  associateSyntaxInner(s, af);
  registerDataFilePath();
=======
#include "TensorMechanicsApp.h"
#include "Moose.h"
#include "AppFactory.h"

#include "TensorMechanicsAction.h"
#include "StressDivergenceTensors.h"
#include "LinearElasticMaterial.h"
#include "FiniteStrainElasticMaterial.h"
#include "FiniteStrainPlasticMaterial.h"
#include "FiniteStrainRatePlasticMaterial.h"
#include "FiniteStrainCrystalPlasticity.h"
#include "RankTwoAux.h"
#include "RealTensorValueAux.h"
#include "RankFourAux.h"
#include "TensorElasticEnergyAux.h"
#include "FiniteStrainPlasticAux.h"
#include "CrystalPlasticitySlipSysAux.h"
#include "CrystalPlasticityRotationOutAux.h"

template<>
InputParameters validParams<TensorMechanicsApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

TensorMechanicsApp::TensorMechanicsApp(const std::string & name, InputParameters parameters) :
    MooseApp(name, parameters)
{
  srand(libMesh::processor_id());

  Moose::registerObjects(_factory);
  TensorMechanicsApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  TensorMechanicsApp::associateSyntax(_syntax, _action_factory);
}

TensorMechanicsApp::~TensorMechanicsApp()
{
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

void
TensorMechanicsApp::registerApps()
{
  registerApp(TensorMechanicsApp);
}

void
TensorMechanicsApp::registerObjects(Factory & factory)
{
<<<<<<< HEAD
  mooseDeprecated("use registerAll instead of registerObjects");
  Registry::registerObjectsTo(factory, {"TensorMechanicsApp"});
=======
  registerKernel(StressDivergenceTensors);

  registerMaterial(LinearElasticMaterial);
  registerMaterial(FiniteStrainElasticMaterial);
  registerMaterial(FiniteStrainPlasticMaterial);
  registerMaterial(FiniteStrainRatePlasticMaterial);
  registerMaterial(FiniteStrainCrystalPlasticity);

  registerAux(RankTwoAux);
  registerAux(RealTensorValueAux);
  registerAux(RankFourAux);
  registerAux(TensorElasticEnergyAux);
  registerAux(FiniteStrainPlasticAux);
  registerAux(CrystalPlasticitySlipSysAux);
  registerAux(CrystalPlasticityRotationOutAux);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

void
TensorMechanicsApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
<<<<<<< HEAD
  mooseDeprecated("use registerAll instead of associateSyntax");
  Registry::registerActionsTo(action_factory, {"TensorMechanicsApp"});
  associateSyntaxInner(syntax, action_factory);
}

void
TensorMechanicsApp::registerExecFlags(Factory & /*factory*/)
{
  mooseDeprecated("Do not use registerExecFlags, apps no longer require flag registration");
}

extern "C" void
TensorMechanicsApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  TensorMechanicsApp::registerAll(f, af, s);
}
extern "C" void
TensorMechanicsApp_registerApps()
{
  TensorMechanicsApp::registerApps();
=======
  syntax.registerActionSyntax("TensorMechanicsAction", "TensorMechanics/*");

  registerAction(TensorMechanicsAction, "add_kernel");
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
