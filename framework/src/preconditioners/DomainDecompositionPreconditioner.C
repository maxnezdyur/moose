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

  return params;
}

DomainDecompositionPreconditioner::DomainDecompositionPreconditioner(const InputParameters & params)
  : SingleMatrixPreconditioner(params), _method(getParam<MooseEnum>("method"))
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
  sys.add_matrix("System Matrix", std::make_unique<PetscMatrixIS>(sys.comm()), libMesh::PARALLEL);
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
}
