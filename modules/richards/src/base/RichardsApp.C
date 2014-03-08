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

#include "RichardsApp.h"
#include "Moose.h"
#include "AppFactory.h"
<<<<<<< HEAD
#include "MooseSyntax.h"

InputParameters
RichardsApp::validParams()
{
  InputParameters params = MooseApp::validParams();

  params.set<bool>("automatic_automatic_scaling") = false;
  params.set<bool>("use_legacy_material_output") = false;

  return params;
}

registerKnownLabel("RichardsApp");

RichardsApp::RichardsApp(const InputParameters & parameters) : MooseApp(parameters)
{
  mooseDeprecated("Please use the PorousFlow module instead.  If Richards contains functionality "
                  "not included in PorousFlow, please contact the moose-users google group");
  RichardsApp::registerAll(_factory, _action_factory, _syntax);
}

RichardsApp::~RichardsApp() {}

static void
associateSyntaxInner(Syntax & syntax, ActionFactory & /*action_factory*/)
{
  registerSyntaxTask("Q2PAction", "Q2P", "add_kernel");
  registerSyntaxTask("Q2PAction", "Q2P", "add_aux_variable");
  registerSyntaxTask("Q2PAction", "Q2P", "add_function");
  registerSyntaxTask("Q2PAction", "Q2P", "add_postprocessor");
}
void
RichardsApp::registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  Registry::registerObjectsTo(f, {"RichardsApp"});
  Registry::registerActionsTo(af, {"RichardsApp"});
  associateSyntaxInner(s, af);
=======

// UserObjects
#include "RichardsPorepressureNames.h"
#include "RichardsDensityConstBulk.h"
#include "RichardsDensityIdeal.h"
#include "RichardsDensityMethane20degC.h"
#include "RichardsRelPermMonomial.h"
#include "RichardsRelPermPower.h"
#include "RichardsRelPermVG.h"
#include "RichardsRelPermVG1.h"
#include "RichardsRelPermBW.h"
#include "RichardsSeff1VG.h"
#include "RichardsSeff1VGcut.h"
#include "RichardsSeff1BWsmall.h"
#include "RichardsSeff1RSC.h"
#include "RichardsSeff2waterVG.h"
#include "RichardsSeff2gasVG.h"
#include "RichardsSeff2waterRSC.h"
#include "RichardsSeff2gasRSC.h"
#include "RichardsSat.h"
#include "RichardsSUPGnone.h"
#include "RichardsSUPGstandard.h"
#include "RichardsSumQuantity.h"

// AuxKernels
#include "RichardsSeffAux.h"
#include "RichardsSeffPrimeAux.h"
#include "RichardsSeffPrimePrimeAux.h"
#include "RichardsDensityAux.h"
#include "RichardsDensityPrimeAux.h"
#include "RichardsDensityPrimePrimeAux.h"
#include "RichardsRelPermAux.h"
#include "RichardsRelPermPrimeAux.h"
#include "RichardsRelPermPrimePrimeAux.h"
#include "FunctionOfVariableAux.h"

// Materials
#include "RichardsMaterial.h"

// DiracKernels
#include "RichardsBorehole.h"
#include "RichardsPolyLineSink.h"

// Functions
#include "RichardsExcavGeom.h"
#include "GradParsedFunction.h"
#include "Grad2ParsedFunction.h"

// Indicators
#include "RichardsFluxJumpIndicator.h"

// Markers
#include "OrientedBoxMarkerDepr.h"

// Postprocessors
#include "RichardsMass.h"
#include "RichardsPiecewiseLinearSinkFlux.h"
#include "RichardsHalfGaussianSinkFlux.h"
#include "NodalMaxVarChange.h"
#include "RichardsExcavFlow.h"
#include "RichardsPlotQuantity.h"

// TimeSteppers
#include "FunctionControlledDT.h"

// Kernels
#include "RichardsMassChange.h"
#include "RichardsFlux.h"
#include "RichardsPPenalty.h"

  // BoundaryConditions
#include "RichardsExcav.h"
#include "RichardsPiecewiseLinearSink.h"
#include "RichardsHalfGaussianSink.h"


