<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "NavierStokesApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"

#include "FluidPropertiesApp.h"
#include "HeatConductionApp.h"

InputParameters
NavierStokesApp::validParams()
{
  InputParameters params = MooseApp::validParams();

  params.set<bool>("automatic_automatic_scaling") = false;

  params.set<bool>("use_legacy_material_output") = false;

  return params;
}

registerKnownLabel("NavierStokesApp");

NavierStokesApp::NavierStokesApp(InputParameters parameters) : MooseApp(parameters)
{
  NavierStokesApp::registerAll(_factory, _action_factory, _syntax);
}

NavierStokesApp::~NavierStokesApp() {}
=======
#include "NavierStokesApp.h"
#include "Moose.h"
#include "AppFactory.h"

#include "NSMassInviscidFlux.h"
#include "NSMomentumInviscidFlux.h"
#include "NSEnergyInviscidFlux.h"
#include "NSGravityPower.h"
#include "NSGravityForce.h"
#include "NSThermalBC.h"
#include "NSVelocityAux.h"
#include "NSImposedVelocityBC.h"
#include "NSTemperatureAux.h"
#include "NSTemperatureL2.h"
#include "NSPressureAux.h"
#include "NSEnthalpyAux.h"
#include "NSEnergyThermalFlux.h"
#include "NSMomentumViscousFlux.h"
#include "NSEnergyViscousFlux.h"
#include "NSMomentumInviscidFluxWithGradP.h"
#include "NSSUPGMomentum.h"
#include "NSSUPGMass.h"
#include "NSSUPGEnergy.h"
#include "NSMassSpecifiedNormalFlowBC.h"
#include "NSMassUnspecifiedNormalFlowBC.h"
#include "NSInflowThermalBC.h"
#include "NSMomentumInviscidSpecifiedPressureBC.h"
#include "NSMomentumInviscidSpecifiedNormalFlowBC.h"
#include "NSMomentumViscousBC.h"
#include "NSEnergyInviscidSpecifiedPressureBC.h"
#include "NSEnergyInviscidSpecifiedNormalFlowBC.h"
#include "NSEnergyInviscidUnspecifiedBC.h"
#include "NSEnergyInviscidSpecifiedBC.h"
#include "NSEnergyInviscidSpecifiedDensityAndVelocityBC.h"
#include "NSEnergyViscousBC.h"
#include "NSStagnationPressureBC.h"
#include "NSStagnationTemperatureBC.h"
#include "NSImposedVelocityDirectionBC.h"
#include "NSMassWeakStagnationBC.h"
#include "NSMomentumConvectiveWeakStagnationBC.h"
#include "NSMomentumPressureWeakStagnationBC.h"
#include "NSEnergyWeakStagnationBC.h"
#include "NSPenalizedNormalFlowBC.h"
#include "NSMomentumInviscidNoPressureImplicitFlowBC.h"

//
// Incompressible
//

// Kernels
#include "INSMass.h"
#include "INSMomentumTimeDerivative.h"
#include "INSMomentum.h"
#include "INSTemperatureTimeDerivative.h"
#include "INSTemperature.h"
#include "INSSplitMomentum.h"
#include "INSProjection.h"
#include "INSPressurePoisson.h"
#include "INSChorinPredictor.h"
#include "INSChorinCorrector.h"
#include "INSChorinPressurePoisson.h"

// BCs
#include "INSMomentumNoBCBC.h"
#include "INSTemperatureNoBCBC.h"
#include "ImplicitNeumannBC.h"

// AuxKernels
#include "INSCourant.h"
#include "INSDivergenceAux.h"

// Postprocessors
#include "INSExplicitTimestepSelector.h"

template<>
InputParameters validParams<NavierStokesApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

NavierStokesApp::NavierStokesApp(const std::string & name, InputParameters parameters) :
    MooseApp(name, parameters)
{
  srand(libMesh::processor_id());

  Moose::registerObjects(_factory);
  NavierStokesApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  NavierStokesApp::associateSyntax(_syntax, _action_factory);
}

NavierStokesApp::~NavierStokesApp()
{
}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

