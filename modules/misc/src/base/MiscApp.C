<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "MiscApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"

InputParameters
MiscApp::validParams()
{
  InputParameters params = MooseApp::validParams();

  params.set<bool>("use_legacy_material_output") = false;

  return params;
}

registerKnownLabel("MiscApp");

MiscApp::MiscApp(const InputParameters & parameters) : MooseApp(parameters)
{
  MiscApp::registerAll(_factory, _action_factory, _syntax);
}

MiscApp::~MiscApp() {}

void
MiscApp::registerAll(Factory & f, ActionFactory & af, Syntax & /*s*/)
{
  Registry::registerObjectsTo(f, {"MiscApp"});
  Registry::registerActionsTo(af, {"MiscApp"});
=======
#include "MiscApp.h"
#include "Moose.h"
#include "AppFactory.h"

#include "BodyForceVoid.h"
#include "CoefDiffusion.h"
#include "Convection.h"
#include "Density.h"
#include "InternalVolume.h"
#include "RobinBC.h"
#include "JouleHeating.h"
#include "CoefTimeDerivative.h"
#include "GaussContForcing.h"
#include "SharpInterfaceForcing.h"
#include "RigidBodyModesRZ.h"
#include "RigidBodyModes3D.h"
#include "CoupledDirectionalMeshHeightInterpolation.h"
#include "CInterfacePosition.h"

template<>
InputParameters validParams<MiscApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

MiscApp::MiscApp(const std::string & name, InputParameters parameters) :
    MooseApp(name, parameters)
{
  srand(libMesh::processor_id());

  Moose::registerObjects(_factory);
  MiscApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  MiscApp::associateSyntax(_syntax, _action_factory);
}

MiscApp::~MiscApp()
{
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

void
MiscApp::registerApps()
{
  registerApp(MiscApp);
}

void
MiscApp::registerObjects(Factory & factory)
{
<<<<<<< HEAD
  Registry::registerObjectsTo(factory, {"MiscApp"});
}

void
MiscApp::associateSyntax(Syntax & /*syntax*/, ActionFactory & action_factory)
{
  Registry::registerActionsTo(action_factory, {"MiscApp"});
}

void
MiscApp::registerExecFlags(Factory & /*factory*/)
{
}

extern "C" void
MiscApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  MiscApp::registerAll(f, af, s);
}

extern "C" void
MiscApp__registerApps()
{
  MiscApp::registerApps();
}
=======
  registerAux(CoupledDirectionalMeshHeightInterpolation);

  registerBoundaryCondition(RobinBC);

  registerKernel(BodyForceVoid);
  registerKernel(CoefDiffusion);
  registerKernel(Convection);
  registerKernel(JouleHeating);
  registerKernel(CoefTimeDerivative);
  registerKernel(GaussContForcing);

  registerMaterial(Density);

  registerUserObject(RigidBodyModesRZ);
  registerUserObject(RigidBodyModes3D);

  registerPostprocessor(InternalVolume);
  registerPostprocessor(SharpInterfaceForcing);

  registerPostprocessor(CInterfacePosition);
}

void
MiscApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
