//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "RigidBodyModes.h"

#include "FEProblemBase.h"
#include "MooseVariableFE.h"
#include "SystemBase.h"

#include "libmesh/numeric_vector.h"
#include "libmesh/node.h"
#include "libmesh/int_range.h"

#include <algorithm>

registerMooseObject("SolidMechanicsApp", RigidBodyModes);

InputParameters
RigidBodyModes::validParams()
{
  InputParameters params = NodalUserObject::validParams();
  params.addClassDescription(
      "Fills the framework near-null-space vectors with the rigid body modes of the displacement "
      "field plus one constant mode per additional field variable, for algebraic multigrid on "
      "unconstrained (floating) solid mechanics problems.");
  params.addRequiredCoupledVar("displacements",
                               "The displacement variables (2 for 2D, 3 for 3D).");
  params.addCoupledVar("constant_mode_variables",
                       "Additional field variables (for example temperature) that each receive a "
                       "single constant near-null-space mode. Must not overlap 'displacements'.");
  // The rigid body modes depend only on the reference coordinates, so they are computed once.
  params.set<ExecFlagEnum>("execute_on") = EXEC_INITIAL;
  return params;
}

RigidBodyModes::RigidBodyModes(const InputParameters & parameters)
  : NodalUserObject(parameters),
    _ndisp(coupledComponents("displacements")),
    _n_rbm(_ndisp == 2 ? 3 : 6),
    _n_const(coupledComponents("constant_mode_variables")),
    _n_modes(_n_rbm + _n_const),
    _nl_sys(nullptr),
    _sys_num(libMesh::invalid_uint)
{
  if (_ndisp != 2 && _ndisp != 3)
    paramError("displacements",
               "The number of coupled displacement variables must be 2 for 2D or 3 for 3D; "
               "expected 2 or 3 but got ",
               _ndisp,
               ".");

  // Resolved in the body rather than in the initializer list, which runs before the check above.
  // Every coupled entry is resolved through coupledFieldVariable(), which reports an error when
  // the entry is a constant value rather than a variable name
  _nl_sys = &coupledFieldVariable("displacements", 0).sys();
  _sys_num = _nl_sys->number();

  _disp_var_num.resize(_ndisp);
  for (const auto i : make_range(_ndisp))
  {
    const auto & disp_var = coupledFieldVariable("displacements", i);
    checkVariableSystem(disp_var, "displacements");
    _disp_var_num[i] = disp_var.number();
  }

  std::vector<VariableName> disp_names(_ndisp);
  for (const auto i : make_range(_ndisp))
    disp_names[i] = coupledName("displacements", i);

  _const_var_num.resize(_n_const);
  for (const auto k : make_range(_n_const))
  {
    const auto & const_var = coupledFieldVariable("constant_mode_variables", k);
    checkVariableSystem(const_var, "constant_mode_variables");
    if (std::find(disp_names.begin(), disp_names.end(), const_var.name()) != disp_names.end())
      paramError("constant_mode_variables",
                 "The variable '",
                 const_var.name(),
                 "' cannot appear in both 'displacements' and 'constant_mode_variables'.");
    _const_var_num[k] = const_var.number();
  }

  // Self-register the near-null-space subspace sized to the number of modes this object fills.
  _fe_problem.initNearNullSpaceVectors(_n_modes);
}

MooseVariableFieldBase &
RigidBodyModes::coupledFieldVariable(const std::string & param, const unsigned int i)
{
  auto * const var = getVar(param, i);
  if (!var)
    paramError(param,
               "Every entry must be a variable name, but entry ",
               i,
               " is a constant value. A constant cannot carry a near-null-space mode.");
  return *var;
}

void
RigidBodyModes::checkVariableSystem(const MooseVariableFieldBase & var,
                                    const std::string & param) const
{
  if (var.sys().number() != _sys_num)
    paramError(param,
               "The variable '",
               var.name(),
               "' belongs to system '",
               var.sys().name(),
               "', but the near-null-space vectors are filled on system '",
               _nl_sys->name(),
               "'. Every variable coupled to this object must belong to that system, because its "
               "variable number is resolved against that system when the modes are written. An "
               "auxiliary variable cannot carry a near-null-space mode.");
}

void
RigidBodyModes::initialize()
{
  // Resolved once here rather than by name on every node, because execute() runs per local node
  _modes.resize(_n_modes);
  for (const auto m : make_range(_n_modes))
  {
    _modes[m] = &_nl_sys->getVector("NearNullSpace_" + std::to_string(m));
    _modes[m]->zero();
    _modes[m]->close();
  }
}

void
RigidBodyModes::execute()
{
  if (_current_node->processor_id() != processor_id())
    return;

  const Node & node = *_current_node;
  const Real x = node(0);
  const Real y = node(1);
  const Real z = (_ndisp == 3) ? node(2) : 0.0;

  // Rigid body modes: write only into the displacement DOFs (indices 0 .. _n_rbm - 1). The modes
  // couple all displacement components, so they are only well defined at a node that carries every
  // component; require all of them rather than assuming the components share nodal support.
  bool all_disp_dofs = true;
  for (const auto i : make_range(_ndisp))
    if (node.n_dofs(_sys_num, _disp_var_num[i]) == 0)
    {
      all_disp_dofs = false;
      break;
    }
  if (all_disp_dofs)
  {
    const dof_id_type dx = node.dof_number(_sys_num, _disp_var_num[0], 0);
    const dof_id_type dy = node.dof_number(_sys_num, _disp_var_num[1], 0);

    // translations
    _modes[0]->set(dx, 1.0);
    _modes[1]->set(dy, 1.0);

    if (_ndisp == 2)
    {
      // rotation about z: (-y, x)
      _modes[2]->set(dx, -y);
      _modes[2]->set(dy, x);
    }
    else
    {
      const dof_id_type dz = node.dof_number(_sys_num, _disp_var_num[2], 0);
      _modes[2]->set(dz, 1.0);

      // rotations about the origin
      // rotation about x: (0, -z, y)
      _modes[3]->set(dy, -z);
      _modes[3]->set(dz, y);
      // rotation about y: (z, 0, -x)
      _modes[4]->set(dx, z);
      _modes[4]->set(dz, -x);
      // rotation about z: (-y, x, 0)
      _modes[5]->set(dx, -y);
      _modes[5]->set(dy, x);
    }
  }

  // Constant modes: one per constant_mode_variables entry (indices _n_rbm .. _n_modes - 1),
  // each written only into its own variable's DOF.
  for (const auto k : index_range(_const_var_num))
    if (node.n_dofs(_sys_num, _const_var_num[k]) > 0)
    {
      const dof_id_type dc = node.dof_number(_sys_num, _const_var_num[k], 0);
      _modes[_n_rbm + k]->set(dc, 1.0);
    }
}

void
RigidBodyModes::finalize()
{
  for (auto * const mode : _modes)
    mode->close();
}
