//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "DomainDecompositionPreconditioner.h"

// MOOSE includes
#include "FEProblem.h"
#include "MooseUtils.h"
#include "NonlinearSystemBase.h"
#include "PetscMatrixIS.h"
#include "PetscSupport.h"

#include "libmesh/implicit_system.h"
#include "libmesh/petsc_nonlinear_solver.h"
#include "libmesh/wrapped_petsc.h"

#include <petscksp.h>

#include <algorithm>

registerMooseObjectAliased("MooseApp", DomainDecompositionPreconditioner, "DDP");

InputParameters
DomainDecompositionPreconditioner::validParams()
{
  InputParameters params = SingleMatrixPreconditioner::validParams();
  params.addClassDescription("Preconditioner that decomposes the system into one non-overlapping "
                             "subdomain per process and solves it with PETSc's PCBDDC or "
                             "KSPFETIDP.");

  MooseEnum method("bddc fetidp", "bddc");
  params.addParam<MooseEnum>(
      "method", method, "The PETSc domain decomposition solver to precondition the system with.");

  params.addParam<std::vector<NonlinearVariableName>>(
      "saddle_point_variables",
      {},
      "Variables whose equations carry a zero diagonal when active, such as the Lagrange "
      "multipliers of mortar contact. Their dofs are handed to KSPFETIDP's saddle point support "
      "as the pressure field, so this requires 'method = fetidp'. The local subdomain problems "
      "become indefinite, so the inner direct solvers default to MUMPS, which pivots.");

  return params;
}

DomainDecompositionPreconditioner::DomainDecompositionPreconditioner(const InputParameters & params)
  : SingleMatrixPreconditioner(params),
    _method(getParam<MooseEnum>("method")),
    _saddle_point_vars(getParam<std::vector<NonlinearVariableName>>("saddle_point_variables"))
{
  if (const auto solve_type = _fe_problem.solverParams(_nl.number())._type;
      solve_type != Moose::ST_NEWTON && solve_type != Moose::ST_LINEAR)
    mooseError("Domain decomposition preconditioning must use a NEWTON or LINEAR solve type. "
               "This is because PCBDDC and KSPFETIDP build their coarse space and their subdomain "
               "solvers by factoring the assembled subdomain blocks of the operator. A solve type "
               "such as PJFNK never forms those blocks because it only provides the action of the "
               "Jacobian on a vector.");

  if (_fe_problem.useHashTableMatrixAssembly())
    mooseError("Domain decomposition preconditioning cannot use hash table matrix assembly. Hash "
               "table assembly accumulates the entries in a hash map and then copies them into the "
               "matrix, and PETSc provides no such copy for MATIS because the matrix is stored in "
               "globally unassembled form. Set 'use_hash_table_matrix_assembly = false' in the "
               "Problem block.");

  if (_fe_problem.restoreOriginalNonzeroPattern())
    mooseError("Domain decomposition preconditioning cannot restore the original nonzero pattern. "
               "Doing so calls MatResetPreallocation, which PETSc does not implement for MATIS "
               "because MATIS is preallocated through the local-to-global mapping of the subdomain "
               "rather than from a global nonzero pattern. Set 'restore_original_nonzero_pattern = "
               "false' in the Problem block.");

  auto & sys = _nl.system();
  if (!dynamic_cast<libMesh::ImplicitSystem *>(&sys))
    mooseError("Domain decomposition preconditioning can only be used with implicit systems, "
               "because only those adopt the matrix registered here as their system matrix.");

  if (sys.have_matrix("System Matrix"))
    mooseError("Domain decomposition preconditioning cannot register its system matrix on system "
               "'",
               sys.name(),
               "', because a matrix named 'System Matrix' is already registered there and "
               "replacing it would destroy an object the system still refers to. The likely cause "
               "is static condensation, which registers its own system matrix; that combination is "
               "not supported.");

  // This runs before the system is initialized, so ImplicitSystem::add_matrices() finds this
  // matrix under the name it would otherwise build a default matrix for and adopts it
  auto matrix = std::make_unique<PetscMatrixIS>(sys.comm(), sys.get_mesh());
  matrix->setSubdomainRemapCallback([this]() { resetSolver(); });
  sys.add_matrix("System Matrix", std::move(matrix), libMesh::PARALLEL);
}

