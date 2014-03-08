<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PhaseFieldApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"

InputParameters
PhaseFieldApp::validParams()
{
  InputParameters params = MooseApp::validParams();

  params.set<bool>("automatic_automatic_scaling") = false;

  params.set<bool>("use_legacy_material_output") = false;

  return params;
}

registerKnownLabel("PhaseFieldApp");

PhaseFieldApp::PhaseFieldApp(const InputParameters & parameters) : MooseApp(parameters)
{
  PhaseFieldApp::registerAll(_factory, _action_factory, _syntax);
}

PhaseFieldApp::~PhaseFieldApp() {}

static void
associateSyntaxInner(Syntax & syntax, ActionFactory & /*action_factory*/)
{
  registerSyntax("BicrystalBoundingBoxICAction", "ICs/PolycrystalICs/BicrystalBoundingBoxIC");
  registerSyntax("BicrystalCircleGrainICAction", "ICs/PolycrystalICs/BicrystalCircleGrainIC");
  registerSyntax("CHPFCRFFSplitKernelAction", "Kernels/CHPFCRFFSplitKernel");
  registerSyntax("CHPFCRFFSplitVariablesAction", "Variables/CHPFCRFFSplitVariables");
  registerSyntax("ConservedAction", "Modules/PhaseField/Conserved/*");
  registerSyntax("DisplacementGradientsAction", "Modules/PhaseField/DisplacementGradients");
  registerSyntax("EmptyAction", "ICs/PolycrystalICs"); // placeholder
  registerSyntax("EulerAngle2RGBAction", "Modules/PhaseField/EulerAngles2RGB");
  registerSyntax("GrainGrowthAction", "Modules/PhaseField/GrainGrowth");
  registerSyntax("HHPFCRFFSplitKernelAction", "Kernels/HHPFCRFFSplitKernel");
  registerSyntax("HHPFCRFFSplitVariablesAction", "Variables/HHPFCRFFSplitVariables");
  registerSyntax("MatVecRealGradAuxKernelAction", "AuxKernels/MatVecRealGradAuxKernel");
  registerSyntax("MaterialVectorAuxKernelAction", "AuxKernels/MaterialVectorAuxKernel");
  registerSyntax("MaterialVectorGradAuxKernelAction", "AuxKernels/MaterialVectorGradAuxKernel");
  registerSyntax("MultiAuxVariablesAction", "AuxVariables/MultiAuxVariables");
  registerSyntax("PFCRFFKernelAction", "Kernels/PFCRFFKernel");
  registerSyntax("PFCRFFVariablesAction", "Variables/PFCRFFVariables");
  registerSyntax("PolycrystalColoringICAction", "ICs/PolycrystalICs/PolycrystalColoringIC");
  registerSyntax("PolycrystalElasticDrivingForceAction", "Kernels/PolycrystalElasticDrivingForce");
  registerSyntax("NonconservedAction", "Modules/PhaseField/Nonconserved/*");
  registerSyntax("PolycrystalKernelAction", "Kernels/PolycrystalKernel");
  registerSyntax("PolycrystalRandomICAction", "ICs/PolycrystalICs/PolycrystalRandomIC");
  registerSyntax("PolycrystalStoredEnergyAction", "Kernels/PolycrystalStoredEnergy");
  registerSyntax("PolycrystalVariablesAction", "Variables/PolycrystalVariables");
  registerSyntax("PolycrystalVoronoiVoidICAction", "ICs/PolycrystalICs/PolycrystalVoronoiVoidIC");
  registerSyntax("RigidBodyMultiKernelAction", "Kernels/RigidBodyMultiKernel");
  registerSyntax("Tricrystal2CircleGrainsICAction", "ICs/PolycrystalICs/Tricrystal2CircleGrainsIC");
  registerSyntax("GrandPotentialKernelAction", "Modules/PhaseField/GrandPotential");
}

void
PhaseFieldApp::registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  Registry::registerObjectsTo(f, {"PhaseFieldApp"});
  Registry::registerActionsTo(af, {"PhaseFieldApp"});
  associateSyntaxInner(s, af);
=======
#include "PhaseFieldApp.h"
#include "Moose.h"
#include "AppFactory.h"

#include "MatDiffusion.h"
#include "ACInterface.h"
#include "CHMath.h"
#include "CHInterface.h"
#include "SplitCHWRes.h"
#include "SplitCHMath.h"
#include "CoupledImplicitEuler.h"
#include "CrossIC.h"
#include "SmoothCircleIC.h"
#include "RndSmoothCircleIC.h"
#include "MultiSmoothCircleIC.h"
#include "LatticeSmoothCircleIC.h"
#include "SpecifiedSmoothCircleIC.h"
#include "RndBoundingBoxIC.h"
#include "PFMobility.h"
#include "NodalFloodCount.h"
#include "NodalFloodCountAux.h"
#include "NodalVolumeFraction.h"
#include "BndsCalcAux.h"
#include "ACGrGrPoly.h"
#include "ACGBPoly.h"
#include "GBEvolution.h"
#include "HexPolycrystalIC.h"
#include "PolycrystalRandomIC.h"
#include "PolycrystalReducedIC.h"
#include "ThumbIC.h"
#include "Tricrystal2CircleGrainsIC.h"
#include "PolycrystalVariablesAction.h"
#include "PolycrystalKernelAction.h"
#include "BicrystalCircleGrainICAction.h"
#include "BicrystalBoundingBoxICAction.h"
#include "Tricrystal2CircleGrainsICAction.h"
#include "PolycrystalHexGrainICAction.h"
#include "PolycrystalVoronoiICAction.h"
#include "PolycrystalRandomICAction.h"
//#include "SPPARKSUserObject.h"
//#include "SPPARKSAux.h"

