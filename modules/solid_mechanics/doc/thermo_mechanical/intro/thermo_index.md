# Overview

This training covers fundamentals of solid mechanics and heat conduction using MOOSE, including:

- Solid mechanics principles and formulations
- Heat conduction implementation
- Numerical solution strategies
- Contact mechanics and gap heat transfer

!---


# Course Outline

- Solid Mechanics Fundamentals
- Heat Conduction
- Numerical Solution Strategies
- Contact and Gap Heat Transfer
- Mortar Methods
- Solver Considerations for Contact
- Reference vs Current Configuration
- Practical Examples

!---


# Introduction to Solid Mechanics

- Fundamental concepts and principles
- Stress and strain tensors
- Conservation equations

!---


# Key Concepts

- *Deformation:* Departure from rest shape (displacement field $\boldsymbol{u}$)
- *Strain:* Measure of deformation relative to original size
- *Stress:* Internal forces that resist deformation
- Material behavior defined by constitutive relationships:
  $\boldsymbol{\sigma} = \tilde{\boldsymbol{\sigma}}(\boldsymbol{\epsilon} - \boldsymbol{\epsilon}_0)$
- For linear elasticity: $\boldsymbol{\sigma} = \boldsymbol{\mathcal{C}}(\boldsymbol{\epsilon} - \boldsymbol{\epsilon}_0)$

!---


The strong form of the governing equation for solid mechanics:

!equation
\nabla \cdot (\boldsymbol{\sigma} + \boldsymbol{\sigma}_0) + \boldsymbol{b} = \boldsymbol{0} \;\mathrm{in}\;\Omega \\
\boldsymbol{u} = \boldsymbol{g}\;\mathrm{in}\;\Gamma_{ \boldsymbol{g}}\\
\boldsymbol{\sigma} \cdot \boldsymbol{n}=\boldsymbol{t}\;\mathrm{in}\;\Gamma_{ \boldsymbol{t}}



Where:

- $\boldsymbol{\sigma}$ = Cauchy stress tensor
- $\boldsymbol{\sigma}_0$ = Additional source of stress (e.g., pore pressure)
- $\boldsymbol{b}$ = Body force
- $\boldsymbol{u}$ = Displacement vector
- $\boldsymbol{g}$ = Prescribed displacement boundary condition
- $\boldsymbol{t}$ = Prescribed traction boundary condition

!---


# Weak Form Formulation

- The weak form of the residual equation:

  !equation
  \mathbb{R} = \left( \boldsymbol{\sigma} + \boldsymbol{\sigma}_0, \nabla \phi_m \right) - \left< \boldsymbol{t}, \phi_m \right> - \left( \boldsymbol{b}, \phi_m \right) = \boldsymbol{0}

- The Jacobian for Newton's method (ignoring boundary terms):

  !equation
  \mathbb{J} = \left( \frac{\partial \boldsymbol{\sigma}}{\partial \nabla \boldsymbol{u}} , \nabla \phi_m \right)

- Where:

  - $(\cdot)$ represents volume integrals
  - $\left< \cdot \right>$ represents boundary integrals
  - $\phi_m$ are the test functions

!---

# Small vs Large Deformation Intro

!row!

!col! width=50%

+Linearized Elasticity+

- Calculated on reference mesh
- Governing equation:
  $\nabla_X \cdot \sigma(X) = 0$
- Valid when displacement gradients are small
- Used with ComputeSmallStrain

!col-end!

!col! width=50%

+Large Deformation+

- Calculated on deformed mesh
- Governing equation:
  $\nabla_x \cdot \sigma(x) = 0$
- Accounts for geometric nonlinearity
- Used with ComputeFiniteStrain

!col-end!

!row-end!

!---


# Small Strain Theory

- *Definition:* Linearized small strain assumes displacement gradients are much smaller than unity, so higher-order terms are negligible.
- *Total Strain Formula:*
  $\boldsymbol{\epsilon} = \frac{1}{2} \left( \nabla \mathbf{u} + (\nabla \mathbf{u})^T \right) \quad \text{when} \quad \left\lvert \frac{\partial \mathbf{u}}{\partial \mathbf{x}} \right\rvert \ll 1$
