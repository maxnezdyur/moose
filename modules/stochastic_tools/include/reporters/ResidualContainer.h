//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "MooseTypes.h"
#include "NonlinearSystemBase.h"
#include "SnapshotContainerBase.h"
#include "SystemBase.h"
#include "libmesh/petsc_vector.h"

#include <map>
#include <vector>

class FEProblemBase;
class SubProblem;

/**
 * This class is responsible for collecting residual vectors in one place. The
 * vectors are kept distributed with respect to the communicator of the application.
 * The whole residual vector is stored.
 * The saving frequency can be defined using the `execute_on` parameter.
 */
class ResidualContainer : public SnapshotContainerBase
{
public:
  static InputParameters validParams();
  ResidualContainer(const InputParameters & parameters);

protected:
  virtual std::unique_ptr<NumericVector<Number>> collectSnapshot() override;
  virtual std::map<TagName, std::unique_ptr<NumericVector<Number>>> collectSnapshots() override;
  virtual bool collectMultipleSnapshots() const override { return _multi_stream; }

  const NonlinearSystem & _nl_sys;
  /// Whether one snapshot stream is collected per residual tag (true, from `tag_names` or the
  /// all-residual-tags default) or a single stream for one tag (false, backward-compatible
  /// scalar `tag_name`).
  const bool _multi_stream;
  /// Names of the residual tags whose vectors are collected.
  const std::vector<TagName> _tag_names;
  /// Ids of the residual tags whose vectors are collected, positionally matched to _tag_names.
  const std::vector<TagID> _tag_ids;

private:
  /**
   * Force a residual evaluation when collecting at EXEC_TIMESTEP_BEGIN. For linear problems the
   * residual is not computed by that point, so snapshotting a tagged residual vector requires
   * forcing the evaluation here.
   */
  void forceResidualEvaluation();

  /**
   * Resolve the residual tag names to collect from the input parameters. Returns `tag_names` if
   * given, otherwise the single `tag_name` if given, otherwise every residual-type vector tag
   * that the nonlinear system stores.
   * @param parameters The object's input parameters
   * @param fe_problem The problem holding the registered vector tags
   * @param nl_sys The nonlinear system whose stored residual vectors bound the default
   * @return the ordered list of residual tag names to collect
   */
  static std::vector<TagName> getResidualTagNames(const InputParameters & parameters,
                                                  const FEProblemBase & fe_problem,
                                                  const SystemBase & nl_sys);

  /**
   * Look up the vector tag id for each residual tag name.
   * @param tag_names The residual tag names to resolve
   * @param subproblem The problem that maps tag names to ids
   * @return the tag ids positionally matched to tag_names
   */
  static std::vector<TagID> getResidualTagIds(const std::vector<TagName> & tag_names,
                                              const SubProblem & subproblem);
};
