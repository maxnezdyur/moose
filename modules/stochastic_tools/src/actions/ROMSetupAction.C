//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ROMSetupAction.h"

#include "ActionWarehouse.h"
#include "StochasticToolsAction.h"
#include "FEProblemBase.h"
#include "Factory.h"
#include "Control.h"
#include "VariableMappingBase.h"
#include "ReporterName.h"
#include "MooseUtils.h"

#include "hit/hit.h"

#include <fstream>
#include <memory>

registerMooseAction("StochasticToolsApp", ROMSetupAction, "meta_action");
registerMooseAction("StochasticToolsApp", ROMSetupAction, "add_multi_app");
registerMooseAction("StochasticToolsApp", ROMSetupAction, "add_transfer");
registerMooseAction("StochasticToolsApp", ROMSetupAction, "add_control");
registerMooseAction("StochasticToolsApp", ROMSetupAction, "add_reporter");
registerMooseAction("StochasticToolsApp", ROMSetupAction, "add_variable_mapping");
registerMooseAction("StochasticToolsApp", ROMSetupAction, "add_output");

InputParameters
ROMSetupAction::validParams()
{
  InputParameters params = Action::validParams();
  params.addClassDescription(
      "Sets up the Discrete Empirical Interpolation Method (DEIM) reduced-order-model training "
      "pipeline: the sampler-driven sub-application, the snapshot and Jacobian-index transfers, the "
      "snapshot storage, the DEIM mapping, and the mapping output.");

  params.addRequiredParam<SamplerName>(
      "sampler",
      "The sampler that drives the training study. It must be defined in the [Samplers] block and, "
      "because the sampled parameters are applied on the command line, its 'execute_on' must "
      "include 'PRE_MULTIAPP_SETUP'.");
  params.addRequiredParam<FileName>(
      "input", "The full-order-model sub-application input file to sample.");
  params.addRequiredParam<std::vector<std::string>>(
      "parameters",
      "The sub-application parameters perturbed by the sampler, in the sampler's column order.");
  params.addParam<bool>(
      "auto_tag_residual",
      true,
      "When true, inject the sub-application [AutoResidualTag] block so the residual is snapshotted "
      "per Kernel and BoundaryCondition and each residual object becomes its own DEIM component. "
      "When false, the residual is treated as a single component and the sub-application must "
      "define its own snapshot containers.");
  params.addParam<std::vector<dof_id_type>>(
      "num_modes",
      "The number of POD modes to compute, as exactly three entries: the solution modes, the modes "
      "for every residual component, and the Jacobian modes, in that order. The residual entry is "
      "applied to each residual component, so this stays independent of the number of residual "
      "objects. If unset, one mode is computed per component.");
  return params;
}

ROMSetupAction::ROMSetupAction(const InputParameters & parameters)
  : Action(parameters),
    _sampler(getParam<SamplerName>("sampler")),
    _input(getParam<FileName>("input")),
    _parameters(getParam<std::vector<std::string>>("parameters")),
    _auto_tag_residual(getParam<bool>("auto_tag_residual"))
{
  // Assemble the ordered DEIM mapping component list: exactly one 'solution', one component per
  // residual snapshot stream, and exactly one 'jacobian'. This ordering is shared by the generated
  // ParallelSolutionStorage, DEIMRBMapping, and MappingReporter so their component lists stay
  // consistent, and it fixes the positional meaning of 'num_modes'.
  const auto residual_components = getResidualComponents();
  _variables.push_back("solution");
  _variables.insert(_variables.end(), residual_components.begin(), residual_components.end());
  _variables.push_back("jacobian");

  if (isParamValid("num_modes"))
  {
    const auto & num_modes = getParam<std::vector<dof_id_type>>("num_modes");
    if (num_modes.size() != 3)
      paramError("num_modes",
                 "'num_modes' must have exactly three entries: the number of POD modes to compute "
                 "for the solution, for each residual component, and for the Jacobian, in that "
                 "order.");
    for (const auto & n : num_modes)
      if (n == 0)
        paramError("num_modes", "The number of modes must be a positive integer.");

    // Broadcast the single residual mode count across every residual component so 'num_modes' stays
    // independent of the number of Kernels and BoundaryConditions discovered on the sub-app.
    _modes_to_compute.push_back(num_modes[0]);
    _modes_to_compute.insert(_modes_to_compute.end(), residual_components.size(), num_modes[1]);
    _modes_to_compute.push_back(num_modes[2]);
  }
}