- *Path Independence:*

  - The total strain is used from the start of the simulation.
  - Stress and strain from previous steps are not stored (no stateful update).
- *Typical Use Case:*

  - Verifying linear elasticity problems with hand calculations.
  - Best suited for problems where deformations remain small.

!---


# Large Deformation Mechanics

- Concerns problems where the displacement gradient is not necessarily small.
- Requires a more general definition of strain (finite strain).
- Linear assumptions (e.g., superposition) no longer strictly valid.
- Geometric nonlinearities must be taken into account.

!---


# Finite Strain Theory

- Uses a deformation gradient $\mathbf{F}$ to describe changes in shape and volume.
- Must account for large rotations and strains:
  $\mathbf{F} = \frac{\partial \mathbf{x}}{\partial \mathbf{X}},$
  where $\mathbf{X}$ is the reference coordinate, and $\mathbf{x}$ is the current (deformed) coordinate.
- Strain measures (e.g., Green-Lagrange strain) incorporate higher-order terms of displacement.
- Often solved in an incremental or updated Lagrangian framework.

!---


# Incremental Deformation Gradient

- Large deformation problems often use an *incremental* (updated Lagrangian) approach.
- Define an incremental deformation gradient $\hat{\mathbf{F}}$ over each time step from $t_n$ to $t_{n+1}$:
  $\hat{\mathbf{F}} = \mathbf{F}_{n+1}\,\mathbf{F}_n^{-1},$
  where $\mathbf{F}_n$ is the total deformation gradient at $t_n$.
- This incremental form captures the *new* deformation each step without re-initializing from the original reference.
- Often expressed via:
  $\hat{\mathbf{F}}^{-1} = \mathbf{I} - \nabla \hat{\mathbf{u}},$
  where $\hat{\mathbf{u}}$ is the incremental displacement between the two configurations.
- Each time step updates strain and rotation incrementally, which is then added to the previous total state.

!---


# Closed Loop Large Deformation Loading Cycle


!media solid_mechanics/closed_loop_large_deform_cycle_loading.png
       id=closed_loop_cycle_loading
       style=width:95%;float:right;padding-top:1.5%;
       caption=Closed loop large deformation loading cycle.

- Initial configuration (A) with dimensions $L \times L$.
- Intermediate stages show stretching ($\Delta y$) and shearing ($\Delta x$).
- Final shape (E) might return to a form similar to the initial state, but with residual effects if large deformations are not perfectly elastic.

!---


# Volumetric Locking Correction

- *Definition:*

  - Numerical artifact in nearly incompressible materials where elements become overly stiff.
  - Leads to inaccurate simulation of large deformations.

- *F-bar Method in MOOSE:*

  - In `ComputeFiniteStrain`, the incremental deformation gradient $\hat{\mathbf{F}}$ can be corrected to $\hat{\mathbf{F}}_{\mathrm{corr}}$ via:
    $\hat{\mathbf{F}}_{\mathrm{corr}} = \hat{\mathbf{F}} \left( \frac{\bigl|\mathrm{av}_{el}(\hat{\mathbf{F}})\bigr|}{\bigl|\hat{\mathbf{F}}\bigr|} \right)^{\tfrac{1}{3}},$
    where $\mathrm{av}_{el}$ denotes the element-averaged deformation gradient.
  - This correction reduces the artificially stiff volumetric response (locking).

- *Incremental Update:*

  - After computing $\hat{\mathbf{F}}_{\mathrm{corr}}$, the strain increment is added to the total strain from $t_n$, then rotated by the rotation increment for $t_{n+1}$.

!---


!row!

!col! width=50%

+Geometry Setup+



!media media/solid_mechanics/cook_problem.png
      style=width:80%;float:left;
      id=fig_cook
      caption=Volumetric Locking Setup

