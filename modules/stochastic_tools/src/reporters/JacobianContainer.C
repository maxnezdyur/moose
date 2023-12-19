//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "JacobianContainer.h"
#include "NonlinearSystemBase.h"
#include "NonlinearSystem.h"
#include "libmesh/id_types.h"
#include "libmesh/distributed_vector.h"

registerMooseObject("StochasticToolsApp", JacobianContainer);

InputParameters
JacobianContainer::validParams()
{
  InputParameters params = SnapshotContainerBase::validParams();
  params.addClassDescription("Class responsible for collecting distributed jacobian row "
                             "concatenated vectors into a container. We append "
                             "a new distributed jacobian row concatenated at every execution.");

  return params;
}

JacobianContainer::JacobianContainer(const InputParameters & parameters)
  : SnapshotContainerBase(parameters)
{
}

std::unique_ptr<NumericVector<Number>>
JacobianContainer::collectSnapshot()
{

  auto & jac = static_cast<ImplicitSystem &>(
                   _fe_problem.getNonlinearSystem(_nonlinear_system_number).system())
                   .get_system_matrix();

  auto num_rows = jac.m();
  auto num_cols = jac.n();
  auto num_local_rows = jac.local_m();
  DistributedVector<Real> flattened_jac(
      _communicator, num_cols * num_rows, num_local_rows * num_cols);

  // Need to close the matrix for NONLINEAR
  jac.close();
  for (dof_id_type row = jac.row_start(); row < jac.row_stop(); row++)
  {
    std::vector<Real> values;
    std::vector<dof_id_type> cols;
    jac.get_row(row, cols, values);

    // Adjust column indices based on the row
    for (auto & col : cols)
    {
      col = row * num_cols + col; // Update column index for concatenated vector
    }

    flattened_jac.add_vector(values, cols);
  }

  // Clone the residual of the correct system
  return flattened_jac.clone();
}
