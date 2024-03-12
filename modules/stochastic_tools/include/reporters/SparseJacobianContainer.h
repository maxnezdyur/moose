//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "SnapshotContainerBase.h"
#include "libmesh/id_types.h"
#include "libmesh/petsc_vector.h"
#include "libmesh/vectormap.h"

/**
 * This class is responsible for collecting jacobian row concatenated vectors in one place. The
 * vectors are kept distributed with respect to the communicator of the application.
 * The whole jacobian row concatenated vector is stored.
 * The saving frequency can be defined using the `execute_on` parameter.
 */
class SparseJacobianContainer : public SnapshotContainerBase
{
public:
  static InputParameters validParams();
  SparseJacobianContainer(const InputParameters & parameters);

protected:
  virtual std::unique_ptr<NumericVector<Number>> collectSnapshot() override;
  /// What output to return, the extreme value, or the time it occurred
  enum class StorageType
  {
    Values,
    Indices
  };
  StorageType _storage_type;
};
