//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "MooseEnum.h"
#include "SparseJacobianContainer.h"
#include "NonlinearSystemBase.h"
#include "NonlinearSystem.h"
#include "libmesh/id_types.h"
#include "libmesh/distributed_vector.h"
#include "libmesh/nonlinear_implicit_system.h"
#include "libmesh/petsc_vector.h"

registerMooseObject("StochasticToolsApp", SparseJacobianContainer);

InputParameters
SparseJacobianContainer::validParams()
{
  InputParameters params = SnapshotContainerBase::validParams();
  params.addClassDescription("Class responsible for collecting distributed jacobian row "
                             "concatenated vectors into a container. We append "
                             "a new distributed jacobian row concatenated at every execution.");
  // Define the extreme value/time enumeration
  MooseEnum storage_type("values=0 indices=1", "values");
  params.addRequiredParam<MooseEnum>(
      "storage_type",
      storage_type,
      "Whether to store the values of the jacobian or individual value.");
  return params;
}

SparseJacobianContainer::SparseJacobianContainer(const InputParameters & parameters)
  : SnapshotContainerBase(parameters),
    _storage_type(getParam<MooseEnum>("storage_type").getEnum<StorageType>())
{
}

// Function to collect a snapshot of the current state of the Jacobian matrix
std::unique_ptr<NumericVector<Number>>
SparseJacobianContainer::collectSnapshot()
{
  auto nl_iter = static_cast<NonlinearImplicitSystem &>(
                     _fe_problem.getNonlinearSystem(_nonlinear_system_number).system())
                     .get_current_nonlinear_iteration_number();
  auto exec_flag = _fe_problem.getCurrentExecuteOnFlag();

  // if the nl_iter is less than 1, the jacobian will always be empty, but at
  // the end we need to grab the jacobian.
  if (nl_iter < 1 && exec_flag != EXEC_FINAL && exec_flag != EXEC_TIMESTEP_END)
  {
    return DistributedVector<Real>(_communicator, 0, 0).clone();
  }
  // Obtain a reference to the Jacobian matrix from the nonlinear system
  auto & jac = static_cast<ImplicitSystem &>(
                   _fe_problem.getNonlinearSystem(_nonlinear_system_number).system())
                   .get_system_matrix();

  // Retrieve the dimensions of the Jacobian matrix
  auto num_cols = jac.n();

  // Close the Jacobian matrix to finalize its state for NONLINEAR systems
  jac.close();

  // Vectors to store the non-zero values and their corresponding indices
  std::vector<Real> indices;
  std::vector<Real> nnz_vals;

  // Iterate over the rows of the Jacobian matrix within the local domain
  for (dof_id_type row = jac.row_start(); row < jac.row_stop(); row++)
  {
    // Temporary vectors to hold the non-zero values and column indices for the current row
    std::vector<Real> values;
    std::vector<dof_id_type> cols;

    // Extract the non-zero values and their column indices for the current row
    jac.get_row(row, cols, values);

    // Adjust the column indices based on the row, for a flattened vector representation
    for (auto & col : cols)
    {
      col = row * num_cols + col; // Update column index
    }

    // Append the adjusted column indices and their corresponding values to the main vectors
    indices.insert(indices.end(), cols.begin(), cols.end());
    nnz_vals.insert(nnz_vals.end(), values.begin(), values.end());
  }

  // Calculate the total size of the non-zero elements across all processors
  dof_id_type total_size = nnz_vals.size();
  _communicator.sum(total_size);

  // Create distributed vectors for storing the non-zero values and their indices
  DistributedVector<Real> sparse_vector(_communicator, total_size, nnz_vals.size());

  // Vector to hold the distributed indices for the non-zero values
  std::vector<dof_id_type> dist_ind(nnz_vals.size());
  // Initialize the distributed indices with consecutive values starting from the first local index
  std::iota(dist_ind.begin(), dist_ind.end(), sparse_vector.first_local_index());

  if (_storage_type == StorageType::Values)
    // Add the non-zero values to the distributed vectors
    sparse_vector.add_vector(nnz_vals, dist_ind);
  else
    // Add the adjusted indices to the distributed vectors
    sparse_vector.add_vector(indices, dist_ind);

  // Return a clone of the flattened Jacobian vector containing the non-zero values
  return sparse_vector.clone();
}
