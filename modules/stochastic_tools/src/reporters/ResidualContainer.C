//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "NonlinearSystem.h"
#include "ResidualContainer.h"
#include "NonlinearSystemBase.h"

registerMooseObject("StochasticToolsApp", ResidualContainer);

InputParameters
ResidualContainer::validParams()
{
  InputParameters params = SnapshotContainerBase::validParams();
  params.addClassDescription(
      "Class responsible for collecting distributed residual vectors into a container. We append "
      "a new distributed residual vector at every execution.");

  return params;
}

ResidualContainer::ResidualContainer(const InputParameters & parameters)
  : SnapshotContainerBase(parameters)
{
}

std::unique_ptr<NumericVector<Number>>
ResidualContainer::collectSnapshot()
{
  // Clone the residual of the correct system
  return _fe_problem.getNonlinearSystem(_nonlinear_system_number).RHS().clone();
}