- Trapezoidal membrane with fixed left edge
- Applied vertical load at right edge
- Near-incompressible material ($\nu \approx 0.4999$)

!col-end!

!col! width=50%

+Vertical Displacement at A+

!media media/solid_mechanics/cook_results.png
      style=width:100%;float:right;
      id=fig_cook_results
      caption= Locking Behavior


*Legend:*

- `first/second` = element order
- `vol/no_vol` = with/without B-bar

!col-end!

!row-end!

!---


!row!

!col! width=50%

+Classical Plane Strain+

- Out-of-plane strain constrained to zero: $\epsilon_{zz} = 0$
- No displacement in out-of-plane direction
- *When to use:*

  - Thick bodies with constrained ends
  - Dams, tunnels, buried structures
  - Long prismatic components

!col-end!

!col! width=50%

+Plane Stress+

- Out-of-plane stress is zero: $\sigma_{zz} = 0$
- Free surfaces normal to out-of-plane direction
- *When to use:*

  - Thin plates and shells
  - Sheet metal forming
  - Membrane structures

!col-end!

!row-end!

+Generalized Plane Strain+

- Extension of plane strain with non-zero but uniform out-of-plane strain: $\epsilon_{zz} = \text{constant} \neq 0$ throughout domain
- *When to use:*

  - Cross-sections of beams with thermal expansion
  - Periodic sections in fiber-reinforced composites
  - Slices of structures with constrained ends allowing uniform expansion

!---


!row!

!col! width=40%

+Finite Strain (Large Deformation)+



```
# Finite strain
#(large deformation)
[Physics]
  [SolidMechanics]
    [QuasiStatic]
      [all]
        strain = FINITE
        incremental = true
        add_variables = true
      []
    []
  []
[]
```



!col-end!

!col! width=1%
\\
!col-end!

!col! width=40%

+Small Strain (Linearized)+

```
# Small strain (linearized)
[Physics]
  [SolidMechanics]
    [QuasiStatic]
      [all]
        strain = SMALL
        add_variables = true
      []
    []
  []
[]
```

!col-end!

!row-end!


!---


!row!

!col! width=50%

+Material System Components+

*Strain Calculator*

- ComputeSmallStrain
- ComputeFiniteStrain
- ComputePlaneSmallStrain

*Elasticity Tensor*

- ComputeIsotropicElasticityTensor
- ComputeElasticityTensor
- ComputeAnisotropicElasticityTensor

*Stress Calculator*

- ComputeLinearElasticStress
- ComputeFiniteStrainElasticStress
- ComputeMultipleInelasticStress

*Automatic Differentiation (AD):*

- Add "AD" prefix: ADComputeSmallStrain

!col-end!


!col! width=40%

+Example: Isotropic Elastic Material+

!style! fontsize=90%

```
[Materials]
  [elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 210e9
    poissons_ratio = 0.3
  []

  [strain]
    type = ComputeSmallStrain
    displacements = 'disp_x disp_y disp_z'
  []

  [stress]
    type = ComputeLinearElasticStress
  []
[]
```

!style-end!

!col-end!

!row-end!

!---


# Introduction to Heat Transfer

- Heat is transferred by three mechanisms:

  - *Conduction*: Diffusion of heat through a solid or stationary fluid
  - *Convection*: Heat transfer between a surface and moving fluid
  - *Radiation*: Energy transfer via electromagnetic waves
- The Heat Transfer module focuses on:

  - Heat conduction equation implementation
  - Associated boundary conditions
  - Radiation between gray, diffuse surfaces
  - Temperature coupling with fluid domains
  - Gap heat transfer models

!---


# Heat Conduction Equation

- The heat conduction equation describes diffusion of heat:

  $\rho c\frac{\partial T}{\partial t} = \nabla \cdot [k \nabla T] + \dot{q}$
- Where:

  - $T$ is temperature
  - $t$ is time
  - $\vec{x}$ is the vector of spatial coordinates
  - $\rho$ is density
  - $c$ is specific heat capacity
  - $k$ is thermal conductivity
  - $\dot{q}$ is volumetric heat source