void
NavierStokesApp::registerApps()
{
  registerApp(NavierStokesApp);
}

<<<<<<< HEAD
static void
associateSyntaxInner(Syntax & syntax, ActionFactory & /*action_factory*/)
{
  // Create the syntax
  registerSyntax("CNSAction", "Modules/CompressibleNavierStokes");
  registerSyntax("INSAction", "Modules/IncompressibleNavierStokes");
  registerSyntax("NSFVAction", "Modules/NavierStokesFV");

  // add variables action
  registerTask("add_navier_stokes_variables", /*is_required=*/false);
  addTaskDependency("add_navier_stokes_variables", "add_variable");
  addTaskDependency("add_mortar_variable", "add_navier_stokes_variables");

  // add ICs action
  registerTask("add_navier_stokes_ics", /*is_required=*/false);
  addTaskDependency("add_navier_stokes_ics", "add_ic");
  addTaskDependency("add_constraint", "add_navier_stokes_ics");

  // add user objects action
  registerTask("add_navier_stokes_user_objects", /*is_required=*/false);
  addTaskDependency("add_navier_stokes_user_objects", "add_user_object");
  addTaskDependency("add_distribution", "add_navier_stokes_user_objects");

  // add Kernels action
  registerTask("add_navier_stokes_kernels", /*is_required=*/false);
  addTaskDependency("add_navier_stokes_kernels", "add_kernel");
  addTaskDependency("resolve_optional_materials", "add_navier_stokes_kernels");

  // add BCs actions
  registerTask("add_navier_stokes_bcs", /*is_required=*/false);
  addTaskDependency("add_navier_stokes_bcs", "add_bc");
  addTaskDependency("resolve_optional_materials", "add_navier_stokes_bcs");

  // register attributes
  registerTask("ns_meta_action", /*is_required=*/true);
  addTaskDependency("ns_meta_action", "meta_action");
  addTaskDependency("dynamic_object_registration", "ns_meta_action");

  // register attributes
  registerTask("add_navier_stokes_pps", /*is_required=*/false);
  addTaskDependency("add_navier_stokes_pps", "add_postprocessor");
  addTaskDependency("add_vector_postprocessor", "add_navier_stokes_pps");

  // register attributes
  registerTask("add_navier_stokes_materials", /*is_required=*/false);
  addTaskDependency("add_navier_stokes_materials", "add_material");
  addTaskDependency("add_master_action_material", "add_navier_stokes_materials");

  // register tasks for copying variables
  registerTask("navier_stokes_check_copy_nodal_vars", /*is_required=*/false);
  addTaskDependency("navier_stokes_check_copy_nodal_vars", "check_copy_nodal_vars");
  addTaskDependency("setup_mesh", "navier_stokes_check_copy_nodal_vars");

  registerTask("navier_stokes_copy_nodal_vars", /*is_required=*/false);
  addTaskDependency("navier_stokes_copy_nodal_vars", "copy_nodal_vars");
  addTaskDependency("add_material", "navier_stokes_copy_nodal_vars");
}

void
NavierStokesApp::registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  FluidPropertiesApp::registerAll(f, af, s);
  HeatConductionApp::registerAll(f, af, s);
  Registry::registerObjectsTo(f, {"NavierStokesApp"});
  Registry::registerActionsTo(af, {"NavierStokesApp"});
  associateSyntaxInner(s, af);
}

void
NavierStokesApp::registerObjectDepends(Factory & factory)
{
  mooseDeprecated("use registerAll instead of registerObjectsDepends");
  FluidPropertiesApp::registerObjects(factory);
}

void
NavierStokesApp::registerObjects(Factory & factory)
{
  mooseDeprecated("use registerAll instead of registerObjects");
  Registry::registerObjectsTo(factory, {"NavierStokesApp"});
}

