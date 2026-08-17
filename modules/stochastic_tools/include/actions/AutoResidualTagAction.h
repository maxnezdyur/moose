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

class MooseObjectAction;

/**
 * Creates one residual vector tag per Kernel and BoundaryCondition on the sub-app, routes each
 * object's residual contribution into its own tag, and adds the solution, residual, and Jacobian
 * snapshot containers used by the DEIM reduced-order-model training pipeline. The per-object tags
 * are assigned through the framework's construction-time 'extra_vector_tags' parameter, so this
 * action runs before the Kernels and BoundaryConditions are built.
 */
class AutoResidualTagAction : public Action
{
public:
  static InputParameters validParams();

  AutoResidualTagAction(const InputParameters & parameters);

  virtual void act() override;

protected:
  /**
   * Create the per-object residual vector tags and the single Jacobian matrix tag, allocate their
   * storage on the nonlinear system, and route each Kernel/BoundaryCondition into its tags before
   * the objects are constructed on add_kernel/add_bc.
   */
  void createResidualTags();

  /**
   * Add the SolutionContainer, ResidualContainer, and JacobianContainer reporters, pointing the
   * ResidualContainer at the per-object residual tags.
   */
  void addSnapshotContainers();

  /**
   * Collect the pending Kernel and BoundaryCondition actions whose residual objects are tagged.
   * @return the residual-object actions in enumeration order
   */
  std::vector<MooseObjectAction *> residualObjectActions() const;

  /**
   * Deterministic residual vector tag name for a residual object. Both the tagging and reporter
   * phases derive the tag names with this scheme so they agree without shared state.
   * @param object_name The name of the Kernel or BoundaryCondition
   * @return the residual vector tag name for that object
   */
  static TagName residualTagName(const std::string & object_name);

private:
  /// Whether the action creates the tags and containers; when false act() is a no-op.
  const bool _enable;
};