template<>
InputParameters validParams<PhaseFieldApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

PhaseFieldApp::PhaseFieldApp(const std::string & name, InputParameters parameters) :
    MooseApp(name, parameters)
{
  srand(libMesh::processor_id());

  Moose::registerObjects(_factory);
  PhaseFieldApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  PhaseFieldApp::associateSyntax(_syntax, _action_factory);
}

PhaseFieldApp::~PhaseFieldApp()
{
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

void
PhaseFieldApp::registerApps()
{
  registerApp(PhaseFieldApp);
}

void
PhaseFieldApp::registerObjects(Factory & factory)
{
<<<<<<< HEAD
  mooseDeprecated("use registerAll instead of registerObjects");
  Registry::registerObjectsTo(factory, {"PhaseFieldApp"});
=======
  registerKernel(MatDiffusion);
  registerKernel(ACInterface);
  registerKernel(CHMath);
  registerKernel(CHInterface);
  registerKernel(SplitCHWRes);
  registerKernel(SplitCHMath);
  registerKernel(CoupledImplicitEuler);
  registerKernel(ACGrGrPoly);
  registerKernel(ACGBPoly);
  registerInitialCondition(CrossIC);
  registerInitialCondition(SmoothCircleIC);
  registerInitialCondition(RndSmoothCircleIC);
  registerInitialCondition(MultiSmoothCircleIC);
  registerInitialCondition(LatticeSmoothCircleIC);
  registerInitialCondition(SpecifiedSmoothCircleIC);
  registerInitialCondition(RndBoundingBoxIC);
  registerInitialCondition(HexPolycrystalIC);
  registerInitialCondition(PolycrystalRandomIC);
  registerInitialCondition(PolycrystalReducedIC);
  registerInitialCondition(ThumbIC);
  registerInitialCondition(Tricrystal2CircleGrainsIC);
  registerMaterial(PFMobility);
  registerMaterial(GBEvolution);
  registerUserObject(NodalFloodCount);
  registerAux(NodalFloodCountAux);
  registerAux(BndsCalcAux);
//  registerAux(SPPARKSAux);
  registerUserObject(NodalVolumeFraction);
//  registerUserObject(SPPARKSUserObject);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

void
PhaseFieldApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
<<<<<<< HEAD
  mooseDeprecated("use registerAll instead of associateSyntax");
  Registry::registerActionsTo(action_factory, {"PhaseFieldApp"});
  associateSyntaxInner(syntax, action_factory);
}

void
PhaseFieldApp::registerExecFlags(Factory & /*factory*/)
{
  mooseDeprecated("Do not use registerExecFlags, apps no longer require flag registration");
}

extern "C" void
PhaseFieldApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  PhaseFieldApp::registerAll(f, af, s);
}
extern "C" void
PhaseFieldApp__registerApps()
{
  PhaseFieldApp::registerApps();
=======
  syntax.registerActionSyntax("PolycrystalKernelAction", "Kernels/PolycrystalKernel");
  syntax.registerActionSyntax("PolycrystalVariablesAction", "Variables/PolycrystalVariables");
  syntax.registerActionSyntax("EmptyAction", "ICs/PolycrystalICs");  // placeholder
  syntax.registerActionSyntax("BicrystalCircleGrainICAction", "ICs/PolycrystalICs/BicrystalCircleGrainIC");
  syntax.registerActionSyntax("BicrystalBoundingBoxICAction", "ICs/PolycrystalICs/BicrystalBoundingBoxIC");
  syntax.registerActionSyntax("Tricrystal2CircleGrainsICAction", "ICs/PolycrystalICs/Tricrystal2CircleGrainsIC");
  syntax.registerActionSyntax("PolycrystalHexGrainICAction", "ICs/PolycrystalICs/PolycrystalHexGrainIC");
  syntax.registerActionSyntax("PolycrystalVoronoiICAction", "ICs/PolycrystalICs/PolycrystalVoronoiIC");
  syntax.registerActionSyntax("PolycrystalRandomICAction", "ICs/PolycrystalICs/PolycrystalRandomIC");

  registerAction(PolycrystalKernelAction, "add_kernel");
  registerAction(PolycrystalVariablesAction, "add_variable");
  registerAction(BicrystalCircleGrainICAction, "add_ic");
  registerAction(BicrystalBoundingBoxICAction, "add_ic");
  registerAction(Tricrystal2CircleGrainsICAction, "add_ic");
  registerAction(PolycrystalHexGrainICAction, "add_ic");
  registerAction(PolycrystalVoronoiICAction, "add_ic");
  registerAction(PolycrystalRandomICAction, "add_ic");
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