void
ROMSetupAction::act()
{
  if (_current_task == "meta_action")
    addStochasticToolsAction();
  else if (_current_task == "add_multi_app")
    addMultiApp();
  else if (_current_task == "add_transfer")
    addTransfers();
  else if (_current_task == "add_control")
    addControl();
  else if (_current_task == "add_reporter")
    addReporters();
  else if (_current_task == "add_variable_mapping")
    addMapping();
  else if (_current_task == "add_output")
    addOutput();
}

std::vector<VariableName>
ROMSetupAction::getResidualComponents() const
{
  // Without sub-app auto-tagging the residual is collected as a single legacy stream keyed
  // 'residual', matching a single-tag ResidualContainer defined by the user on the sub-application.
  if (!_auto_tag_residual)
    return {"residual"};

  // With auto-tagging the sub-app AutoResidualTagAction creates one residual vector tag named
  // 'residual_tag_<block>' per Kernel and BoundaryCondition, and the SerializedSnapshotTransfer
  // stores each tag's stream under the key 'residual::residual_tag_<block>'. Parse the sub-app input
  // here to reproduce that component list on the main app. Parsing at construction is the same
  // approach ParameterStudyAction uses to inspect the sub-app input.
  const auto input_filename = MooseUtils::realpath(_input);
  MooseUtils::checkFileReadable(input_filename);
  std::ifstream f(input_filename);
  const std::string input((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
  std::unique_ptr<hit::Node> root(hit::parse(input_filename, input));

  std::vector<VariableName> components;
  for (const auto & block : {"Kernels", "BCs"})
    if (const hit::Node * const node = root->find(block))
      for (const hit::Node * const child : node->children(hit::NodeType::Section))
        components.push_back("residual::residual_tag_" + child->path());

  if (components.empty())
    paramError("input",
               "No Kernels or BoundaryConditions were found in the sub-application input file '",
               input_filename,
               "'. Automatic residual tagging requires residual objects to tag; add the residual "
               "objects or set 'auto_tag_residual = false'.");

  return components;
}

void
ROMSetupAction::addStochasticToolsAction()
{
  // Only build the driver action if the user did not already provide a [StochasticTools] block.
  if (!_awh.getActions<StochasticToolsAction>().empty())
    return;

  auto params = _action_factory.getValidParams("StochasticToolsAction");
  params.set<bool>("_built_by_moose") = true;
  params.set<std::string>("registered_identifier") = "(AutoBuilt)";

  std::shared_ptr<Action> action =
      _action_factory.create("StochasticToolsAction", _name + "_stochastic_tools_action", params);
  _awh.addActionBlock(action);
}

void
ROMSetupAction::addMultiApp()
{
  auto params = _factory.getValidParams("SamplerFullSolveMultiApp");
  params.set<std::vector<FileName>>("input_files") = {_input};
  params.set<SamplerName>("sampler") = _sampler;

  // 'batch-reset' re-initializes the sub-app for each sample, which is required so the per-sample
  // snapshots are clean, and it is the mode MultiAppSamplerControl needs to modify command-line
  // arguments.
  params.set<MooseEnum>("mode") = "batch-reset";

  // Enable the sub-app [AutoResidualTag] block through the command line. MultiAppSamplerControl
  // preserves this static argument and appends the sampled parameters to it.
  if (_auto_tag_residual)
    params.set<std::vector<CLIArgString>>("cli_args").push_back("AutoResidualTag/enable=true");

  params.set<ExecFlagEnum>("execute_on") = {EXEC_TIMESTEP_BEGIN};

  _problem->addMultiApp("SamplerFullSolveMultiApp", multiAppName(), params);
}

void
ROMSetupAction::addTransfers()
{
  // Serialized snapshots of the solution, residual, and Jacobian. The sub-app container names are
  // fixed by the [AutoResidualTag] block on the sub-application.
  {
    auto params = _factory.getValidParams("SerializedSnapshotTransfer");
    params.set<std::string>("parallel_storage") = parallelStorageName();
    params.set<MultiAppName>("from_multi_app") = multiAppName();
    params.set<SamplerName>("sampler") = _sampler;
    params.set<std::string>("solution_container") = "solution_storage";
    params.set<std::string>("residual_container") = "residual_storage";
    params.set<std::string>("jacobian_container") = "jacobian_storage";
    params.set<bool>("serialize_on_root") = true;
    _problem->addTransfer("SerializedSnapshotTransfer", snapshotTransferName(), params);
  }

  // The Jacobian sparsity indices are identical for every sample, so transfer the single copy from
  // the first sub-app into the dummy storage on the main app.
  {
    auto params = _factory.getValidParams("MultiAppReporterTransfer");
    params.set<MultiAppName>("from_multi_app") = multiAppName();
    params.set<std::vector<ReporterName>>("from_reporters") = {
        ReporterName("jacobian_storage", "indices")};
    params.set<std::vector<ReporterName>>("to_reporters") = {
        ReporterName(jacobianIndexStorageName(), "indices")};
    params.set<unsigned int>("subapp_index") = 0;
    _problem->addTransfer("MultiAppReporterTransfer", jacobianIndexTransferName(), params);
  }
}

void
ROMSetupAction::addControl()
{
  auto params = _factory.getValidParams("MultiAppSamplerControl");
  params.set<MultiAppName>("multi_app") = multiAppName();
  params.set<SamplerName>("sampler") = _sampler;
  params.set<std::vector<std::string>>("param_names") = _parameters;

  auto control = _factory.create<Control>("MultiAppSamplerControl", samplerControlName(), params);
  _problem->getControlWarehouse().addObject(control);
}

void
ROMSetupAction::addReporters()
{
  // Parallel storage receiving the serialized snapshots for every mapping component.
  {
    auto params = _factory.getValidParams("ParallelSolutionStorage");
    params.set<std::vector<VariableName>>("variables") = _variables;
    params.set<std::vector<OutputName>>("outputs") = {"none"};
    _problem->addReporter("ParallelSolutionStorage", parallelStorageName(), params);
  }

  // Reporter that builds the DEIM mapping from the stored snapshots.
  {
    auto params = _factory.getValidParams("MappingReporter");
    params.set<UserObjectName>("mapping") = mappingName();
    params.set<std::vector<VariableName>>("variables") = _variables;
    params.set<bool>("build_all_mappings_only") = true;
    params.set<ExecFlagEnum>("execute_on") = {EXEC_TIMESTEP_END};
    params.set<std::vector<OutputName>>("outputs") = {"none"};
    _problem->addReporter("MappingReporter", mappingReporterName(), params);
  }

  // Dummy container that only holds the transferred Jacobian sparsity indices. JacobianContainer
  // resolves its matrix tag at construction, so register the tag first; the container never
  // collects (execute_on = NONE).
  {
    _problem->addMatrixTag(jacobianMatrixTagName());

    auto params = _factory.getValidParams("JacobianContainer");
    params.set<TagName>("tag_name") = jacobianMatrixTagName();
    params.set<ReporterValueName>("jac_indices_reporter_name") = "indices";
    params.set<ExecFlagEnum>("execute_on") = {EXEC_NONE};
    params.set<std::vector<OutputName>>("outputs") = {"none"};
    _problem->addReporter("JacobianContainer", jacobianIndexStorageName(), params);
  }
}

void
ROMSetupAction::addMapping()
{
  auto params = _factory.getValidParams("DEIMRBMapping");
  params.set<std::vector<VariableName>>("variables") = _variables;
  params.set<UserObjectName>("solution_storage") = parallelStorageName();
  if (!_modes_to_compute.empty())
    params.set<std::vector<dof_id_type>>("num_modes_to_compute") = _modes_to_compute;
  params.set<ReporterName>("jac_index_name") = ReporterName(jacobianIndexStorageName(), "indices");
  _problem->addObject<VariableMappingBase>(
      "DEIMRBMapping", mappingName(), params, /* threaded = */ false);
}

void
ROMSetupAction::addOutput()
{
  auto params = _factory.getValidParams("MappingOutput");
  params.set<std::vector<std::string>>("mappings") = {mappingName()};
  params.set<ExecFlagEnum>("execute_on") = {EXEC_FINAL};
  _problem->addOutput("MappingOutput", mappingOutputName(), params);
}
