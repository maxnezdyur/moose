## Day 1: MOOSE Framework Fundamentals (morning lecture)

- Introduction to MOOSE and the anatomy of an input file
- Mesh generation and the Mesh system
- Variables and Kernels: building the governing equations
- Materials and material properties
- Boundary conditions and auxiliary variables
- Executioners, time stepping, Outputs, and Postprocessors

## Day 2: Solid Mechanics and Heat Conduction (morning lecture)

- Solid mechanics in MOOSE: weak form, small vs. finite strain formulations
- The heat conduction equation and thermal boundary conditions
- Coupled thermo-mechanics: thermal expansion and thermal stress analysis

## Day 3: Thermal Radiation & Solver Strategies (morning lecture)

> Reworked per Rich's feedback: dropped mechanical contact (mortar/Lagrange
> multipliers, multi-body) as too advanced and unnecessary for the fellows'
> summer projects. Reoriented around thermal radiation — accessible physics and
> a strong fit for space-reactor heat rejection / radiator design.

**Thermal radiation**
- Thermal radiation in MOOSE: black-body and gray-body radiation and the Stefan–Boltzmann law
- Radiative boundary conditions: surface-to-ambient radiation and radiative heat flux
- Application to space reactors: heat rejection and radiator thermal management

**Solver strategies (expanded focus)**
- Nonlinear solve: Newton vs. JFNK, and the mild T⁴ nonlinearity radiation introduces
- Linear solve: direct (LU) vs. iterative Krylov methods (GMRES/CG) — when each is appropriate
- Preconditioning for ok iterative solvers: why Krylov methods need it, and reading linear-iteration counts as a diagnostic
  - Common PETSc preconditioners: ILU, block Jacobi/ASM, and algebraic multigrid (HYPRE BoomerAMG, GAMG)
  - MOOSE `[Preconditioning]` block: SMP vs. FDP, and field-split (block) preconditioning for coupled thermo-mechanics
  - Choosing/setting options via `petsc_options_iname` / `petsc_options_value`
- Scaling (manual and automatic) and its effect on convergence
- Troubleshooting non-converging solves: distinguishing linear vs. nonlinear failure

**Inverse problems & optimization**
- Recovering unknown sources, properties, or loads from measurements with the Optimization module (adjoint-based gradients)

**Parameter studies**
- Parameter studies with the Stochastic Tools module: samplers, MultiApp batch execution, parameter/result transfers, and statistics reporters

## Afternoons (all three days)

Hands-on sessions: open time for student questions and guided work on the
students' own research problems, with instructor support (Max + Cade).

---

## Prerequisites

- Laptop with a working MOOSE installation
  (instructions: https://mooseframework.inl.gov/getting_started/installation/)
- Students encouraged to come with a research problem in mind for the afternoon sessions
