//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "AutoResidualTagAction.h"

#include "ActionWarehouse.h"
#include "MooseObjectAction.h"
#include "Factory.h"
#include "FEProblemBase.h"
#include "NonlinearSystemBase.h"
#include "ReporterName.h"

#include "libmesh/enum_parallel_type.h"

registerMooseAction("StochasticToolsApp", AutoResidualTagAction, "auto_residual_tag");
registerMooseAction("StochasticToolsApp", AutoResidualTagAction, "add_reporter");

// Single matrix tag that collects the full (single) Jacobian; every residual object fills it.
static const TagName jacobian_tag_name = "jacobian_tag";

InputParameters
AutoResidualTagAction::validParams()
{
  InputParameters params = Action::validParams();
  params.addClassDescription(
      "Creates one residual vector tag per Kernel and BoundaryCondition, routes each object's "
      "residual into its own tag, and adds the solution, residual, and Jacobian snapshot "
      "containers for DEIM reduced-order-model training.");
  params.addParam<bool>(
      "enable",
      true,
      "Whether to create the per-object residual tags and snapshot containers. Set to false to "
      "disable automatic residual tagging while leaving the block in place.");
  return params;
}

AutoResidualTagAction::AutoResidualTagAction(const InputParameters & parameters)
  : Action(parameters), _enable(getParam<bool>("enable"))
{
}

void
AutoResidualTagAction::act()
{
  if (!_enable)
    return;

  if (_current_task == "auto_residual_tag")
    createResidualTags();
  else if (_current_task == "add_reporter")
    addSnapshotContainers();
}

std::vector<MooseObjectAction *>
AutoResidualTagAction::residualObjectActions() const
{
  std::vector<MooseObjectAction *> actions;
  for (const auto & task : {"add_kernel", "add_bc"})
    for (auto * const action : _awh.getActionListByName(task))
      if (auto * const object_action = dynamic_cast<MooseObjectAction *>(action))
        actions.push_back(object_action);
  return actions;
}

TagName
AutoResidualTagAction::residualTagName(const std::string & object_name)
{
  return "residual_tag_" + object_name;
}

void
AutoResidualTagAction::createResidualTags()
{
  const auto actions = residualObjectActions();
  if (actions.empty())
    mooseError("found no Kernels or BoundaryConditions to tag. Add residual "
               "objects before enabling automatic residual tagging.");

  // The DEIM sub-apps use a single nonlinear system; the per-object residual tags and the Jacobian
  // tag are collected on it. Allocating the storage now, before the nonlinear system initializes,
  // keeps getVector/getMatrix valid when the residual and Jacobian are assembled.
  auto & nl = _problem->getNonlinearSystemBase(0);

  const TagID jacobian_tag = _problem->addMatrixTag(jacobian_tag_name);
  nl.addMatrix(jacobian_tag);

  std::set<TagName> residual_tags;
  for (auto * const action : actions)
  {
    const TagName tag_name = residualTagName(action->name());
    if (!residual_tags.insert(tag_name).second)
      mooseError("two residual objects map to the residual tag '",
                 tag_name,
                 "'. Residual object names must be unique for automatic residual tagging.");

    const TagID tag_id = _problem->addVectorTag(tag_name, Moose::VECTOR_TAG_RESIDUAL);
    nl.addVector(tag_id, false, libMesh::GHOSTED);

    // Route the object into its own residual tag and the shared Jacobian tag via the framework's
    // construction-time association parameters; the object is built later on add_kernel/add_bc.
    auto & object_params = action->getObjectParams();
    object_params.set<std::vector<TagName>>("extra_vector_tags").push_back(tag_name);
    object_params.set<std::vector<TagName>>("extra_matrix_tags").push_back(jacobian_tag_name);
  }
}

void
AutoResidualTagAction::addSnapshotContainers()
{
  // Solution snapshots.
  {
    auto params = _factory.getValidParams("SolutionContainer");
    params.set<ExecFlagEnum>("execute_on") = {EXEC_TIMESTEP_END};
    _problem->addReporter("SolutionContainer", "solution_storage", params);
  }

  // Residual snapshots: one stream per per-object residual tag.
  {
    auto params = _factory.getValidParams("ResidualContainer");
    std::vector<TagName> tag_names;
    for (auto * const action : residualObjectActions())
      tag_names.push_back(residualTagName(action->name()));
    params.set<std::vector<TagName>>("tag_names") = tag_names;
    params.set<ExecFlagEnum>("execute_on") = {EXEC_TIMESTEP_END};
    _problem->addReporter("ResidualContainer", "residual_storage", params);
  }

  // Jacobian snapshots: the single full-Jacobian matrix tag.
  {
    auto params = _factory.getValidParams("JacobianContainer");
    params.set<TagName>("tag_name") = jacobian_tag_name;
    params.set<ReporterValueName>("jac_indices_reporter_name") = "indices";
    params.set<ExecFlagEnum>("execute_on") = {EXEC_TIMESTEP_END};
    _problem->addReporter("JacobianContainer", "jacobian_storage", params);
  }
}
