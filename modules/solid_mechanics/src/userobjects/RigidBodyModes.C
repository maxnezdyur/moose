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
    _nl_sys(getVar("displacements", 0)->sys()),
    _sys_num(_nl_sys.number())
{
  if (_ndisp != 2 && _ndisp != 3)
    paramError("displacements",
               "The number of coupled displacement variables must be 2 for 2D or 3 for 3D; "
               "expected 2 or 3 but got ",
               _ndisp,
               ".");

  _disp_var_num.resize(_ndisp);
  for (const auto i : make_range(_ndisp))
    _disp_var_num[i] = getVar("displacements", i)->number();

  std::vector<VariableName> disp_names(_ndisp);
  for (const auto i : make_range(_ndisp))
    disp_names[i] = coupledName("displacements", i);

  _const_var_num.resize(_n_const);
  for (const auto k : make_range(_n_const))
  {
    const auto cname = coupledName("constant_mode_variables", k);
    if (std::find(disp_names.begin(), disp_names.end(), cname) != disp_names.end())
      paramError("constant_mode_variables",
                 "The variable '",
                 cname,
                 "' cannot appear in both 'displacements' and 'constant_mode_variables'.");
    _const_var_num[k] = getVar("constant_mode_variables", k)->number();
  }

  // Self-register the near-null-space subspace sized to the number of modes this object fills.
  _fe_problem.initNearNullSpaceVectors(_n_modes);
}

void
RigidBodyModes::initialize()
{
  for (const auto m : make_range(_n_modes))
  {
    auto & mode = _nl_sys.getVector("NearNullSpace_" + std::to_string(m));
    mode.zero();
    mode.close();
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
    _nl_sys.getVector("NearNullSpace_0").set(dx, 1.0);
    _nl_sys.getVector("NearNullSpace_1").set(dy, 1.0);

    if (_ndisp == 2)
    {
      // rotation about z: (-y, x)
      auto & rot_z = _nl_sys.getVector("NearNullSpace_2");
      rot_z.set(dx, -y);
      rot_z.set(dy, x);
    }
    else
    {
      const dof_id_type dz = node.dof_number(_sys_num, _disp_var_num[2], 0);
      _nl_sys.getVector("NearNullSpace_2").set(dz, 1.0);

      // rotations about the origin
      // rotation about x: (0, -z, y)
      auto & rot_x = _nl_sys.getVector("NearNullSpace_3");
      rot_x.set(dy, -z);
      rot_x.set(dz, y);
      // rotation about y: (z, 0, -x)
      auto & rot_y = _nl_sys.getVector("NearNullSpace_4");
      rot_y.set(dx, z);
      rot_y.set(dz, -x);
      // rotation about z: (-y, x, 0)
      auto & rot_z = _nl_sys.getVector("NearNullSpace_5");
      rot_z.set(dx, -y);
      rot_z.set(dy, x);
    }
  }

  // Constant modes: one per constant_mode_variables entry (indices _n_rbm .. _n_modes - 1),
  // each written only into its own variable's DOF.
  for (const auto k : index_range(_const_var_num))
    if (node.n_dofs(_sys_num, _const_var_num[k]) > 0)
    {
      const dof_id_type dc = node.dof_number(_sys_num, _const_var_num[k], 0);
      _nl_sys.getVector("NearNullSpace_" + std::to_string(_n_rbm + k)).set(dc, 1.0);
    }
}

void
RigidBodyModes::finalize()
{
  for (const auto m : make_range(_n_modes))
    _nl_sys.getVector("NearNullSpace_" + std::to_string(m)).close();
}
