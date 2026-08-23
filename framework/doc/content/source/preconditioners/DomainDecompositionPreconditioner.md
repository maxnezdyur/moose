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

- `bddc` builds [PCBDDC](https://petsc.org/release/manualpages/PC/PCBDDC/), the balancing domain
  decomposition by constraints method of [!citep](dohrmann2003preconditioner), as the
  preconditioner of the outer Krylov solve.
- `fetidp` replaces the Krylov solver with
  [KSPFETIDP](https://petsc.org/release/manualpages/KSP/KSPFETIDP/), the dual-primal finite element
  tearing and interconnecting method of [!citep](farhat2001fetidp), which solves the dual interface
  problem and preconditions it with an inner PCBDDC.

Both methods eliminate the subdomain interiors and couple the subdomains through a coarse problem
posed on the interface, and the two are spectrally equivalent [!citep](mandel2005algebraic): their
preconditioned operators share the same spectrum apart from the eigenvalues zero and one, so they
converge at the same rate. For a symmetric positive definite second order elliptic operator the
condition number of the preconditioned system satisfies [!citep](mandel2003convergence)

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

`DDP` stores each default after the options supplied by the `[Executioner]` and `[Preconditioning]`
blocks, and skips any option name those blocks already carry. To change a default, name it in
`petsc_options_iname` and give its value in `petsc_options_value`;
`-pc_bddc_use_change_of_basis false` turns the default off. Use the spelling given above for the
selected method: with `method = fetidp` only `-fetidp_bddc_pc_bddc_use_change_of_basis` matches the
stored default, and the unprefixed name does not.

### Recommended Tuning

Two further options improve the coarse space of vector valued problems such as elasticity. `DDP`
leaves them to the input file, because they tune a solve that already runs:

- `-pc_bddc_monolithic` builds the coarse space across the coupled variable components together
  rather than one component at a time.
- `-pc_bddc_use_faces` adds face constraints to the coarse space.

With [!param](/Preconditioning/DDP/method) set to `fetidp`, these reach the inner PCBDDC as
`-fetidp_bddc_pc_bddc_monolithic` and `-fetidp_bddc_pc_bddc_use_faces`.

For elasticity, pair them with the rigid body mode near-null-space
([RigidBodyModes.md], enabled through `rigid_body_near_null_space = true` on the solid mechanics
QuasiStatic physics), which PCBDDC consumes to enrich its coarse space
(`-pc_bddc_use_nnsp` defaults to true). The shipped finite strain `bddc` test converges at four
subdomains only with all three ingredients; the change of basis default alone is not sufficient
there.

## Example Input File Syntax

!listing test/tests/preconditioners/ddp/ddp_diffusion.i block=Preconditioning

The solve type must assemble the Jacobian; this input uses `NEWTON`:

!listing test/tests/preconditioners/ddp/ddp_diffusion.i block=Executioner

## Limitations

- The solve type must be `NEWTON` or `LINEAR`. `JFNK` and `PJFNK` are rejected, because a matrix
  free operator supplies only the action of the Jacobian on a vector and never forms the assembled
  subdomain blocks that PCBDDC and KSPFETIDP factor.
- [Constraints] objects, penalty and kinematic mechanical contact included, are supported when
  every degree of freedom carries a nonzero assembled diagonal. The constraint couplings must stay
  inside each rank's subdomain (the owned dofs plus the send list, which the constraint ghosting
  populates); the `MATIS` backed matrix reports an error naming the dof on any insertion that
  leaves the subdomain, instead of dropping the entry silently, and splits the assembled diagonal
  across the subdomains sharing a dof whose local diagonal would otherwise be zero. Lagrange
  multiplier enforcement is not supported: an active multiplier row has an exactly zero assembled
  diagonal, which leaves the local subdomain problems singular, and the solve fails at
  preconditioner setup.
- `use_hash_table_matrix_assembly` and `restore_original_nonzero_pattern` in the
  [Problem](syntax/Problem/index.md) block must both be `false`. PETSc implements neither the
  assembled copy nor `MatResetPreallocation` for `MATIS`.
- The decomposition places one subdomain on each process, so a serial run holds a single subdomain
  and degenerates into a near direct solve. Run on two or more processes.
- Systems of PDEs rest on `-pc_bddc_use_change_of_basis`, which `DDP` applies by default; see
  [#solver-options]. Turning it off leaves the local Neumann problems of floating subdomains as
  singular saddle point systems, and PETSc's unpivoted LU fails on them. With it, three dimensional
  elasticity converges with FETI-DP at four and eight subdomains; the primal `bddc` method remains
  sensitive to the material stiffness as the subdomain count grows, so the shipped `bddc` tests cap
  the process count -- two for linear elasticity and four for finite strain -- while `fetidp`
  carries the larger decompositions.
- Unsymmetric operators, such as the exact finite strain Jacobian that automatic differentiation
  assembles, are supported. The multiplier solve runs with GMRES and the change of basis stays on,
  both by default. PETSc states no symmetry requirement for KSPFETIDP, and the shipped finite strain
  test converges quadratically with the exact AD Jacobian at four and eight subdomains. Newton
  robustness is then governed by the physics setup, not the solver: presetting a displacement jump
  against a zero interior concentrates the whole increment in one element layer and defeats exact
  Newton on fine meshes under any linear solver. Drive such loads with tractions, use non-preset
  boundary conditions with `line_search = none`, or take smaller time steps.
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