!---


# Boundary Conditions

The boundary is divided into Dirichlet boundaries $\partial \Omega_D$ and Robin boundaries $\partial \Omega_R$:

!equation
\begin{aligned}
  T &= T_D~\text{for}~\vec{x}\in \partial \Omega_D \\
  -k \vec{n} \cdot \nabla T - G &= 0 ~\text{for}~\vec{x}\in \partial \Omega_R
\end{aligned}


Common types of Robin boundary conditions:

!equation
\begin{aligned}
  &\text{Neumann:} & G &= 0 \\
  &\text{Convection:} & G &= h (T - T_{\infty}) \\
  &\text{Radiation:} & G &= \sigma S (T^4 - T_{\infty}^4)
\end{aligned}


Where:

- $h$ is the convective heat transfer coefficient
- $\sigma$ is the Stefan-Boltzmann constant
- $S$ depends on geometry and surface properties

!---


# Newton's Method for Nonlinear Systems

*Newton's Method in Update Form:*

!equation
\begin{split}
  \mathbf{J}(\vec{u}_n) \delta\vec{u}_{n+1} &= -\vec{R}(\vec{u}_n) \\
  \vec{u}_{n+1} &= \vec{u}_n + \delta\vec{u}_{n+1}
\end{split}


*Jacobian Matrix:*

!equation
J_{ij}(\vec{u}_n) = \dfrac{\partial R_i(\vec{u}_n)}{\partial u_j}


*Key Properties:*

- Quadratic convergence when near solution
- Robust for well-posed problems
- Efficient with good initial guess
- May diverge if initial guess poor

!---


# Newton Implementation Options

*Hand-coded Jacobians:*

- Explicit analytic derivatives
- Most efficient but tedious to implement
- Error-prone for complex problems

*Automatic Differentiation (AD):*

- Computes derivatives automatically
- Uses dual number approach
- Trade computational cost for development time
- Uses `ADKernel` instead of `Kernel`

*Jacobian-Free Newton-Krylov (JFNK):*

- Approximates Jacobian action without forming matrix
- Requires only residual evaluations
- Good for complex multiphysics problems

!---


# Jacobian-Free Newton-Krylov (JFNK)

*Key Concept:* Never explicitly forms the Jacobian matrix

*Approximates Jacobian action on vector:*

!equation
\mathbf{J}\vec{v} \approx \dfrac{\vec{R}(\vec{u} + \epsilon\vec{v}) - \vec{R}(\vec{u})}{\epsilon}


*PJFNK (Preconditioned JFNK):*

- Default in MOOSE
- Uses simplified Jacobian for preconditioning
- Improves convergence while maintaining matrix-free advantages

!---


# When to Use Each Newton Approach

*When to Use PJFNK:*

- Difficult/expensive Jacobian derivation
- Complex multiphysics problems
- Memory constraints (no matrix storage)
- Rapid prototyping (no Jacobian needed)

*When to Use Full Newton with AD:*

- When accuracy of Jacobian is critical
- Complex physics where hand-coding is error-prone
- When development time must be minimized
- Modern approach for most new MOOSE development

*When to Use Hand-Coded Jacobians:*

- Simple physics with known analytical Jacobians
- Performance-critical applications

!---


# Preconditioning: Fundamentals

*Purpose:* Transform linear system to improve condition number

*Original System:* $\mathbf{A}\vec{x} = \vec{b}$

*Preconditioned System:* $\mathbf{M}^{-1}\mathbf{A}\vec{x} = \mathbf{M}^{-1}\vec{b}$

*Ideal Preconditioner:*

- $\mathbf{M}^{-1} \approx \mathbf{A}^{-1}$ (approximates inverse)
- Inexpensive to apply
- Improves clustering of eigenvalues
- Reduces condition number

*Effect on Convergence:*

- Good preconditioning: Few iterations needed
- Poor preconditioning: Many iterations or no convergence
- Critical for JFNK and iterative solvers

!---


# Preconditioner Types in MOOSE

