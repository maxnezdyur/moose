//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "SingleMatrixPreconditioner.h"
#include "MooseEnum.h"

/**
 * Preconditioner that maps onto PETSc's non-overlapping domain decomposition solvers, PCBDDC and
 * KSPFETIDP.
 *
 * Both solvers require the operator in PETSc's globally unassembled MATIS format, so the
 * constructor registers a PetscMatrixIS as the nonlinear system matrix. The decomposition has
 * exactly one subdomain per MPI process; the subdomain is defined by the local-to-global mapping
 * that PetscMatrixIS builds from the system's DofMap.
 */
class DomainDecompositionPreconditioner : public SingleMatrixPreconditioner
{
public:
  static InputParameters validParams();

  DomainDecompositionPreconditioner(const InputParameters & params);

  /**
   * Store the PETSc options that engage the requested solver on this system.
   *
   * The options without which a system of PDEs cannot be solved at all are stored here as
   * defaults: '-pc_bddc_use_change_of_basis', and '-fetidp_ksp_type gmres' for KSPFETIDP. Each
   * default is stored under its fully prefixed name: the nonlinear system prefix is prepended
   * when the system carries one, and with 'method = fetidp' the change of basis becomes
   * '-fetidp_bddc_pc_bddc_use_change_of_basis', because KSPFETIDP hands its inner PCBDDC the
   * 'fetidp_bddc_' options prefix. The defaults are stored after the options this object's own
   * parameters carry, and only for names those options do not already hold, so naming the fully
   * prefixed option in 'petsc_options' or in 'petsc_options_iname'/'petsc_options_value'
   * overrides the default. That includes turning one off, which is a pair such as
   * '-pc_bddc_use_change_of_basis false' ('-fetidp_bddc_pc_bddc_use_change_of_basis false' for
   * 'method = fetidp'); the unprefixed spelling does not match the stored default.
   */
  virtual void initialSetup() override;

  /**
   * Register the dofs of the saddle point variables on KSPFETIDP's inner PCBDDC.
   *
   * Runs before every solve rather than once: a system reinitialization (for example a contact
   * patch update changing the ghosting) rebuilds the PETSc solver and renumbers dofs, which
   * discards both the registered fields and KSPFETIDP's cached pressure set.
   */
  virtual void preSolve() override;

private:
  /**
   * Replace the linear solver after the subdomain matrix was remapped in place. PCBDDC caches
   * index sets sized for the subdomain it first saw and does not survive a reset on a resized one,
   * so the SNES is given a new KSP carrying the prefix, tolerances, norm type and convergence test
   * of the old one; its options are read again and the saddle point dofs are registered again on
   * the new inner PCBDDC.
   */
  void resetSolver();

  /**
   * Register the dofs of the saddle point variables on KSPFETIDP's inner PCBDDC, when there are
   * saddle point variables.
   */
  void registerSaddlePointDofs();

  /// The PETSc domain decomposition solver to engage, either 'bddc' or 'fetidp'
  const MooseEnum _method;

  /// Variables whose dofs form KSPFETIDP's saddle point pressure field, e.g. contact multipliers
  const std::vector<NonlinearVariableName> _saddle_point_vars;
};