void
DomainDecompositionPreconditioner::initialSetup()
{
  // With a NEWTON or LINEAR solve the operator and the preconditioning matrix are both the MATIS
  // registered by the constructor, which satisfies PCBDDC's requirement that the preconditioning
  // matrix be MATIS as well as KSPFETIDP's requirement that the operator be MATIS
  auto & petsc_options = _fe_problem.getPetscOptions();
  const std::string prefix_with_dash = '-' + _nl.prefix();

  if (_method == "bddc")
    petsc_options.pairs.emplace_back(prefix_with_dash + "pc_type", "bddc");
  else
    petsc_options.pairs.emplace_back(prefix_with_dash + "ksp_type", "fetidp");

  // Options coming from this object's parameters are stored last so that they override the above
  SingleMatrixPreconditioner::initialSetup();

  // KSPFETIDP gives its inner PCBDDC the 'fetidp_bddc_' options prefix, so the change of basis only
  // reaches that PCBDDC when it carries the prefix as well
  const std::string bddc_prefix =
      _method == "fetidp" ? prefix_with_dash + "fetidp_bddc_" : prefix_with_dash;

  // Without the change of basis a system of PDEs makes the local Neumann problems of floating
  // subdomains singular saddle point systems, which PETSc's unpivoted LU cannot factor
  Moose::PetscSupport::addDefaultPetscOption(
      petsc_options, bddc_prefix + "pc_bddc_use_change_of_basis", "true");

  // PETSc documents GMRES on the multiplier solve as the non-symmetric usage of KSPFETIDP, and the
  // operators assembled here are not guaranteed to be symmetric
  if (_method == "fetidp")
    Moose::PetscSupport::addDefaultPetscOption(
        petsc_options, prefix_with_dash + "fetidp_ksp_type", "gmres");

#ifdef LIBMESH_PETSC_HAVE_MUMPS
  for (const auto solver : {"dirichlet", "neumann"})
    Moose::PetscSupport::addDefaultPetscOption(
        petsc_options, bddc_prefix + "pc_bddc_" + solver + "_pc_factor_mat_solver_type", "mumps");
#endif

  if (!_saddle_point_vars.empty())
  {
    if (_method != "fetidp")
      paramError("saddle_point_variables",
                 "Saddle point support is provided by KSPFETIDP; set 'method = fetidp'.");

    for (const auto & var_name : _saddle_point_vars)
      if (!_nl.hasVariable(var_name))
        paramError("saddle_point_variables",
                   "The nonlinear system does not hold a variable named '",
                   var_name,
                   "'.");

    // Engage KSPFETIDP's saddle point path. While the multipliers are all inactive the operator
    // has no zero diagonals for it to detect, so the field preSolve() registers is what it falls
    // back to; once multipliers activate, the zero diagonal detection narrows the set to them
    const auto saddlepoint_flag = prefix_with_dash + "ksp_fetidp_saddlepoint";
    if (!petsc_options.flags.isValueSet(saddlepoint_flag) &&
        !petsc_options.dont_add_these_options.isValueSet(saddlepoint_flag))
      petsc_options.flags.setAdditionalValue(saddlepoint_flag);

    // The local subdomain problems and the coarse problem carry the saddle point structure, and
    // PETSc's own LU does not pivot, so it fails on them with a zero pivot; MUMPS pivots. The
    // coarse problem is solved through PCREDUNDANT, whose inner factorization reads the
    // 'coarse_redundant_' prefix
    for (const auto solver : {"dirichlet", "neumann", "coarse", "coarse_redundant"})
      Moose::PetscSupport::addDefaultPetscOption(
          petsc_options, bddc_prefix + "pc_bddc_" + solver + "_pc_factor_mat_solver_type", "mumps");
  }
}

void
DomainDecompositionPreconditioner::preSolve()
{
  registerSaddlePointDofs();
}

