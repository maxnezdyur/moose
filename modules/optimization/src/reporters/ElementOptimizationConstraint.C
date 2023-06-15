//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ElementOptimizationConstraint.h"
#include "MooseError.h"
#include "libmesh/libmesh_common.h"
#include <cstddef>
#include <vector>

InputParameters
ElementOptimizationConstraint::validParams()
{
  InputParameters params = ElementReporter::validParams();
  params.addRequiredParam<UserObjectName>("element_map", "Element vector map user object");
  params.addParam<std::string>("base_name", "Name to append to reporters.");
  return params;
}

ElementOptimizationConstraint::ElementOptimizationConstraint(const InputParameters & parameters)
  : ElementReporter(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _constraint(declareValueByName<Real>(_base_name + "constraint")),
    _constraint_jacobian(declareValueByName<std::vector<Real>>(_base_name + "constraint_jacobian")),
    _elem_vec_map(getUserObject<ElementMapUO>("element_map").getElementVectorMap())
{
}
void
ElementOptimizationConstraint::initialize()
{
  // resize to the local size
  _constraint_jacobian.resize(_elem_vec_map.size());
  _constraint = 0;
}

void
ElementOptimizationConstraint::execute()
{
  _constraint += computeQpConstraint();
  std::size_t local_index;
  auto it = _elem_vec_map.find(_current_elem->id());
  if (it != _elem_vec_map.end())
  {
    local_index = it->second.local_index;
  }
  else
  {
    mooseError("Element Vector Map did not find the element.");
  }
  _constraint_jacobian[local_index] = computeQpConstraintJacobian();
}

void
ElementOptimizationConstraint::finalize()
{
  _communicator.sum(_constraint);

  //   The ElementVectorMap makes sure that the local vectors are in the correct
  //   order on each processor so that the gather will setup the global vector
  //   in the correct order.
  _communicator.gather(0, _constraint_jacobian);
}
