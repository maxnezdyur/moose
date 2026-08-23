# DDP

!syntax description /Preconditioning/DDP

## Overview

`DomainDecompositionPreconditioner` assembles the nonlinear system Jacobian in PETSc's globally
unassembled `MATIS` format instead of the usual assembled sparse matrix, then hands that matrix to
one of PETSc's non-overlapping domain decomposition solvers. The decomposition carries exactly one
subdomain per MPI process, and a subdomain holds the degrees of freedom its process reaches through
the mesh partition, so the partitioner selected in the [Mesh] block also fixes the decomposition the
solver sees.

[!param](/Preconditioning/DDP/method) selects the solver:

- `bddc` builds [PCBDDC](https://petsc.org/release/manualpages/PC/PCBDDC/) as the preconditioner of
  the outer Krylov solve.
- `fetidp` replaces the Krylov solver with
  [KSPFETIDP](https://petsc.org/release/manualpages/KSP/KSPFETIDP/), which solves the dual interface
  problem and preconditions it with an inner PCBDDC.

Both methods eliminate the subdomain interiors and couple the subdomains through a coarse problem
posed on the interface, and the two are spectrally equivalent: their preconditioned operators share
the same spectrum apart from the eigenvalues zero and one, so they converge at the same rate. For a
symmetric positive definite second order elliptic operator the condition number of the
preconditioned system satisfies

\begin{equation}
\kappa \leq C \left( 1 + \log \frac{H}{h} \right)^2 ,
\end{equation}

where $H$ is the subdomain diameter, $h$ is the element size, and $C$ depends on neither $H$, $h$,
nor the number of subdomains. The bound grows only logarithmically under mesh refinement and does
not grow with the process count.

## When To Use

Use a domain decomposition preconditioner for large parallel solves of elliptic problems, or
problems dominated by a symmetric positive definite operator, where one-level preconditioners lose
ground as the process count rises. Block Jacobi and additive Schwarz carry no coarse space, so their
iteration counts grow with the number of subdomains, while the bound above does not. PETSc documents
PCBDDC support for unsymmetric and indefinite problems, although the condition number bound above
holds for the symmetric positive definite case.

## Solver Options id=solver-options

PETSc options set in the [Executioner](syntax/Executioner/index.md) block, or in this block, pass
through unchanged, so PCBDDC and KSPFETIDP are tuned with their native options. With
[!param](/Preconditioning/DDP/method) set to `fetidp`, KSPFETIDP gives its inner PCBDDC the
`fetidp_bddc_` options prefix and its multiplier solve the `fetidp_` prefix, so an option reaches
either one only under that prefix.

### Defaults Applied By DDP

`DDP` applies the two options below itself. Each is a correctness requirement rather than a tuning
choice, so a solve that needs one and lacks it fails outright instead of converging slowly:

- `-pc_bddc_use_change_of_basis`, spelled `-fetidp_bddc_pc_bddc_use_change_of_basis` for
  `method = fetidp`. Without it the local Neumann problems of floating subdomains are singular
  saddle point systems, and PETSc's unpivoted LU fails on them.
- `-fetidp_ksp_type gmres`, for `method = fetidp` only. GMRES admits the unsymmetric operators MOOSE
  assembles, which is the usage PETSc documents for the non-symmetric case.

`DDP` stores each default after the options the two blocks above supply, and skips any option name
those blocks already carry. To change a default, name it in `petsc_options_iname` and give its value
in `petsc_options_value`; `-pc_bddc_use_change_of_basis false` turns the default off.

### Recommended Tuning

Two further options improve the coarse space of vector valued problems such as elasticity. `DDP`
leaves them to the input file, because they tune a solve that already runs:

- `-pc_bddc_monolithic` builds the coarse space across the coupled variable components together
  rather than one component at a time.
- `-pc_bddc_use_faces` adds face constraints to the coarse space.

With [!param](/Preconditioning/DDP/method) set to `fetidp`, these reach the inner PCBDDC as
`-fetidp_bddc_pc_bddc_monolithic` and `-fetidp_bddc_pc_bddc_use_faces`.

## Example Input File Syntax

!listing test/tests/preconditioners/ddp/ddp_diffusion.i block=Preconditioning

The solve type must assemble the Jacobian; this input uses `NEWTON`:

!listing test/tests/preconditioners/ddp/ddp_diffusion.i block=Executioner

## Limitations

- The solve type must be `NEWTON` or `LINEAR`. `JFNK` and `PJFNK` are rejected, because a matrix
  free operator supplies only the action of the Jacobian on a vector and never forms the assembled
  subdomain blocks that PCBDDC and KSPFETIDP factor.
- Problems that hold [Constraints] objects are rejected. The constraint Jacobian path reaches the
  system matrix through a downcast that the `MATIS` backed matrix does not satisfy.
- `use_hash_table_matrix_assembly` and `restore_original_nonzero_pattern` in the
  [Problem](syntax/Problem/index.md) block must both be `false`. PETSc implements neither the
  assembled copy nor `MatResetPreallocation` for `MATIS`.
- The decomposition places one subdomain on each process, so a serial run holds a single subdomain
  and degenerates into a near direct solve. Run on two or more processes.
- Systems of PDEs rest on `-pc_bddc_use_change_of_basis`, which `DDP` applies by default; see
  [#solver-options]. Turning it off leaves the local Neumann problems of floating subdomains as
  singular saddle point systems, and PETSc's unpivoted LU fails on them. With it, three dimensional
  elasticity converges with FETI-DP at four and eight subdomains; the primal `bddc` method remains
  sensitive to the material stiffness beyond two subdomains, so the shipped tests pin their `bddc`
  details to two processes and carry the large decompositions with `fetidp`.
- Unsymmetric operators, such as the exact finite strain Jacobian automatic differentiation
  assembles, are supported. The multiplier solve runs with GMRES and the change of basis stays on,
  both by default. PETSc states no symmetry requirement for KSPFETIDP, and the shipped finite strain
  test converges quadratically with the exact AD Jacobian at four and eight subdomains. Newton
  robustness is then governed by the physics setup, not the solver: presetting a displacement jump
  against a zero interior concentrates the whole increment in one element layer and defeats exact
  Newton on fine meshes under any linear solver; drive such loads with tractions, non-preset
  conditions with `line_search = none`, or smaller steps.
- Distributed meshes work: a mesh built distributed with
  [DistributedRectilinearMeshGenerator.md], whose grid partition MOOSE keeps, was verified correct
  at four and eight subdomains. One known gap: redistributing a replicated-generated mesh
  (`parallel_type = distributed` on a generated mesh) fails at four subdomains - PCBDDC
  disqualifies that partition's interface face candidates, the local problems stay singular, and no
  solver option recovers it, while the same mesh replicated is fine. Under investigation.
- Do not set `-pc_bddc_corner_selection`. It segfaults inside PETSc on a `MATIS` that carries no
  `PetscSection` (observed with PETSc 3.25.4).
- PETSc 3.20 or newer is required.

!syntax parameters /Preconditioning/DDP

!syntax inputs /Preconditioning/DDP

!syntax children /Preconditioning/DDP
