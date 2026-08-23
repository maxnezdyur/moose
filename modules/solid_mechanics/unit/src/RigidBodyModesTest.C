//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "gtest/gtest.h"

#include "MooseObjectUnitTest.h"
#include "MooseVariableFE.h"
#include "MooseMesh.h"
#include "NonlinearSystemBase.h"
#include "RigidBodyModes.h"

#include "libmesh/node.h"
#include "libmesh/numeric_vector.h"
#include "libmesh/int_range.h"

#include <array>

/**
 * Fixture: the 3D fixture mesh carrying a coupled displacement field (disp_x,
 * disp_y, disp_z) plus a temperature field, with a RigidBodyModes user object
 * that fills the framework near-null-space vectors. The equation systems are
 * initialized so the DOFs are numbered and the near-null-space vectors receive
 * storage, then the user object is driven over every local node exactly the way
 * ComputeNodalUserObjectsThread would, so the resulting vectors can be
 * inspected DOF by DOF.
 */
class RigidBodyModesTest : public MooseObjectUnitTest
{
public:
  RigidBodyModesTest() : MooseObjectUnitTest("SolidMechanicsApp") { buildAndRun(); }

protected:
  void buildAndRun()
  {
    InputParameters var_params = _factory.getValidParams("MooseVariable");
    var_params.set<MooseEnum>("family") = "LAGRANGE";
    var_params.set<MooseEnum>("order") = "FIRST";

    _fe_problem->addVariable("MooseVariable", "disp_x", var_params);
    _fe_problem->addVariable("MooseVariable", "disp_y", var_params);
    _fe_problem->addVariable("MooseVariable", "disp_z", var_params);
    _fe_problem->addVariable("MooseVariable", "temperature", var_params);

    InputParameters uo_params = _factory.getValidParams("RigidBodyModes");
    uo_params.set<std::vector<VariableName>>("displacements") = {"disp_x", "disp_y", "disp_z"};
    uo_params.set<std::vector<VariableName>>("constant_mode_variables") = {"temperature"};
    // Constructing the object self-registers the near-null-space subspace, which allocates the
    // NearNullSpace vectors on the nonlinear system. This must happen before es().init() so the
    // vectors receive storage when the system is initialized.
    _fe_problem->addUserObject("RigidBodyModes", "rbm", uo_params);

    _fe_problem->es().init();

    _nl = &_fe_problem->getNonlinearSystemBase(0);
    _sys_num = _nl->number();
    _disp_x_num = _fe_problem->getVariable(0, "disp_x").number();
    _disp_y_num = _fe_problem->getVariable(0, "disp_y").number();
    _disp_z_num = _fe_problem->getVariable(0, "disp_z").number();
    _temp_num = _fe_problem->getVariable(0, "temperature").number();

    auto & rbm = _fe_problem->getUserObject<RigidBodyModes>("rbm");
    rbm.initialize();
    for (const auto & node : _mesh->getMesh().local_node_ptr_range())
    {
      _fe_problem->reinitNode(node, 0);
      rbm.execute();
    }
    rbm.finalize();
  }

  NonlinearSystemBase * _nl = nullptr;
  unsigned int _sys_num = libMesh::invalid_uint;
  unsigned int _disp_x_num = libMesh::invalid_uint;
  unsigned int _disp_y_num = libMesh::invalid_uint;
  unsigned int _disp_z_num = libMesh::invalid_uint;
  unsigned int _temp_num = libMesh::invalid_uint;
};

// The six rigid body modes (indices 0 .. 5 in 3D) carry the expected translation and rotation
// values on the displacement DOFs and leave the temperature DOF at exactly zero.
TEST_F(RigidBodyModesTest, rigidModesFillDisplacementsAndSkipConstantField)
{
  for (const auto & node : _mesh->getMesh().local_node_ptr_range())
  {
    const Real x = (*node)(0);
    const Real y = (*node)(1);
    const Real z = (*node)(2);

    const auto dof_x = node->dof_number(_sys_num, _disp_x_num, 0);
    const auto dof_y = node->dof_number(_sys_num, _disp_y_num, 0);
    const auto dof_z = node->dof_number(_sys_num, _disp_z_num, 0);
    const auto dof_t = node->dof_number(_sys_num, _temp_num, 0);

    // Displacement components (disp_x, disp_y, disp_z) of the three translations followed by the
    // rotations about the x, y and z axes evaluated at the reference coordinates.
    const std::array<std::array<Real, 3>, 6> expected = {{{1.0, 0.0, 0.0},
                                                          {0.0, 1.0, 0.0},
                                                          {0.0, 0.0, 1.0},
                                                          {0.0, -z, y},
                                                          {z, 0.0, -x},
                                                          {-y, x, 0.0}}};

    for (const auto m : index_range(expected))
    {
      const auto & mode = _nl->getVector("NearNullSpace_" + std::to_string(m));
      EXPECT_DOUBLE_EQ(expected[m][0], mode(dof_x));
      EXPECT_DOUBLE_EQ(expected[m][1], mode(dof_y));
      EXPECT_DOUBLE_EQ(expected[m][2], mode(dof_z));
      // DOF safety: a rigid body mode must never touch the temperature DOF.
      EXPECT_DOUBLE_EQ(0.0, mode(dof_t));
    }
  }
}

// The single constant mode (index 6, following the six rigid body modes in 3D) is exactly one on
// every temperature DOF and exactly zero on every displacement DOF.
TEST_F(RigidBodyModesTest, constantModeFillsConstantFieldAndSkipsDisplacements)
{
  const auto & mode = _nl->getVector("NearNullSpace_6");

  for (const auto & node : _mesh->getMesh().local_node_ptr_range())
  {
    const auto dof_x = node->dof_number(_sys_num, _disp_x_num, 0);
    const auto dof_y = node->dof_number(_sys_num, _disp_y_num, 0);
    const auto dof_z = node->dof_number(_sys_num, _disp_z_num, 0);
    const auto dof_t = node->dof_number(_sys_num, _temp_num, 0);

    EXPECT_DOUBLE_EQ(1.0, mode(dof_t));
    EXPECT_DOUBLE_EQ(0.0, mode(dof_x));
    EXPECT_DOUBLE_EQ(0.0, mode(dof_y));
    EXPECT_DOUBLE_EQ(0.0, mode(dof_z));
  }
}
