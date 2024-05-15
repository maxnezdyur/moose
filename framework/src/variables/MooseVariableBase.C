//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "MooseVariableBase.h"

#include "AddVariableAction.h"
#include "SubProblem.h"
#include "SystemBase.h"
#include "MooseMesh.h"
#include "MooseApp.h"
#include "InputParameterWarehouse.h"
#include "BlockRestrictable.h"

#include "libmesh/variable.h"
#include "libmesh/dof_map.h"
#include "libmesh/system.h"
#include "libmesh/fe_type.h"
#include "libmesh/string_to_enum.h"

// Users should never actually create this object
registerMooseObject("MooseApp", MooseVariableBase);

InputParameters
MooseVariableBase::validParams()
{
  InputParameters params = MooseObject::validParams();
  params += BlockRestrictable::validParams();
  params += OutputInterface::validParams();

  MooseEnum order(
      "CONSTANT FIRST SECOND THIRD FOURTH FIFTH SIXTH SEVENTH EIGHTH NINTH TENTH ELEVENTH TWELFTH "
      "THIRTEENTH FOURTEENTH FIFTEENTH SIXTEENTH SEVENTEENTH EIGHTTEENTH NINETEENTH TWENTIETH "
      "TWENTYFIRST TWENTYSECOND TWENTYTHIRD TWENTYFOURTH TWENTYFIFTH TWENTYSIXTH TWENTYSEVENTH "
      "TWENTYEIGHTH TWENTYNINTH THIRTIETH THIRTYFIRST THIRTYSECOND THIRTYTHIRD THIRTYFOURTH "
      "THIRTYFIFTH THIRTYSIXTH THIRTYSEVENTH THIRTYEIGHTH THIRTYNINTH FORTIETH FORTYFIRST "
      "FORTYSECOND FORTYTHIRD",
      "FIRST",
      true);
  params.addParam<MooseEnum>("order",
                             order,
                             "Order of the FE shape function to use for this variable (additional "
                             "orders not listed here are allowed, depending on the family).");

  MooseEnum family{AddVariableAction::getNonlinearVariableFamilies()};

  params.addParam<MooseEnum>(
      "family", family, "Specifies the family of FE shape functions to use for this variable.");

  // ArrayVariable capability
  params.addRangeCheckedParam<unsigned int>(
      "components", 1, "components>0", "Number of components for an array variable");

  // Advanced input options
  params.addParam<std::vector<Real>>("scaling",
                                     "Specifies a scaling factor to apply to this variable");
  params.addParam<bool>("eigen", false, "True to make this variable an eigen variable");
  params.addParam<bool>("fv", false, "True to make this variable a finite volume variable");
  params.addParam<bool>("array",
                        false,
                        "True to make this variable a array variable regardless of number of "
                        "components. If 'components' > 1, this will automatically be set to true.");
  params.addParam<NonlinearSystemName>("nl_sys",
                                       "nl0",
                                       "If this variable is a nonlinear variable, this is the "
                                       "nonlinear system to which it should be added.");
  params.addParamNamesToGroup("scaling eigen", "Advanced");

  params.addParam<bool>("use_dual", false, "True to use dual basis for Lagrange multipliers");

  params.registerBase("MooseVariableBase");
  params.addPrivateParam<SystemBase *>("_system_base");
  params.addPrivateParam<FEProblemBase *>("_fe_problem_base");
  params.addPrivateParam<Moose::VarKindType>("_var_kind");
  params.addPrivateParam<unsigned int>("_var_num");
  params.addPrivateParam<THREAD_ID>("tid");

  params.addClassDescription(
      "Base class for Moose variables. This should never be the terminal object type");
  return params;
}