*SMP (Single Matrix Preconditioner)*

- Default in MOOSE
- Full Jacobian or block-diagonal approximation
- Efficient for single-physics problems

*FDP (Finite Difference Preconditioner)*

- Approximates Jacobian via finite differences
- No hand-coding of Jacobians needed
- Less efficient than SMP with good Jacobians

*FSP (Field Split Preconditioner)*

- Physics-based splitting of variables
- Handles multi-physics coupling
- Allows different solver for each physics
- Powerful for strongly coupled problems

!---


# Preconditioning Matrix Example

*Fully Coupled System:*

!equation
\begin{aligned}
\nabla \cdot k(s,T) \nabla T &= 0 \\
\nabla \cdot D(s,T) \nabla s &= 0
\end{aligned}


*Full Jacobian:*

!equation
\boldsymbol{R}'(s,T) =
 \begin{bmatrix}
 (\boldsymbol{R}_T)_T & (\boldsymbol{R}_T)_s
\\
 (\boldsymbol{R}_s)_T & (\boldsymbol{R}_s)_s
 \end{bmatrix}


*Block-Diagonal Approximation:*

!equation
\boldsymbol{M} \equiv
 \begin{bmatrix}
 (k(s,T) \nabla \phi_j, \nabla \psi_i) & \boldsymbol{0} \\
 \boldsymbol{0} & (D(s,T) \nabla \phi_j, \nabla\psi_i)
 \end{bmatrix}


!---


# Preconditioner Selection Strategy

*Single-physics Problems:*

- SMP with direct solver (LU, MUMPS, etc.)
- Full Jacobian preconditioner
- Usually most robust option

*Loosely Coupled Multi-physics:*

- Block-diagonal SMP (ignores cross-coupling)
- FSP with block-Jacobi or block-Gauss-Seidel
- Good balance of efficiency and robustness

*Strongly Coupled Multi-physics:*

- FSP with physics-based splits
- Nested preconditioners for each field
- May require custom configuration

!---


# Direct Linear Solvers

*LU Decomposition:* $\mathbf{A} = \mathbf{L}\mathbf{U}$

*Implementation Options:*

- SuperLU_DIST: Distributed memory parallel
- MUMPS: Robust multifrontal solver

*Advantages:*

- Very robust solution method
- Single-step solution (non-iterative)
- Excellent as preconditioner
- Handles ill-conditioned systems

*Limitations:*

- Memory intensive (scales poorly)
- Limited parallel scalability
- Impractical for very large systems

!---


# Iterative Linear Solvers

*GMRES (Generalized Minimal RESidual):*

- Default in MOOSE/PETSc
- Works for general non-symmetric systems
- Memory increases with iterations
- Restart parameter balances memory vs. convergence

*BiCGStab (Biconjugate Gradient Stabilized):*

- Non-symmetric systems
- Sometimes more stable than GMRES
- Constant memory requirements

*Key Advantages:*

- Better scaling with problem size
- Lower memory requirements
- Better parallel performance

!---


# Linear Solver Configuration

*PETSc Configuration:*

```
[Executioner]
# Direct solver
 petsc_options_iname='-pc_type -pc_factor_mat_solver_package'
 petsc_options_value='lu mumps'

# Iterative solver
 petsc_options_iname = '-pc_type -sub_pc_type -ksp_type'
 petsc_options_value = 'asm lu gmres'
[]
```

*Linear vs. Nonlinear Residuals:*

- Linear: $\|\mathbf{A}\vec{x}_k - \vec{b}\|$ (inner iteration)
- Nonlinear: $\|\vec{R}(\vec{u}_n)\|$ (outer iteration)
- Monitor with `print_linear_residuals = true`

!---


# Automatic Scaling: Problem and Approach

*Condition Number Problem:*

- Different physics can have vastly different scales
- Poor condition number in Jacobian
- Slow/stalled convergence
- Dominance by variables with largest magnitudes

*Basic Concept:*

- Apply scaling factors to each variable
- Normalize Jacobian entries to similar magnitude
- Improve condition number
- Enable efficient convergence