template<>
InputParameters validParams<RichardsApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

RichardsApp::RichardsApp(const std::string & name, InputParameters parameters) :
    MooseApp(name, parameters)
{
  srand(libMesh::processor_id());

  Moose::registerObjects(_factory);
  RichardsApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  RichardsApp::associateSyntax(_syntax, _action_factory);
}

RichardsApp::~RichardsApp()
{
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

void
RichardsApp::registerApps()
{
  registerApp(RichardsApp);
}

void
RichardsApp::registerObjects(Factory & factory)
{
<<<<<<< HEAD
  mooseDeprecated("use registerAll instead of registerObjects");
  Registry::registerObjectsTo(factory, {"RichardsApp"});
=======
  // UserObjects
  registerUserObject(RichardsPorepressureNames);
  registerUserObject(RichardsDensityConstBulk);
  registerUserObject(RichardsDensityIdeal);
  registerUserObject(RichardsDensityMethane20degC);
  registerUserObject(RichardsRelPermMonomial);
  registerUserObject(RichardsRelPermPower);
  registerUserObject(RichardsRelPermVG);
  registerUserObject(RichardsRelPermVG1);
  registerUserObject(RichardsRelPermBW);
  registerUserObject(RichardsSeff1VG);
  registerUserObject(RichardsSeff1VGcut);
  registerUserObject(RichardsSeff1BWsmall);
  registerUserObject(RichardsSeff1RSC);
  registerUserObject(RichardsSeff2waterVG);
  registerUserObject(RichardsSeff2gasVG);
  registerUserObject(RichardsSeff2waterRSC);
  registerUserObject(RichardsSeff2gasRSC);
  registerUserObject(RichardsSat);
  registerUserObject(RichardsSUPGnone);
  registerUserObject(RichardsSUPGstandard);
  registerUserObject(RichardsSumQuantity);

  // AuxKernels
  registerAux(RichardsSeffAux);
  registerAux(RichardsSeffPrimeAux);
  registerAux(RichardsSeffPrimePrimeAux);
  registerAux(RichardsDensityAux);
  registerAux(RichardsDensityPrimeAux);
  registerAux(RichardsDensityPrimePrimeAux);
  registerAux(RichardsRelPermAux);
  registerAux(RichardsRelPermPrimeAux);
  registerAux(RichardsRelPermPrimePrimeAux);
  registerAux(FunctionOfVariableAux);

  // Materials
  registerMaterial(RichardsMaterial);

  // DiracKernels
  registerDiracKernel(RichardsPolyLineSink);
  registerDiracKernel(RichardsBorehole);

  // Functions
  registerFunction(RichardsExcavGeom);
  registerFunction(GradParsedFunction);
  registerFunction(Grad2ParsedFunction);

  // Indicators
  registerIndicator(RichardsFluxJumpIndicator);

  // Markers
  registerMarker(OrientedBoxMarkerDepr);

  // Postprocessors
  registerPostprocessor(RichardsMass);
  registerPostprocessor(RichardsPiecewiseLinearSinkFlux);
  registerPostprocessor(RichardsHalfGaussianSinkFlux);
  registerPostprocessor(NodalMaxVarChange);
  registerPostprocessor(RichardsExcavFlow);
  registerPostprocessor(RichardsPlotQuantity);

  // TimeSteppers
  registerTimeStepper(FunctionControlledDT);

  // Kernels
  registerKernel(RichardsMassChange);
  registerKernel(RichardsFlux);
  registerKernel(RichardsPPenalty);

  // BoundaryConditions
  registerBoundaryCondition(RichardsExcav);
  registerBoundaryCondition(RichardsPiecewiseLinearSink);
  registerBoundaryCondition(RichardsHalfGaussianSink);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

void
RichardsApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
<<<<<<< HEAD
  mooseDeprecated("use registerAll instead of associateSyntax");
  Registry::registerActionsTo(action_factory, {"RichardsApp"});
  associateSyntaxInner(syntax, action_factory);
}

void
RichardsApp::registerExecFlags(Factory & /*factory*/)
{
  mooseDeprecated("Do not use registerExecFlags, apps no longer require flag registration");
=======
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