void
NavierStokesApp::associateSyntaxDepends(Syntax & syntax, ActionFactory & action_factory)
{
  mooseDeprecated("use registerAll instead of associateSyntaxDepends");
  FluidPropertiesApp::associateSyntax(syntax, action_factory);
=======
void
NavierStokesApp::registerObjects(Factory & factory)
{
  registerKernel(NSMassInviscidFlux);
  registerKernel(NSMomentumInviscidFlux);
  registerKernel(NSEnergyInviscidFlux);
  registerKernel(NSGravityPower);
  registerKernel(NSGravityForce);
  registerKernel(NSTemperatureL2);
  registerBoundaryCondition(NSThermalBC);
  registerAux(NSVelocityAux);
  registerBoundaryCondition(NSImposedVelocityBC);
  registerAux(NSTemperatureAux);
  registerAux(NSPressureAux);
  registerAux(NSEnthalpyAux);
  registerKernel(NSEnergyThermalFlux);
  registerKernel(NSMomentumViscousFlux);
  registerKernel(NSEnergyViscousFlux);
  registerKernel(NSMomentumInviscidFluxWithGradP);
  registerKernel(NSSUPGMomentum);
  registerKernel(NSSUPGMass);
  registerKernel(NSSUPGEnergy);
  registerBoundaryCondition(NSMassSpecifiedNormalFlowBC);
  registerBoundaryCondition(NSMassUnspecifiedNormalFlowBC);
  registerBoundaryCondition(NSInflowThermalBC);
  registerBoundaryCondition(NSMomentumInviscidSpecifiedPressureBC);
  registerBoundaryCondition(NSMomentumInviscidSpecifiedNormalFlowBC);
  registerBoundaryCondition(NSMomentumViscousBC);
  registerBoundaryCondition(NSEnergyInviscidSpecifiedPressureBC);
  registerBoundaryCondition(NSEnergyInviscidSpecifiedNormalFlowBC);
  registerBoundaryCondition(NSEnergyInviscidUnspecifiedBC);
  registerBoundaryCondition(NSEnergyInviscidSpecifiedBC);
  registerBoundaryCondition(NSEnergyInviscidSpecifiedDensityAndVelocityBC);
  registerBoundaryCondition(NSEnergyViscousBC);
  registerBoundaryCondition(NSStagnationPressureBC);
  registerBoundaryCondition(NSStagnationTemperatureBC);
  registerBoundaryCondition(NSImposedVelocityDirectionBC);
  registerBoundaryCondition(NSMassWeakStagnationBC);
  registerBoundaryCondition(NSMomentumConvectiveWeakStagnationBC);
  registerBoundaryCondition(NSMomentumPressureWeakStagnationBC);
  registerBoundaryCondition(NSEnergyWeakStagnationBC);
  registerBoundaryCondition(NSPenalizedNormalFlowBC);
  registerBoundaryCondition(NSMomentumInviscidNoPressureImplicitFlowBC);

  //
  // Incompressible
  //

  // Kernels
  registerKernel(INSMass);
  registerKernel(INSMomentumTimeDerivative);
  registerKernel(INSMomentum);
  registerKernel(INSTemperatureTimeDerivative);
  registerKernel(INSTemperature);
  registerKernel(INSSplitMomentum);
  registerKernel(INSProjection);
  registerKernel(INSPressurePoisson);
  registerKernel(INSChorinPredictor);
  registerKernel(INSChorinCorrector);
  registerKernel(INSChorinPressurePoisson);

  // BCs
  registerBoundaryCondition(INSMomentumNoBCBC);
  registerBoundaryCondition(INSTemperatureNoBCBC);
  registerBoundaryCondition(ImplicitNeumannBC);

  // AuxKernels
  registerAux(INSCourant);
  registerAux(INSDivergenceAux);

  // Postprocessors
  registerPostprocessor(INSExplicitTimestepSelector);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

void
NavierStokesApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
<<<<<<< HEAD
  mooseDeprecated("use registerAll instead of associateSyntax");
  Registry::registerActionsTo(action_factory, {"NavierStokesApp"});
  associateSyntaxInner(syntax, action_factory);
}

void
NavierStokesApp::registerExecFlags(Factory & /*factory*/)
{
  mooseDeprecated("Do not use registerExecFlags, apps no longer require flag registration");
}

extern "C" void
NavierStokesApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  NavierStokesApp::registerAll(f, af, s);
}
extern "C" void
NavierStokesApp__registerApps()
{
  NavierStokesApp::registerApps();
=======
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
