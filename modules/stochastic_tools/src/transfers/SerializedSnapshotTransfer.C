//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

// StochasticTools includes
#include "SerializedSnapshotTransfer.h"
#include "NonlinearSystemBase.h"
#include "Sampler.h"
#include "libmesh/id_types.h"

registerMooseObject("StochasticToolsApp", SerializedSnapshotTransfer);

InputParameters
SerializedSnapshotTransfer::validParams()
{
  InputParameters params = StochasticToolsTransfer::validParams();
  params.addClassDescription(
      "Serializes and transfers solution vectors for given variables from sub-applications.");
  params.addRequiredParam<std::string>("parallel_storage",
                                       "The name of the parallel storage reporter.");
  params.addRequiredParam<std::string>("solution_container",
                                       "The name of the solution container on the subapp.");
  params.addRequiredParam<std::string>("residual_container",
                                       "The name of the residual container on the subapp.");
  params.addRequiredParam<std::string>("jacobian_container",
                                       "The name of the jacobian container on the subapp.");
  params.addParam<std::string>("jacobian_index_container",
                               "The name of the jacobian container on the subapp.");
  params.addParam<bool>("serialize_on_root",
                        false,
                        "If we want to gather the solution fields only on the root processors of "
                        "the subapps before transfering to the main app.");
  return params;
}

SerializedSnapshotTransfer::SerializedSnapshotTransfer(const InputParameters & parameters)
  : StochasticToolsTransfer(parameters), _serialize_on_root(getParam<bool>("serialize_on_root"))
{
  if (hasToMultiApp())
    paramError("to_multi_app", "To and between multiapp directions are not implemented");
}

void
SerializedSnapshotTransfer::initialSetup()
{
  // Check if we have the storage space to receive the serialized solution fields
  _parallel_storage = &_fe_problem.getUserObject<ParallelSolutionStorage>(
      getParam<std::string>("parallel_storage"));
}

void
SerializedSnapshotTransfer::initializeInNormalMode()
{
  _solution_container.clear();
  _residual_container.clear();
  _jacobian_container.clear();

  const auto n = getFromMultiApp()->numGlobalApps();
  const auto & serialized_solution_reporter = getParam<std::string>("solution_container");
  const auto & serialized_residual_reporter = getParam<std::string>("residual_container");
  const auto & serialized_jacobian_reporter = getParam<std::string>("jacobian_container");

  for (MooseIndex(n) i = 0; i < n; i++)
    if (getFromMultiApp()->hasLocalApp(i))
    {
      FEProblemBase & app_problem = getFromMultiApp()->appProblemBase(i);
      _solution_container.push_back(
          &app_problem.getUserObject<SnapshotContainerBase>(serialized_solution_reporter));
      _residual_container.push_back(
          &app_problem.getUserObject<SnapshotContainerBase>(serialized_residual_reporter));
      _jacobian_container.push_back(
          &app_problem.getUserObject<SnapshotContainerBase>(serialized_jacobian_reporter));
    }
}

void
SerializedSnapshotTransfer::initializeInBatchMode()
{
  // First we fetch the solution containers from the subapps. This function is used
  // in batch mode only so we will have one solution container on each rank
  _solution_container.clear();
  _residual_container.clear();
  _jacobian_container.clear();
  FEProblemBase & app_problem = getFromMultiApp()->appProblemBase(_app_index);

  _solution_container.push_back(&app_problem.getUserObject<SnapshotContainerBase>(
      getParam<std::string>("solution_container")));
  _residual_container.push_back(&app_problem.getUserObject<SnapshotContainerBase>(
      getParam<std::string>("residual_container")));
  _jacobian_container.push_back(&app_problem.getUserObject<SnapshotContainerBase>(
      getParam<std::string>("jacobian_container")));
}

void
SerializedSnapshotTransfer::execute()
{
  initializeInNormalMode();

  const auto n = getFromMultiApp()->numGlobalApps();

  for (MooseIndex(n) i = 0; i < n; i++)
  {
    if (getFromMultiApp()->hasLocalApp(i))
    {
      FEProblemBase & app_problem = getFromMultiApp()->appProblemBase(i);

      // Converting the local indexing to global sample indices
      const unsigned int local_i = i - _sampler_ptr->getLocalRowBegin();

      transferStream(app_problem, _solution_container[local_i]->getSnapshots(), i, "solution");
      transferResidual(app_problem, *_residual_container[local_i], i);
      transferStream(app_problem, _jacobian_container[local_i]->getSnapshots(), i, "jacobian");
    }
  }
}

