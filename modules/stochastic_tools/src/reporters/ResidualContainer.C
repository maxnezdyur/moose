//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "Moose.h"
#include "MooseTypes.h"
#include "NonlinearSystem.h"
#include "ResidualContainer.h"
#include "NonlinearSystemBase.h"
#include "FEProblemBase.h"
#include "VectorTag.h"

#include "libmesh/int_range.h"

registerMooseObject("StochasticToolsApp", ResidualContainer);

InputParameters
ResidualContainer::validParams()
{
  InputParameters params = SnapshotContainerBase::validParams();
  params.addClassDescription(
      "Class responsible for collecting distributed residual vectors into a container. We append "
      "a new distributed residual vector at every execution.");
  params.addParam<TagName>(
      "tag_name",
      "Name of a single residual tag whose vector is collected into one snapshot stream. Mutually "
      "exclusive with 'tag_names'.");
  params.addParam<std::vector<TagName>>(
      "tag_names",
      "Names of the residual tags to collect, one snapshot stream per tag. When neither this nor "
      "'tag_name' is set, every residual vector tag stored by the nonlinear system is collected.");
  return params;
}

ResidualContainer::ResidualContainer(const InputParameters & parameters)
  : SnapshotContainerBase(parameters),
    _nl_sys(_fe_problem.getNonlinearSystem(_nonlinear_system_number)),
    _multi_stream(!parameters.isParamValid("tag_name") || parameters.isParamValid("tag_names")),
    _tag_names(getResidualTagNames(parameters, _fe_problem, _nl_sys)),
    _tag_ids(getResidualTagIds(_tag_names, _fe_problem))
{
  if (isParamValid("tag_name") && isParamValid("tag_names"))
    paramError("tag_names",
               "Provide either 'tag_name' (a single residual tag) or 'tag_names' (multiple "
               "residual tags), not both.");

  if (_multi_stream && _tag_names.empty())
    mooseError("No residual vector tags are available to collect. Set 'tag_name' or 'tag_names', "
               "or ensure the nonlinear system stores tagged residual vectors.");
}

std::vector<TagName>
ResidualContainer::getResidualTagNames(const InputParameters & parameters,
                                       const FEProblemBase & fe_problem,
                                       const SystemBase & nl_sys)
{
  if (parameters.isParamValid("tag_names"))
    return parameters.get<std::vector<TagName>>("tag_names");

  if (parameters.isParamValid("tag_name"))
    return {parameters.get<TagName>("tag_name")};

  // Neither given: default to every residual-type vector tag the nonlinear system stores, since
  // a tag without a stored vector cannot be snapshotted.
  std::vector<TagName> tag_names;
  for (const auto & vector_tag : fe_problem.getVectorTags(Moose::VECTOR_TAG_RESIDUAL))
    if (nl_sys.hasVector(vector_tag._id))
      tag_names.push_back(vector_tag._name);
  return tag_names;
}

std::vector<TagID>
ResidualContainer::getResidualTagIds(const std::vector<TagName> & tag_names,
                                     const SubProblem & subproblem)
{
  std::vector<TagID> tag_ids;
  tag_ids.reserve(tag_names.size());
  for (const auto & tag_name : tag_names)
    tag_ids.push_back(subproblem.getVectorTagID(tag_name));
  return tag_ids;
}

void
ResidualContainer::forceResidualEvaluation()
{
  if (_fe_problem.getCurrentExecuteOnFlag() == EXEC_TIMESTEP_BEGIN)
    _fe_problem.computeResidualL2Norm();
}

std::unique_ptr<NumericVector<Number>>
ResidualContainer::collectSnapshot()
{
  mooseAssert(!_tag_ids.empty(), "The single-stream path requires exactly one residual tag.");

  forceResidualEvaluation();

  return _nl_sys.getVector(_tag_ids.front()).clone();
}

std::map<TagName, std::unique_ptr<NumericVector<Number>>>
ResidualContainer::collectSnapshots()
{
  forceResidualEvaluation();

  std::map<TagName, std::unique_ptr<NumericVector<Number>>> snapshots;
  for (const auto i : index_range(_tag_names))
    snapshots.emplace(_tag_names[i], _nl_sys.getVector(_tag_ids[i]).clone());
  return snapshots;
}