```
[Executioner]
  automatic_scaling = true
[]
```

!---


# Reference Residual: Fundamentals

*Standard Convergence Issues:*

- Variable scaling differences skew convergence check
- Low initial residual causes over-tightening
- Zero residuals at Dirichlet boundaries

*Default Convergence Check:*
$\frac{\|\vec{R}(\vec{u}_n)\|}{\|\vec{R}(\vec{u}_0)\|} < \text{tolerance}$

*Reference Residual Approach:*

- Checks each variable individually
- Uses physically meaningful reference quantity
- Converged when:
  $\|\vec{R}_i(\vec{u}_n)\| < \text{tolerance} \times \|\vec{R}_{ref,i}\|$
- Reference typically from reaction forces or fluxes

!---


# Reference Residual: Implementation

*Key Components:*

- `ReferenceResidualProblem` object
- Reference vector from physically relevant quantities
- Tagging key objects with `extra_vector_tags`

*Example Input:*

```
[Problem]
  type = ReferenceResidualProblem
  extra_tag_vectors = 'ref'
  reference_vector = 'ref'
  group_variables = 'disp_x disp_y disp_z'
[]
[BCs]
  [fixed_x]
    type = DirichletBC
    variable = disp_x
    boundary = 'left'
    value = 0.0
    extra_vector_tags = 'ref'
  []
[]
```

!---

# Time Integration

!row!

!col! width=50%

+First-Order: Implicit Euler+

!equation
\frac{u^{n+1} - u^n}{\Delta t} = \dot u(u^{n+1}, t^{n+1})

*Properties:*

- First-order accurate in time
- L-stable (strong damping)
- Unconditionally stable
- More numerical dissipation

!col-end!

!col! width=50%

+Second-Order: BDF2+

!equation
\frac{3u^{n+1} - 4u^n + u^{n-1}}{2\Delta t} = \dot u(u^{n+1}, t^{n+1})

*Properties:*

- Second-order accurate in time
- A-stable (no error growth)
- Requires two previous steps
- Better accuracy for same $\Delta t$

!col-end!

!row-end!

*MOOSE Configuration:*

```
[Executioner]
  type = Transient
  scheme = 'bdf2'  # or 'implicit-euler'
[]
```

!---


# Contact Mechanics: Fundamentals

*Contact Constraints:*

!equation
\begin{aligned}
g &\leq 0 \quad \text{(non-penetration)} \\
t_N &\geq 0 \quad \text{(compressive normal force)} \\
t_N g &= 0 \quad \text{(complementarity condition)}
\end{aligned}


*Key Concepts:*

- Gap ($g$): Penetration distance between contacting bodies
- Contact force ($t_N$): Force opposing penetration
- Either penetration is zero or contact force is zero

*Constraint Enforcement Methods:*

- *Penalty Method*: Applies force proportional to penetration
- *Lagrange Multiplier*: Adds variables to enforce constraints exactly
- *Augmented Lagrangian*: Hybrid approach combining both methods

!---


# Node-Face Contact Approach

![Node-face contact diagram](graphics/node_face_contact.png)

!equation
\lambda_n = \kappa\,g_n^+,
\quad
g_n =
\bigl(\mathbf{u}_{\mathrm{secondary}} - \mathbf{u}_{\mathrm{primary}}\bigr)\cdot \mathbf{n},
\quad
g_n^+ = \max\{\,g_n,\,0\}.


- $\lambda_n$ is the normal contact traction (force per unit area).
- $\kappa$ is the *penalty parameter* (often treated as a stiff spring constant).
- $g_n$ is the *normal gap* between primary and secondary surfaces.

!---


# Mortar-Based Contact Methods

![Mortar contact visualization](graphics/mortar_contact.png)

!row!

!col! width=48%

+Mortar Concept+

- Uses separate space of Lagrange multipliers
- Weak enforcement of constraints
- Integrated over contact interface
- Avoids over-constraint problems

!col-end!

!col! width=48%