MooseVariableBase::MooseVariableBase(const InputParameters & parameters)
  : MooseObject(parameters),
    BlockRestrictable(this),
    OutputInterface(parameters),
    SetupInterface(this),
    _sys(*getParam<SystemBase *>("_system_base")), // TODO: get from _fe_problem_base
    _fe_type(Utility::string_to_enum<Order>(getParam<MooseEnum>("order")),
             Utility::string_to_enum<FEFamily>(getParam<MooseEnum>("family"))),
    _var_num(getParam<unsigned int>("_var_num")),
    _is_eigen(getParam<bool>("eigen")),
    _var_kind(getParam<Moose::VarKindType>("_var_kind")),
    _subproblem(_sys.subproblem()),
    _variable(_sys.system().variable(_var_num)),
    _assembly(_subproblem.assembly(getParam<THREAD_ID>("_tid"),
                                   _var_kind == Moose::VAR_NONLINEAR ? _sys.number() : 0)),
    _dof_map(_sys.dofMap()),
    _mesh(_subproblem.mesh()),
    _tid(getParam<THREAD_ID>("tid")),
    _count(getParam<unsigned int>("components")),
    _scaling_factor(_count, 1.0),
    _use_dual(getParam<bool>("use_dual")),
    _is_array(getParam<bool>("array"))
{
  scalingFactor(isParamValid("scaling") ? getParam<std::vector<Real>>("scaling")
                                        : std::vector<Real>(_count, 1.));
  if (getParam<bool>("fv") && getParam<bool>("eigen"))
    paramError("eigen", "finite volume (fv=true) variables do not have eigen support");
  if (getParam<bool>("fv") && _fe_type.family != MONOMIAL)
    paramError("family", "finite volume (fv=true) variables must be have MONOMIAL family");
  if (getParam<bool>("fv") && _fe_type.order != 0)
    paramError("order", "finite volume (fv=true) variables currently support CONST order only");
  if (_count > 1)
    mooseAssert(_is_array, "Must be true with component > 1");
  if (_is_array)
  {
    auto name0 = _sys.system().variable(_var_num).name();
    std::size_t found = name0.find_last_of("_");
    if (found == std::string::npos)
      mooseError("Error creating ArrayMooseVariable name with base name ", name0);
    _var_name = name0.substr(0, found);
  }
  else
  {
    mooseAssert(_count == 1, "component size of normal variable (_count) must be one");
    _var_name = _sys.system().variable(_var_num).name();
  }

  if (!blockRestricted())
    _is_lower_d = false;
  else
  {
    const auto & blk_ids = blockIDs();
    mooseAssert(!blk_ids.empty(), "Every variable should have at least one subdomain");

    _is_lower_d = _mesh.isLowerD(*blk_ids.begin());
#ifdef DEBUG
    for (auto it = ++blk_ids.begin(); it != blk_ids.end(); ++it)
      mooseAssert(_is_lower_d == _mesh.isLowerD(*it),
                  "A user should not specify a mix of lower-dimensional and higher-dimensional "
                  "blocks for a variable");
#endif
  }
}

const std::vector<dof_id_type> &
MooseVariableBase::allDofIndices() const
{
  const auto it = _sys.subproblem()._var_dof_map.find(name());
  if (it != _sys.subproblem()._var_dof_map.end())
    return it->second;
  else
    mooseError("VariableAllDoFMap not prepared for ",
               name(),
               " . Check nonlocal coupling requirement for the variable.");
}

Order
MooseVariableBase::order() const
{
  return _fe_type.order;
}

std::vector<dof_id_type>
MooseVariableBase::componentDofIndices(const std::vector<dof_id_type> & dof_indices,
                                       unsigned int component) const
{
  // Initialize the new_dof_indices vector with the input dof_indices
  std::vector<dof_id_type> new_dof_indices(dof_indices);

  // Only perform mapping if the component is not 0
  if (component != 0)
  {
    // Grab all DoFs for component 0; this will be used as a reference
    std::vector<dof_id_type> comp_0_dofs;
    _dof_map.local_variable_indices(comp_0_dofs, _mesh, _var_num);

    // Determine the variable number for the specified component
    auto comp_num = _var_num + component;

    // Grab all DoFs for the specified component
    std::vector<dof_id_type> comp_n_dofs;
    _dof_map.local_variable_indices(comp_n_dofs, _mesh, comp_num);

    // Create a map to store the index of each DoF in comp_0_dofs
    std::unordered_map<dof_id_type, size_t> index_map;
    for (size_t i = 0; i < comp_0_dofs.size(); ++i)
    {
      index_map[comp_0_dofs[i]] = i;
    }
    // We expect that the ordering of the dofs from one compoent to another
    // is the same, relatively.
    // Map the input dof_indices to the corresponding DoFs in comp_n_dofs
    for (size_t i = 0; i < dof_indices.size(); ++i)
    {
      // Find the index of the current DoF in comp_0_dofs using the map
      auto it = index_map.find(dof_indices[i]);
      if (it != index_map.end())
      {
        // Use the found index to get the corresponding DoF in comp_n_dofs
        new_dof_indices[i] = comp_n_dofs[it->second];
      }
      else
      {
        mooseError("DoF not found in comp_0_dofs");
      }
    }
  }

  return new_dof_indices;
}

void
MooseVariableBase::scalingFactor(const std::vector<Real> & factor)
{
  mooseAssert(factor.size() == _count, "Inconsistent scaling factor size");
  for (const auto i : make_range(_count))
    _scaling_factor[i] = factor[i];
}

void
MooseVariableBase::initialSetup()
{
  // Currently the scaling vector is only used through AD residual computing objects
  if ((_var_kind == Moose::VAR_NONLINEAR) && _subproblem.haveADObjects() &&
      (_subproblem.automaticScaling() || (std::find_if(_scaling_factor.begin(),
                                                       _scaling_factor.end(),
                                                       [](const Real element) {
                                                         return !MooseUtils::absoluteFuzzyEqual(
                                                             element, 1.);
                                                       }) != _scaling_factor.end())))

    _sys.addScalingVector();
}
