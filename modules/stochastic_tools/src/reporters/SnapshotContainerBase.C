//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "SnapshotContainerBase.h"

#include "libmesh/int_range.h"
#include "libmesh/utility.h"

InputParameters
SnapshotContainerBase::validParams()
{
  InputParameters params = GeneralReporter::validParams();
  params.addParam<NonlinearSystemName>(
      "nonlinear_system_name",
      "nl0",
      "Option to select which nonlinear system's solution shall be stored.");
  params.addParam<Real>("save_tolerance", 1e-8, "Tolerance for determining a duplicate snapshot.");
  return params;
}

SnapshotContainerBase::SnapshotContainerBase(const InputParameters & parameters)
  : GeneralReporter(parameters),
    _accumulated_data(
        declareRestartableDataWithContext<Snapshots>("accumulated_snapshots", (void *)&comm())),
    _accumulated_data_streams(declareRestartableDataWithContext<std::map<TagName, Snapshots>>(
        "accumulated_snapshot_streams", (void *)&comm())),
    _nonlinear_system_number(
        _fe_problem.nlSysNum(getParam<NonlinearSystemName>("nonlinear_system_name"))),
    _save_tolerance(getParam<Real>("save_tolerance"))
{
}

void
SnapshotContainerBase::initialSetup()
{
  _accumulated_data.clear();
  _accumulated_data_streams.clear();
}

const NumericVector<Number> &
SnapshotContainerBase::getSnapshot(unsigned int local_i) const
{
  mooseAssert(local_i < _accumulated_data.size(),
              "The container only has (" + std::to_string(_accumulated_data.size()) +
                  ") solutions so we cannot find any with index (" + std::to_string(local_i) +
                  ")!");
  return _accumulated_data[local_i];
}

const SnapshotContainerBase::Snapshots &
SnapshotContainerBase::getSnapshots(const TagName & tag_name) const
{
  return libmesh_map_find(_accumulated_data_streams, tag_name);
}

std::vector<TagName>
SnapshotContainerBase::getSnapshotTags() const
{
  std::vector<TagName> tags;
  tags.reserve(_accumulated_data_streams.size());
  for (const auto & [tag_name, _] : _accumulated_data_streams)
    tags.push_back(tag_name);
  return tags;
}

std::map<TagName, std::unique_ptr<NumericVector<Number>>>
SnapshotContainerBase::collectSnapshots()
{
  return {};
}

std::unique_ptr<NumericVector<Number>>
SnapshotContainerBase::cloneNonGhosted(const NumericVector<Number> & vector)
{
  // The checkpoint serializer requires a non-ghosted PetscVector, so build one directly rather
  // than cloning. Use the owned index range for the local size so the copy carries no ghosting
  // regardless of the source vector's type.
  const dof_id_type local_size = vector.last_local_index() - vector.first_local_index();
  auto result = std::make_unique<libMesh::PetscVector<Number>>(
      vector.comm(), vector.size(), local_size, libMesh::PARALLEL);
  for (const auto dof : make_range(vector.first_local_index(), vector.last_local_index()))
    result->set(dof, vector(dof));
  result->close();
  return result;
}

void
SnapshotContainerBase::execute()
{
  // Store a non-ghosted PetscVector copy of each collected snapshot so it is serializable for
  // checkpoint/restart (the collected vectors may be ghosted or non-PetscVectors).
  if (collectMultipleSnapshots())
    // One entry per timestep per stream.
    for (auto & [tag_name, snapshot] : collectSnapshots())
      _accumulated_data_streams[tag_name].addPointer(cloneNonGhosted(*snapshot));
  else
    _accumulated_data.addPointer(cloneNonGhosted(*collectSnapshot()));
}

void
dataStore(std::ostream & stream, SnapshotContainerBase::Snapshots & v, void * context)
{
  storeHelper(stream, static_cast<UniqueStorage<NumericVector<Number>> &>(v), context);
}

void
dataLoad(std::istream & stream, SnapshotContainerBase::Snapshots & v, void * context)
{
  loadHelper(stream, static_cast<UniqueStorage<NumericVector<Number>> &>(v), context);
}

void
dataStore(std::ostream & stream,
          std::map<TagName, SnapshotContainerBase::Snapshots> & data,
          void * context)
{
  unsigned int size = data.size();
  dataStore(stream, size, nullptr);
  for (auto & [tag_name, snapshots] : data)
  {
    // TagName is a derivative-string type with no dataStore overload of its own, so serialize the
    // key through a std::string to select the std::string overload.
    std::string key = tag_name;
    dataStore(stream, key, context);
    dataStore(stream, snapshots, context);
  }
}

void
dataLoad(std::istream & stream,
         std::map<TagName, SnapshotContainerBase::Snapshots> & data,
         void * context)
{
  data.clear();
  unsigned int size = 0;
  dataLoad(stream, size, nullptr);
  for (unsigned int i = 0; i < size; ++i)
  {
    // Load the key as a std::string to select the std::string overload, then key the map by the
    // TagName constructed from it.
    std::string key;
    dataLoad(stream, key, context);
    dataLoad(stream, data[TagName(key)], context);
  }
}
