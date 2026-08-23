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
   * Reject problems that hold Constraint objects, then store the PETSc options that engage the
   * requested solver on this system.
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

private:
  /// The PETSc domain decomposition solver to engage, either 'bddc' or 'fetidp'
  const MooseEnum _method;
};