void
SerializedSnapshotTransfer::executeFromMultiapp()
{
  initializeInBatchMode();

  if (getFromMultiApp()->hasLocalApp(_app_index))
  {
    FEProblemBase & app_problem = getFromMultiApp()->appProblemBase(_app_index);

    transferStream(app_problem, _solution_container[0]->getSnapshots(), _global_index, "solution");
    transferResidual(app_problem, *_residual_container[0], _global_index);
    transferStream(app_problem, _jacobian_container[0]->getSnapshots(), _global_index, "jacobian");
  }
}

void
SerializedSnapshotTransfer::transferResidual(FEProblemBase & app_problem,
                                             const SnapshotContainerBase & residual_container,
                                             const dof_id_type global_i)
{
  const auto tags = residual_container.getSnapshotTags();

  // Legacy single-tag containers keep the single "residual" stream; multi-tag containers store
  // each tag's stream under its own key so the tags stay separable in the parallel storage.
  if (tags.empty())
    transferStream(app_problem, residual_container.getSnapshots(), global_i, "residual");
  else
    for (const auto & tag : tags)
      transferStream(
          app_problem, residual_container.getSnapshots(tag), global_i, "residual::" + tag);
}

void
SerializedSnapshotTransfer::transferStream(FEProblemBase & app_problem,
                                           const SnapshotContainerBase::Snapshots & snapshots,
                                           const dof_id_type global_i,
                                           const std::string & snapshot_type)
{
  if (_serialize_on_root)
    transferToSubAppRoot(app_problem, snapshots, global_i, snapshot_type);
  else
    transferInParallel(app_problem, snapshots, global_i, snapshot_type);
}

void
SerializedSnapshotTransfer::transferInParallel(FEProblemBase & app_problem,
                                               const SnapshotContainerBase::Snapshots & snapshots,
                                               const dof_id_type global_i,
                                               const std::string & snapshot_type)
{
  unsigned int local_app_index = global_i - _sampler_ptr->getLocalRowBegin();

  // We need to go through this communicator because the multiapp's
  // communicator is not necessarily the communicator of the underlying
  // MooseObject.
  const auto & comm = app_problem.comm();
  dof_id_type num_entries = _sampler_ptr->getNumberOfLocalRows();
  comm.sum(num_entries);

  // We shall distribute the samples on the given application between its processors.
  // Only using a linear partitioning here for the sake of simplicity.
  dof_id_type new_local_entries_begin;
  dof_id_type new_local_entries_end;
  dof_id_type num_new_local_entries;

  MooseUtils::linearPartitionItems(num_entries,
                                   comm.size(),
                                   comm.rank(),
                                   num_new_local_entries,
                                   new_local_entries_begin,
                                   new_local_entries_end);

  for (const auto & snapshot : snapshots)
  {
    DenseVector<Real> serialized_solution;

    // Create indices of that span entire vector
    auto num_vals = snapshot.size();
    std::vector<dof_id_type> indices(num_vals);
    std::iota(indices.begin(), indices.end(), 0);

    // Localize the solution and add it to the local container on the rank
    // which is supposed to own it
    snapshot.localize(
        serialized_solution.get_values(),
        (local_app_index >= new_local_entries_begin && local_app_index < new_local_entries_end)
            ? indices
            : std::vector<dof_id_type>());

    if (local_app_index >= new_local_entries_begin && local_app_index < new_local_entries_end)
      _parallel_storage->addEntry(snapshot_type, global_i, serialized_solution);
  }
}

void
SerializedSnapshotTransfer::transferToSubAppRoot(FEProblemBase & /*app_problem*/,
                                                 const SnapshotContainerBase::Snapshots & snapshots,
                                                 const dof_id_type global_i,
                                                 const std::string & snapshot_type)
{

  for (const auto & snapshot : snapshots)
  {
    DenseVector<Real> serialized_solution;

    // In this case we always serialize on the root processor of the application.
    snapshot.localize_to_one(serialized_solution.get_values());

    if (getFromMultiApp()->isRootProcessor())
      _parallel_storage->addEntry(snapshot_type, global_i, serialized_solution);
  }
}

SystemBase &
SerializedSnapshotTransfer::getSystem(FEProblemBase & app_problem, const VariableName & vname)
{
  auto & variable = app_problem.getVariable(_tid, vname);
  return variable.sys();
}
