//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "Action.h"

#include <string>
#include <vector>

/**
 * Action for the [ROM] block that auto-wires the Discrete Empirical Interpolation Method (DEIM)
 * reduced-order-model training pipeline. From a single block it generates the sampler-driven
 * SamplerFullSolveMultiApp, the serialized snapshot transfer, the Jacobian sparsity-index transfer,
 * the parallel snapshot storage, the DEIMRBMapping and its build reporter, and the mapping output.
 * When 'auto_tag_residual' is set it also injects the sub-application [AutoResidualTag] block so the
 * residual is snapshotted per Kernel and BoundaryCondition. The action is registered against the
 * individual pipeline tasks and act() dispatches on the current task.
 */
class ROMSetupAction : public Action
{
public:
  static InputParameters validParams();

  ROMSetupAction(const InputParameters & parameters);

  virtual void act() override;

protected:
  /**
   * Build the StochasticToolsAction that turns the main application into a driver with an
   * auto-created mesh, problem, and executioner, unless a [StochasticTools] block already did so.
   * Runs on the 'meta_action' task.
   */
  void addStochasticToolsAction();

  /**
   * Add the SamplerFullSolveMultiApp that solves the full-order sub-application for every sample,
   * injecting the AutoResidualTag command-line argument when 'auto_tag_residual' is set. Runs on
   * the 'add_multi_app' task.
   */
  void addMultiApp();

  /**
   * Add the SerializedSnapshotTransfer that gathers the solution, residual, and Jacobian snapshots
   * and the MultiAppReporterTransfer that pulls the Jacobian sparsity indices. Runs on the
   * 'add_transfer' task.
   */
  void addTransfers();

  /**
   * Add the MultiAppSamplerControl that passes the sampled parameters to the sub-application on the
   * command line. Runs on the 'add_control' task.
   */
  void addControl();

  /**
   * Add the ParallelSolutionStorage, the build-only MappingReporter, and the dummy JacobianContainer
   * that receives the transferred Jacobian sparsity indices. Runs on the 'add_reporter' task.
   */
  void addReporters();

  /**
   * Add the DEIMRBMapping over the discovered solution, residual, and Jacobian components. Runs on
   * the 'add_variable_mapping' task.
   */
  void addMapping();

  /**
   * Add the MappingOutput that writes the trained mapping model data. Runs on the 'add_output' task.
   */
  void addOutput();

  /**
   * Discover the residual snapshot components of the DEIM mapping. When 'auto_tag_residual' is set,
   * parse the sub-application input and return one 'residual::residual_tag_<block>' component per
   * Kernel and BoundaryCondition block, matching the per-object residual tags that the sub-app
   * AutoResidualTagAction creates and that the SerializedSnapshotTransfer keys the streams under.
   * Otherwise return the single legacy 'residual' component.
   * @return the ordered residual component names
   */
  std::vector<VariableName> getResidualComponents() const;

  ///@{
  /// Prescribed names of the objects this action generates.
  std::string multiAppName() const { return "rom_trainer"; }
  std::string samplerControlName() const { return "rom_sampler_control"; }
  std::string snapshotTransferName() const { return "rom_snapshot_transfer"; }
  std::string jacobianIndexTransferName() const { return "rom_jacobian_index_transfer"; }
  std::string parallelStorageName() const { return "rom_parallel_storage"; }
  std::string mappingReporterName() const { return "rom_build_mapping"; }
  std::string jacobianIndexStorageName() const { return "rom_jacobian_indices"; }
  std::string mappingName() const { return "rom_mapping"; }
  std::string mappingOutputName() const { return "rom_mapping_output"; }
  ///@}

  /// Name of the main-application matrix tag the dummy JacobianContainer resolves at construction.
  std::string jacobianMatrixTagName() const { return "rom_jacobian_matrix"; }

private:
  /// The user-provided sampler that drives the training study.
  const SamplerName _sampler;
  /// The full-order-model sub-application input file.
  const FileName _input;
  /// The sub-application parameters perturbed by the sampler, in the sampler's column order.
  const std::vector<std::string> _parameters;
  /// Whether to inject the sub-app [AutoResidualTag] block for per-object residual snapshots.
  const bool _auto_tag_residual;
  /// The ordered DEIM mapping components: 'solution', the residual components, then 'jacobian'.
  std::vector<VariableName> _variables;
  /// The POD modes to compute per entry of _variables; empty when 'num_modes' is unset.
  std::vector<dof_id_type> _modes_to_compute;
};
