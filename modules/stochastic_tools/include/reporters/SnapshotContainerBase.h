//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "GeneralReporter.h"
#include "TaggingInterface.h"
#include "libmesh/petsc_vector.h"
#include "libmesh/petsc_matrix.h"
#include "NonlinearSystemBase.h"

#include <map>

/**
 * Base class for storing and managing numerical data like solutions, residuals, and Jacobians.
 * The vectors are kept distributed with respect to the communicator of the application.
 * The whole snapshot vector is stored. The saving frequency can be defined using the `execute_on`
 * parameter.
 */
class SnapshotContainerBase : public GeneralReporter
{
public:
  static InputParameters validParams();
  SnapshotContainerBase(const InputParameters & parameters);

  /**
   * Storage for the snapshots.
   *
   * The underlying storage is unique_ptrs, but the public API
   * (read-only access) exposes just references.
   */
  class Snapshots : public UniqueStorage<NumericVector<Number>>
  {
  public:
    friend class SnapshotContainerBase;
  };

  virtual void initialSetup() override;
  virtual void initialize() override {}
  virtual void execute() override;
  virtual void finalize() override {}

  /**
   * Return the whole snapshot container
   * @return const std::vector<std::unique_ptr<NumericVector<Number>>>&
   */
  const Snapshots & getSnapshots() const { return _accumulated_data; }

  /**
   * Return one of the stored snapshot vectors
   * @param local_i The index of the locally stored numeric data container
   */
  const NumericVector<Number> & getSnapshot(unsigned int local_i) const;

  /**
   * Return the snapshot stream accumulated for a single tag on the multi-stream path.
   * @param tag_name The name of the tag whose snapshot stream is requested
   * @return const Snapshots&
   */
  const Snapshots & getSnapshots(const TagName & tag_name) const;

  /**
   * Return the tag names for which snapshot streams have been accumulated on the multi-stream
   * path. Empty when the container uses the single-stream default.
   * @return std::vector<TagName>
   */
  std::vector<TagName> getSnapshotTags() const;

protected:
  /**
   * Clone the current snapshot vector.
   * @return std::unique_ptr<NumericVector<Number>>
   */
  virtual std::unique_ptr<NumericVector<Number>> collectSnapshot() = 0;

  /**
   * Whether this container accumulates one snapshot stream per tag (the multi-stream path)
   * instead of the single default stream. Override to return true to opt in; the default false
   * preserves the single `collectSnapshot()` stream used by SolutionContainer and
   * JacobianContainer.
   * @return whether execute() should use the per-tag multi-stream path
   */
  virtual bool collectMultipleSnapshots() const { return false; }

  /**
   * Clone the current snapshot vectors for the multi-stream path, one per stream key. Only
   * called from execute() when collectMultipleSnapshots() returns true. The default returns an
   * empty map so single-stream derived classes need no implementation.
   * @return map from tag name to the cloned snapshot vector for that stream
   */
  virtual std::map<TagName, std::unique_ptr<NumericVector<Number>>> collectSnapshots();

  /// Dynamic container for snapshot vectors. We store pointers to make sure that the change in size
  /// comes with little overhead. This is a reference because we need it to be restartable for
  /// stochastic runs in batch mode.
  Snapshots & _accumulated_data;

  /// Per-tag snapshot streams for the multi-stream path, keyed by tag name. Empty when the
  /// container uses the single-stream default. This is a reference because we need it to be
  /// restartable for stochastic runs in batch mode.
  std::map<TagName, Snapshots> & _accumulated_data_streams;

  /// The nonlinear system's number whose solution shall be collected
  const unsigned int _nonlinear_system_number;
  /// Tolerance for comparing two snapshots. If comparison is below tolerance
  /// snapshot is not saved.
  const Real _save_tolerance;

private:
  /**
   * Copy a collected snapshot into a non-ghosted parallel PetscVector holding the same owned
   * entries at the same global positions. Collected snapshots may be ghosted or non-PetscVectors,
   * but the checkpoint serializer rejects both, so every stored snapshot is converted here first.
   * @param vector The collected snapshot vector to copy
   * @return a non-ghosted parallel PetscVector with identical values
   */
  static std::unique_ptr<NumericVector<Number>>
  cloneNonGhosted(const NumericVector<Number> & vector);
};

void dataStore(std::ostream & stream, SnapshotContainerBase::Snapshots & v, void * context);
void dataLoad(std::istream & stream, SnapshotContainerBase::Snapshots & v, void * context);

// The default map serializer only supports trivially copyable keys, so a TagName-keyed
// (std::string) map of snapshot streams needs these custom overloads.
void dataStore(std::ostream & stream,
               std::map<TagName, SnapshotContainerBase::Snapshots> & data,
               void * context);
void dataLoad(std::istream & stream,
              std::map<TagName, SnapshotContainerBase::Snapshots> & data,
              void * context);
