//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "NodalUserObject.h"

class SystemBase;

/**
 * Fills the framework near-null-space vectors with the rigid body modes of the
 * displacement field (translations and rotations about the origin, evaluated at
 * the reference coordinates) plus one constant mode per additional field variable.
 *
 * The object self-registers the near-null-space subspace from its constructor, so no
 * near_null_space_dimension needs to be set on the Problem. Rigid body modes are written
 * only into the displacement DOFs and each constant mode is written only into its own
 * variable's DOFs, which keeps the subspace valid when the displacement field is coupled
 * to extra fields such as temperature.
 */
class RigidBodyModes : public NodalUserObject
{
public:
  static InputParameters validParams();

  RigidBodyModes(const InputParameters & parameters);

  virtual void initialize() override;
  virtual void execute() override;
  virtual void threadJoin(const UserObject &) override {}
  virtual void finalize() override;

protected:
  /// Number of coupled displacement components (2 or 3)
  const unsigned int _ndisp;
  /// Number of rigid body modes (3 in 2D, 6 in 3D)
  const unsigned int _n_rbm;
  /// Number of constant modes (one per constant_mode_variables entry)
  const unsigned int _n_const;
  /// Total number of near-null-space modes filled by this object
  const unsigned int _n_modes;

  /// Nonlinear system that owns the displacement variables and the near-null-space vectors
  SystemBase & _nl_sys;
  /// libMesh system number used to resolve DOFs on that system
  const unsigned int _sys_num;

  /// libMesh variable numbers of the displacement variables
  std::vector<unsigned int> _disp_var_num;
  /// libMesh variable numbers of the constant-mode variables
  std::vector<unsigned int> _const_var_num;
};
