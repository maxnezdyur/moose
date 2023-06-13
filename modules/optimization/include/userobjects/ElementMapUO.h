//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

// MOOSE includes
#include "ElementUserObject.h"
#include "libmesh/id_types.h"
#include <cstddef>
#include <unordered_map>
#include <utility>
#include <vector>
#include "libmesh/int_range.h"
#include <set>
/**
Setup a element to vector index map that will allow for TAO optimization of
elemental variables.
 */
class ElementMapUO : public ElementUserObject
{
public:
  static InputParameters validParams();

  ElementMapUO(const InputParameters & parameters);

  virtual void initialize() override{};
  virtual void execute() override;
  virtual void threadJoin(const UserObject & in_uo) override;
  virtual void finalize() override;

  using ElementVectorMap = std::unordered_map<dof_id_type, std::size_t>;
  const ElementVectorMap & getElementVectorMap() const { return _elem_to_index; }

protected:
  ElementVectorMap _elem_to_index;
  std::set<dof_id_type> _elem_ids;
};