void
DomainDecompositionPreconditioner::resetSolver()
{
  auto * const petsc_solver =
      libMesh::cast_ptr<libMesh::PetscNonlinearSolver<libMesh::Number> *>(_nl.nonlinearSolver());
  SNES snes = petsc_solver->snes();

  // A fresh KSP rather than KSPReset: PCBDDC does not survive a reset on a MATIS whose local size
  // changed. The prefix and tolerances are what the nonlinear solver placed on the old KSP; the
  // options are read again below, which also restores KSPFETIDP's saddle point flag
  KSP old_ksp;
  LibmeshPetscCall(SNESGetKSP(snes, &old_ksp));
  const char * prefix;
  LibmeshPetscCall(KSPGetOptionsPrefix(old_ksp, &prefix));
  PetscReal rtol, abstol, dtol;
  PetscInt maxits;
  LibmeshPetscCall(KSPGetTolerances(old_ksp, &rtol, &abstol, &dtol, &maxits));
  KSPNormType norm_type;
  LibmeshPetscCall(KSPGetNormType(old_ksp, &norm_type));
  // MOOSE's linear convergence test carries the problem as its context and no destroy routine
  PetscErrorCode (*converged)(KSP, PetscInt, PetscReal, KSPConvergedReason *, void *);
  void * converged_context;
  LibmeshPetscCall(KSPGetConvergenceTest(old_ksp, &converged, &converged_context, nullptr));

  KSP ksp;
  LibmeshPetscCall(KSPCreate(comm().get(), &ksp));
  LibmeshPetscCall(KSPSetOptionsPrefix(ksp, prefix));
  LibmeshPetscCall(KSPSetTolerances(ksp, rtol, abstol, dtol, maxits));
  LibmeshPetscCall(KSPSetNormType(ksp, norm_type));
  LibmeshPetscCall(KSPSetConvergenceTest(ksp, converged, converged_context, nullptr));
  // The SNES takes its own reference and releases the old KSP
  LibmeshPetscCall(SNESSetKSP(snes, ksp));
  LibmeshPetscCall(KSPDestroy(&ksp));
  LibmeshPetscCall(SNESGetKSP(snes, &ksp));
  LibmeshPetscCall(KSPSetFromOptions(ksp));
  registerSaddlePointDofs();
}

void
DomainDecompositionPreconditioner::registerSaddlePointDofs()
{
  if (_saddle_point_vars.empty())
    return;

  // The dofs of the saddle point variables, in a global IS for the inner PCBDDC. Rebuilt on
  // every solve, because a system reinitialization renumbers dofs
  std::vector<PetscInt> saddle_dofs;
  for (const auto & var_name : _saddle_point_vars)
  {
    std::set<libMesh::dof_id_type> var_dofs;
    _nl.system().local_dof_indices(_nl.system().variable_number(var_name), var_dofs);
    for (const auto dof : var_dofs)
      saddle_dofs.push_back(libMesh::cast_int<PetscInt>(dof));
  }
  std::sort(saddle_dofs.begin(), saddle_dofs.end());

  // The inner PCBDDC only exists once the KSP is typed, so type it here; the later
  // KSPSetFromOptions re-applies '-ksp_type fetidp', which is a no-op for a matching type and
  // keeps the inner PCBDDC together with the field registered on it
  auto * const petsc_solver =
      libMesh::cast_ptr<libMesh::PetscNonlinearSolver<libMesh::Number> *>(_nl.nonlinearSolver());
  KSP ksp;
  LibmeshPetscCall(SNESGetKSP(petsc_solver->snes(), &ksp));
  LibmeshPetscCall(KSPSetType(ksp, KSPFETIDP));
  PC bddc_pc;
  LibmeshPetscCall(KSPFETIDPGetInnerBDDC(ksp, &bddc_pc));

  libMesh::WrappedPetsc<IS> saddle_is;
  LibmeshPetscCall(ISCreateGeneral(comm().get(),
                                   libMesh::cast_int<PetscInt>(saddle_dofs.size()),
                                   saddle_dofs.data(),
                                   PETSC_COPY_VALUES,
                                   saddle_is.get()));
  LibmeshPetscCall(PCBDDCSetDofsSplitting(bddc_pc, 1, saddle_is.get()));
}
