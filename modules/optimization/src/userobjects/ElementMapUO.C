//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

// MOOSE includes
#include "ElementMapUO.h"

InputParameters
ElementMapUO::validParams()
{
  InputParameters params = ElementUserObject::validParams();
  return params;
}

ElementMapUO::ElementMapUO(const InputParameters & parameters) : ElementUserObject(parameters) {}

void
ElementMapUO::execute()
{
  _elem_ids.insert(_current_elem->id());
}

void
ElementMapUO::threadJoin(const UserObject & in_uo)
{
  const ElementMapUO & uo = static_cast<const ElementMapUO &>(in_uo);
  _elem_ids.insert(uo._elem_ids.begin(), uo._elem_ids.end());
}

void
ElementMapUO::finalize()
{
  // Clear map
  _elem_to_index.clear();

  std::vector<dof_id_type> all_elem_ids(_elem_ids.begin(), _elem_ids.end());
  _communicator.allgather(all_elem_ids);

  for (auto i : index_range(all_elem_ids))
  {
    // if local processor owns element then add the element and index into
    if (_elem_ids.count(all_elem_ids[i]))
      _elem_to_index.insert({all_elem_ids[i], i});
  }
  _elem_ids.clear();
}