+Implementation+

- Defines mortar space on a lower-dimensional interface
- Transfers variables between non-matching meshes
- Enforces constraints in a weighted-integral sense
- Supports primal (displacement) and dual (contact pressure) variables

!col-end!

!row-end!

!---


# Gap Heat Transfer: Physics

!equation
h_{\text{gap}} = h_{\text{contact}} + h_{\text{gas}} + h_{\text{radiation}}


*Heat Transfer Mechanisms:*

- *Contact conductance*

  - Direct solid-solid conduction
  - Depends on: pressure, roughness, hardness
- *Gas conductance*

  - Through gap medium
  - Depends on: gap width, gas properties
- *Radiation*

  - Important at high temperatures
  - Depends on: surface emissivities

!---


# Gap Heat Transfer: MOOSE Implementation

*Implementation Options:*

- *GapHeatTransfer*

  - Node-to-face approach
  - Works with mechanical contact


- *ModularGapConductanceConstraint*

  - Mortar-based approach
  - Better for non-matching meshes

*Gap Conductivity Model:*

!equation
k_{\text{eff}} = \frac{k_1 k_2}{(k_1 + k_2)} \cdot f(p, \sigma, h, T)


!---


# MOOSE Implementation: Contact Block

*Contact Block Syntax:*

```
[Contact]
  [block_to_block]
    primary = 1
    secondary = 2
    model = frictionless
    formulation = kinematic
    normal_smoothing_distance = 0.1
  []
[]
```

*Available Contact Models:*

- `model = frictionless`: No tangential forces
- `model = coulomb`: Friction with Coulomb's law
- `model = glued`: No relative motion

*Contact Formulations:*

- `formulation = kinematic`: Exact enforcement (node/face or mortar)
- `formulation = penalty`: Approximate enforcement with penalty parameter
- `formulation = augmented_lagrange`: Hybrid approach
- `formulation = mortar`: Mortar-based discretization

!---


# Gap Heat Transfer Implementation

*Traditional Node-Face Approach:*

```
[ThermalContact]
  [thermal_contact]
    type = GapHeatTransfer
    variable = temp
    primary = 1
    secondary = 2
    gap_conductivity = 1.0
    gap_conductivity_function = gap_cond
  []
[]
```

*Mortar-Based Approach:*

```
[Constraints]
  [mortar_thermal]
    type = ModularGapConductanceConstraint
    variable = lambda
    primary_variable = temp_primary
    secondary_variable = temp_secondary
    primary_boundary = 10
    secondary_boundary = 20
    gap_conductance = 1000.0
  []
[]
```

*Gap Conductance Models:*

- Constant: Fixed conductance value
- Pressure-dependent: Function of contact pressure
- Temperature-dependent: Function of surface temperatures
- Gap-width-dependent: Varies with physical separation

!---


# Introduction to Mortar Methods

- Mathematical foundation
- Domain decomposition
- Non-matching meshes

!---


# Implementation in MOOSE (Mortar Methods)

- Mortar constraints
- Input file syntax
- Practical examples

!---


# Edge Dropping

- Problem statement
- Mathematical treatment
- Implementation in MOOSE
- Effect on solution accuracy

!---


# Solver Considerations for Node/Face Contact

- Key aspects of numerical solution
- Stability and convergence
- Implementation details in MOOSE

!---


# Introduction to Saddle Point Problems

- Mathematical formulation
- Stability conditions

!---


# Lagrange Multiplier Method

- Formulation
- Implementation in MOOSE
- Connection to contact and mortar methods

!---


# Continuum Mechanics Review

- Lagrangian vs Eulerian descriptions
- Deformation gradient

!---


# MOOSE Implementation (Reference & Current Config)

- Reference configuration for:
  - Time integration
  - Volumetric source terms
- Current configuration applications
- Code examples demonstrating proper usage

!---


# Combined Thermomechanical Problems

- Coupled physics
- Implementation strategies

!---


# Real-World Applications

- Industrial examples
- Performance optimization

!---


# Questions?


