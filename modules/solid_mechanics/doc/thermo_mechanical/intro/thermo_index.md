# Heat Conduction & Solid Mechanics for Thermomechanical Analysis

## MOOSE Training

!---

# Overview

This training covers fundamentals of solid mechanics and heat conduction using MOOSE, including:

- Solid mechanics principles and formulations
- Heat conduction implementation
- Numerical solution strategies
- Contact mechanics and gap heat transfer

!---

# Course Outline

+Day 1 — MOOSE Framework Fundamentals+

- Anatomy of an input file; Mesh, Variables, Kernels
- Materials, boundary conditions, AuxVariables
- Executioners, time stepping, Outputs, Postprocessors

+Day 2 — Solid Mechanics & Heat Conduction+

- Solid mechanics: weak form, small vs. finite strain; modern Lagrangian kernel system
- Heat conduction and thermal boundary conditions
- Coupled thermo-mechanics: thermal expansion & thermal stress

+Day 3 — Thermal Radiation & Solver Strategies+

- Black/gray-body radiation; surface-to-ambient & enclosure radiation
- Nonlinear & linear solvers, preconditioning, scaling, troubleshooting
- Parameter studies with stochastic tools

!---

# Day 1

## MOOSE Framework Fundamentals

!---

# Moose Introduction


A basic MOOSE input file can contain these eight parts, which will be covered in greater detail later.

- `[Mesh]`: Define the geometry of the domain
- `[Variables]`: Define the unknown(s) of the problem
- `[Kernels]`: Define the equation(s) to solve
- `[Materials]`: Define the physical properties of materials in the domain
- `[BCs]`: Define the boundary condition(s) of the problem
- `[AuxVariables]`:  Define secondary variables for postprocessing and visualization
- `[Executioner]`: Define how the problem will be solved
- `[Outputs]`: Define how the solution will be returned

!---

# [Mesh System](syntax/Mesh/index.md)

A system for defining a finite element / volume mesh.

!---

## Creating a Mesh

For complicated geometries, we often use CUBIT from Sandia National Laboratories
[cubit.sandia.gov](https://cubit.sandia.gov).

Other mesh generators can work as long as they output a file format that libMesh reads.

!---

## Mesh generators

Meshes in MOOSE are built or loaded using [MeshGenerators](syntax/Mesh/index.md).

To only generate the mesh without running the simulation, you can pass `--mesh-only` on the command line.

!---

## FileMeshGenerator

`FileMeshGenerator` is the MeshGenerator to load external meshes:

!listing test/tests/meshgenerators/file_mesh_generator/file_mesh_generator.i block=Mesh

MOOSE supports reading and writing a large number of formats and could be extended to read more.

!---

| Extension   | Description                              |
| :-          | :-                                       |
| .dat        | Tecplot ASCII file                       |
| .e, .exd    | Sandia's ExodusII format                 |
| .fro        | ACDL's surface triangulation file        |
| .gmv        | LANL's GMV (General Mesh Viewer) format  |
| .mat        | Matlab triangular ASCII file (read only) |
| .msh        | GMSH ASCII file                          |
| .n, .nem    | Sandia's Nemesis format                  |
| .plt        | Tecplot binary file (write only)         |
| .node, .ele; .poly | TetGen ASCII file (read; write)   |
| .inp        | Abaqus .inp format (read only)           |
| .ucd        | AVS's ASCII UCD format                   |
| .unv        | I-deas Universal format                  |
| .xda, .xdr  | libMesh formats                          |
| .vtk, .pvtu | Visualization Toolkit                    |

!---

## Generating Meshes in MOOSE

!row!

!col! width=40%

Built-in mesh generation is implemented for lines, rectangles, or rectangular prisms.

!style! fontsize=50%

!listing face_info_tri.i block=Mesh

!style-end!

!col-end!

!col! width=10%
\\
!col-end!

!col! width=60%


The sides are named in a logical way and are numbered:

- 1D: left = 0, right = 1
- 2D: bottom = 0, right = 1, top = 2, left = 3
- 3D: back = 0, bottom = 1, right = 2, top = 3, left = 4, front = 5

The capability is very convenient for parametric mesh optimization!


!col-end!

!row-end!

!---

## Named Entity Support

Human-readable names can be assigned to blocks, sidesets, and nodesets that can be used throughout
an input file.

A parameter that requires an ID will accept either numbers or "names".

Names can be assigned to IDs for existing meshes to ease input file maintenance.

+Note+

- Nodesets and sidesets should have different ids.
- At construction all sets with the same id are merged.

!---

## Replicated Mesh

When running in parallel the default mode for operation is to use a replicated mesh, which
creates a complete copy of the mesh for each processor.

```text
parallel_type = replicated
```

!---

## Distributed Mesh

Changing the type to distributed when running in parallel operates such that only the portion of the
mesh owned by a processor is stored on that processor.

```text
parallel_type = distributed
```

If the mesh is too large to read in on a single processor, it can be split prior to the simulation.

1. Copy the mesh to a large memory machine
1. Use the `--split-mesh` option to split the mesh into $n$ pieces
1. Run the executable with `--use-split`

!---

## Displaced Mesh

Calculations can take place in either the initial mesh configuration or, when requested, the
"displaced" configuration.

To enable displacements, provide a vector of displacement variable names for each spatial dimension
in the Mesh block.

!row!

!col! width=45%

!style! fontsize=60%



!listing /displaced/child.i block=Mesh

!style-end!

!col-end!

!col! width=5%
\\
!col-end!

!col! width=45%

Objects can enforce the use of the displaced mesh within the validParams function.

!style! fontsize=60%

!listing PenetrationAux.C line=use_displaced_mesh

!listing modules/solid_mechanics/test/tests/volumetric_eigenstrain/volumetric_mechanical.i block=Postprocessors/vol

!style-end!

!col-end!

!row-end!

!---

# [Variable System](syntax/Variables/index.md)

A system for defining primary unknown variables to be solved for using a nonlinear system of PDEs.

!---

## Variables

- Defines the primary "unknowns" in the system of PDEs
- Variables are associated with finite element shape functions
- Referenced by `Kernel` objects to define the governing equations
- Form the solution vector in the nonlinear system

!---

## Variable Declaration

Variables are declared in the `[Variables]` input file block:

```
[Variables]
  [temp]
    order = FIRST
    family = LAGRANGE
  []
[]
```

- `order`: Polynomial order (FIRST, SECOND, etc.)
- `family`: Shape function family (LAGRANGE, HERMITE, MONOMIAL, etc.)

!---

## Variable Types

MOOSE supports several variable types:

- *Nodal Variables*: Continuous finite-element variables (typically Lagrange basis)
- *Elemental Variables*: Element-wise/discontinuous variables (typically monomial basis)
- *Scalar Variables*: Single, spatially-uniform values not tied to the mesh — useful for global constraints, Lagrange multipliers, or ODE unknowns
- *Finite Volume Variables*: Variables for cell-centered finite volume methods

!---

## Variable Initial Conditions

Variables can be assigned initial conditions:

```
[Variables]
  [u]
    order = FIRST
    family = LAGRANGE
    [InitialCondition]
      type = FunctionIC
      function = x*y
    []
  []
[]
```

- Initial conditions can be constant, function-based, or random
- More complex ICs can be defined in a separate `[ICs]` block

!---

## Variable Scaling

Variable scaling improves solver convergence for multiphysics problems:

```
[Variables]
  [pressure]
    scaling = 1e-6  # Scale down for better conditioning
  []
  [temperature]
    scaling = 1.0
  []
[]
```

- Helps balance variables with different physical units
- Improves conditioning of the Jacobian matrix
- Particularly important for coupled multiphysics problems

!---

## Coupling Variables

Variables can be explicitly coupled when necessary:

```
[Kernels]
  [heat_conduction]
    type = HeatConduction
    variable = temperature
  []
[]
```

- Coupling indicates dependencies between physics
- Makes another variable's value/gradient available inside the kernel
- Proper Jacobian assembly comes from `computeQpOffDiagJacobian` (non-AD) or automatic differentiation (AD), not from the coupling declaration alone
- Enables preconditioning optimization

!---

# [Kernel System](syntax/Kernels/index.md)

A system for implementing the physics of a PDE by defining the residual and Jacobian contributions.

!---

## Kernels: The Physics Building Blocks

- A "Kernel" represents a piece of physics
- Implements terms in the weak form of PDEs
- Evaluates residuals at integration points
- Assembled to form the complete system of equations
- Available in standard and AD (Automatic Differentiation) versions

!---

## Kernel Structure

For standard `Kernel` objects:

- *Must override:* `computeQpResidual()`
- *Optionally override:* `computeQpJacobian()`, `computeQpOffDiagJacobian()`

For `ADKernel` objects:

- *Only override:* `computeQpResidual()`
- Jacobians calculated automatically through AD

!---

## Kernel Member Variables

- `_i`, `_j`: Current test and trial function indices
- `_qp`: Current quadrature point index
- `_u`, `_grad_u`: Variable value and gradient
- `_test`, `_grad_test`: Test function value and gradient
- `_phi`, `_grad_phi`: Trial function value and gradient
- `_q_point`: Coordinates of current quadrature point
- `_current_elem`: Pointer to current element

!---

## Custom Kernel Example: Diffusion

!row!

!col! width=45%

+Weak Form+

!equation
(\nabla u, \nabla \psi_i)\quad \forall\,\psi_i

+Standard Implementation+

!style! fontsize=70%

```cpp
Real
Diffusion::computeQpResidual()
{
  return _grad_u[_qp] * _grad_test[_i][_qp];
}
```

!style-end!

!col-end!

!col! width=5%
\\
!col-end!

!col! width=45%

+Jacobian Term+

!equation
(\nabla \phi_j, \nabla \psi_i)\quad \forall\,\psi_i

+Standard Jacobian+

!style! fontsize=70%

```cpp
Real
Diffusion::computeQpJacobian()
{
  return _grad_phi[_j][_qp] * _grad_test[_i][_qp];
}
```

!style-end!

!col-end!

!row-end!

!---

## AD Version: Simpler Implementation

!row!

!col! width=50%

+AD Implementation+

!style! fontsize=70%

```cpp
ADRealVectorValue
ADDiffusion::precomputeQpResidual()
{
  return _grad_u[_qp];
}
```

!style-end!

ADDiffusion derives from `ADKernelGrad`: `precomputeQpResidual` returns the gradient vector; the base contracts it with `_grad_test` and applies integration weights.

+Benefits+

- No need to implement Jacobian
- Automatic calculation of derivatives
- Less error-prone
- Better for complex physics

!col-end!

!col! width=5%
\\
!col-end!

!col! width=50%

+How AD Works+

- Uses dual numbers approach
- Tracks derivatives along with values
- Applies chain rule automatically
- Computes exact Jacobian entries


!col-end!

!row-end!

!---

## Example: Complete Diffusion Input

```
[Kernels]
  [diffusion]
    type = ADDiffusion
    variable = temperature
  []
  [time_derivative]
    type = ADTimeDerivative
    variable = temperature
  []
[]
```

This defines a simple heat equation with diffusion and time derivative terms.

!---

# [Material System](syntax/Materials/index.md)

A system for defining material properties to be used by multiple systems and allow for variable
coupling.

!---

The material system operates by creating a producer/consumer relationship among objects

- `Material` objects +produce+ properties.
- Other MOOSE objects (including materials) +consume+ these properties.

!---

## Producing Properties

1. Each property to be produced must be declared to be available for use, the
   `declareProperty<TYPE>()` method does this and returns a writable reference.
1. Override `computeQpProperties()` to compute all of the declared properties at one quadrature point.
   Within this method, the references obtained from declaring the property are updated.

!---

## Consuming Properties

To consume a material property, call the correct get method in an object and store the
constant reference as a member variable.

`getMaterialProperty<TYPE>()`\\
Use within non-AD objects to retrieve non-AD material properties.

`getADMaterialProperty<TYPE>()`\\
Use within AD objects to retrieve AD material properties.

!---

## Material Property Evaluation

Quantities are recomputed at quadrature points, as needed.

Multiple `Material` objects may define the same "property" for different parts of the subdomain or
boundaries.

!---

## Stateful Material Properties

The values are not stored between timesteps unless "stateful" properties are enabled, which is
accomplished by calling `getMaterialPropertyOld<TYPE>()` or `getMaterialPropertyOlder<TYPE>()`

It can be useful to have "old" values of `Material` properties available in a simulation, such as
in solid mechanics plasticity constitutive models.

Traditionally, this type of value is called a "state variable"; in MOOSE, they are called
"stateful material properties".

Stateful `Material` properties require more memory.

!---

## Default Material Properties

Default values for material properties may be assigned within the `validParams` function.

```cpp
addParam<MaterialPropertyName>("combination_property_name", 12345,
 "The name of the property providing the luggage combination");
```

Only scalar (`Real`) values may have defaults.

When `getMaterialProperty<Real>("combination_property_name")` is called, the default will be returned
if the value has not been computed via a `declareProperty` call within a `Material` object.

!---

## Material Property Output

Output of `Material` properties is enabled by setting the "outputs" parameter.

The following example creates additional variables called "real_property", "tensor_property", and "vector_property" that will show up in
the output file.

!listing output_block.i block=Materials Outputs

!---

## Supported Property Types for Output

`Material` properties can be of arbitrary (C++) type, but not all types can be output.

| Type | AuxKernel | Variable Name(s) |
| :- | :- | :- |
| Real | `MaterialRealAux` | prop |
| RealVectorValue | `MaterialRealVectorValueAux` | prop_1, prop_2, and prop_3 |
| RealTensorValue | `MaterialRealTensorValueAux` | prop_11, prop_12, prop_13, prop_21, etc. |

!---

# [Boundary Condition System](syntax/BCs/index.md)

System for computing residual contributions from boundary terms of a [!ac](PDE).

!---

A `BoundaryCondition` (BC) object computes a residual on a boundary (or internal side) of a domain.

There are two flavors of BC objects: Nodal and Integrated.

!---

## Integrated BC

Integrated BCs are integrated over a boundary or internal side and should inherit
from `ADIntegratedBC`. A non-AD `IntegratedBC` base class also exists for cases requiring hand-coded Jacobians.

The structure is very similar to Kernels: objects must override `computeQpResidual`

!---

## ADIntegratedBC Object Members

`_u`, `_grad_u`\\
Value and gradient of the variable this boundary condition is operating on

`_test`, `_grad_test`\\
Value ($\psi$) and gradient ($\nabla \psi$) of the test functions at the quadrature points

`_phi`\\
Value ($\phi$) of the trial functions at the quadrature points

`_i`, `_j`, `_qp`\\
Current index for test function, trial functions, and quadrature point, respectively

`_normals`:\\
Outward normal vector for boundary element

`_current_boundary_id`\\
The boundary ID

`_current_elem`, `_current_side`\\
A pointer to the element and index to the current boundary side

+Note:+ in an AD BC, override only `computeQpResidual`; the Jacobian is formed automatically.

!---

## Nodal BC

Nodal BCs set values of the residual directly at the nodes of a boundary (nodeset) and
should inherit from `ADNodalBC`.

The structure is very similar to Kernels: objects must override `computeQpResidual`.

!---

## NodalBC Object Members

`_u`\\
Value of the variable this BC is operating on

`_qp`\\
Pseudo quadrature-point index, always 0 for a nodal BC; present for API consistency with Kernels

`_current_node`\\
A pointer to the current node that is being operated on.

!---

## Dirichlet BCs

Set a condition on the `value` of a variable on a boundary:

!equation
u = g_1 \quad \text{on} \quad \partial\Omega_1

becomes

!equation
u - g_1 = 0 \quad \text{on} \quad \partial\Omega_1

!---

## Integrated BCs

Integrated BCs (including Neumann BCs) are actually integrated over the external face of an element.

!equation
\left\{
   \begin{array}{rl}
     (\nabla u, \nabla \psi_i) - (f, \psi_i) - \langle \nabla u\cdot \hat{\boldsymbol n}, \psi_i\rangle &= 0 \quad \forall i
    \\
      \nabla u \cdot \hat{\boldsymbol n} &= g_1\quad \text{on} \quad\partial\Omega
   \end{array}
\right.

becomes:

!equation
(\nabla u, \nabla \psi_i) - (f, \psi_i) - \langle g_1, \psi_i\rangle = 0 \quad \forall i

If $\nabla u \cdot \hat{\boldsymbol n} = 0$, then the boundary integral is zero
("natural boundary condition").

!---

```
[BCs]
  [fixed_temp]
    type = DirichletBC
    variable = temperature
    boundary = left
    value = 200
  []
  [heat_flux]
    type = NeumannBC
    variable = temperature
    boundary = top
    value = 2
  []
[]
```

!---

## Periodic BCs

Periodic boundary conditions are useful for modeling quasi-infinite domains and systems with
conserved quantities.

- 1D, 2D, and 3D
- With mesh adaptivity
- Can be restricted to specific variables
- Supports arbitrary translation vectors for defining periodicity

!---

# [Auxiliary System](syntax/AuxVariables/index.md)

A system for direct calculation of field variables ("AuxVariables") that is designed for
postprocessing, coupling, and proxy calculations.

!---

The term "nonlinear variable" is defined, in MOOSE language, as a variable that is being solved for
using a nonlinear system of [!ac](PDEs) using `Kernel` and `BoundaryCondition` objects.


The term "auxiliary variable" is defined, in MOOSE language, as a variable that is directly
calculated using an `AuxKernel` object.

!---

## AuxVariables

Auxiliary variables are declared in the `[AuxVariables]` input file block

Auxiliary variables are field variables that are associated with finite element shape functions
and can serve as a proxy for nonlinear variables

Auxiliary variables currently come in two flavors:

- Element (e.g. constant or higher order monomials)
- Nodal (e.g. linear Lagrange)

Auxiliary variables have "old" and "older" states, from previous time steps, just like nonlinear variables

!---

### Elemental Auxiliary Variables

Element auxiliary variables can compute:

- average values per element, if stored in a constant monomial variable
- spatial profiles using higher order variables

AuxKernel objects computing elemental values can couple to nonlinear variables and both element and
nodal auxiliary variables

```text
[AuxVariables]
  [aux]
    order = CONSTANT
    family = MONOMIAL
  []
[]
```

!---

### Nodal Auxiliary Variables

Nodal auxiliary variables are computed at each node and are stored as linear Lagrange variables

AuxKernel objects computing nodal values can +only+ couple to nodal nonlinear variables and
other nodal auxiliary variables

```text
[AuxVariables]
  [aux]
    order = FIRST
    family = LAGRANGE
  []
[]
```

!---

# [Executioner System](syntax/Executioner/index.md)

A system for dictating the simulation solving strategy.

!---

## Steady-state Executioner

Steady-state executioners generally solve the nonlinear system just once.

!listing steady_time.i block=Executioner

The Steady executioner can solve the nonlinear system multiple times while adaptively
refining the mesh to improve the solution.

!---

## Transient Executioners

Transient executioners solve the nonlinear system at least once per time step.

| Option | Definition
| :- | :- |
| `dt` | Starting time step size |
| `num_steps` | Number of time steps |
| `start_time` | The start time of the simulation |
| `end_time` | The end time of the simulation |
| `scheme` | Time integration scheme (discussed next) |


!listing executioner/transient.i block=Executioner

!---

### Steady-State Detection

| Option | Definition |
| :- | :- |
| `steady_state_detection` | Whether to try and detect achievement of steady-state (Default = `false`) |
| `steady_state_tolerance` | Used for determining a steady-state; Compared against the difference in solution vectors between current and old time steps (Default = `1e-8`) |

!---

## Common Executioner Options

There are a number of options that appear in the executioner block and are used to control the
solver. Here are a few common options:

| Option | Definition |
| :- | :- |
| `l_tol` | Linear Tolerance (default: 1e-5) |
| `l_max_its` | Max Linear Iterations (default: 10000) |
| `nl_rel_tol` | Nonlinear Relative Tolerance (default: 1e-8) |
| `nl_abs_tol` | Nonlinear Absolute Tolerance (default: 1e-50) |
| `nl_max_its` | Max Nonlinear Iterations (default: 50) |

!---

# [Time Integrator System](syntax/Executioner/TimeIntegrator/index.md)

A system for defining schemes for numerical integration in time.

!---

The TimeIntegrator can be set using "scheme" parameter within the `[Executioner]` block, if
the "type = Transient", the following options exist:

- `implicit-euler`: Backward Euler (default)
- `bdf2`: Second order backward difference formula
- `crank-nicolson`: Second order Crank-Nicolson method
- `dirk`: Second order Diagonally-Implicit Runge-Kutta (DIRK)
- `newmark-beta`: Second order Newmark-beta method (structural dynamics)

!---

## TimeIntegrator Block

It is also possible to specify a time integrator as a separate sub-block within the input file.
This allows for additional types and parameters to be defined, including custom TimeIntegrator
objects.

!listing newmark_beta_prescribed_parameters.i block=Executioner

!---

#  [Time Stepper System](syntax/Executioner/TimeSteppers/index.md)

A system for suggesting time steps for transient executioners.

!---

!listing adapt_tstep_grow_dtfunc.i block=Executioner

Custom objects are created by inheriting from `TimeStepper` overriding `computeDT()`.

!---

## Built-in TimeSteppers

MOOSE includes many built-in TimeStepper objects:

- `ConstantDT`
- `IterationAdaptiveDT`
- `FunctionDT`
- `PostprocessorDT`
- `FixedPointIterationAdaptiveDT`
- `TimeSequenceStepper`

!---

## IterationAdaptiveDT

IterationAdaptiveDT grows or shrinks the time step based on the number of iterations taken to obtain
a converged solution in the last converged step.

!listing adapt_tstep_shrink_init_dt.i block=Executioner

!---

## TimeSequenceStepper

Provide a vector of time points using parameter `time_sequence`, the object simply moves through
these time points.

The $t_{start}$ and $t_{end}$ parameters are automatically added to the sequence.

Only time points satisfying $t_{start} < t \le t_{end}$ are considered.

If a solve fails at step $n$ an additional time point $t_{new} = \frac{1}{2}(t_{n+1}+t_n)$ is
inserted and the step is resolved.

!---

## Composing TimeSteppers

Time steppers can now be composed to follow complex time histories.
By default, the minimum of all the time steps computed by all the time steppers is used!

What steps will be taken, starting at time = 0s?

```bash
[Executioner]
  [TimeSteppers]
    [constant]
      type = ConstantDT
      dt = 0.2
    []
    [hit_these_times]
      type = TimeSequenceStepper
      time_sequence = '0.5 1 1.5 2.1'
    []
  []
[]
```

!---

# [Output System](syntax/Outputs/index.md)

A system for outputting simulation data to the screen or files.

!---

The output system is designed to be just like any other system in MOOSE: modular and expandable.

It is possible to create multiple output objects for outputting:

- at specific time or timestep intervals,
- custom subsets of variables, and
- to various file types.

There exists a short-cut syntax for common output types as well as common parameters.

!---

## Short-cut Syntax

The following two methods for creating an Output object are equivalent within the internals of MOOSE.

```text
[Outputs]
  exodus = true
[]
```

```text
[Outputs]
  [out]
    type = Exodus
  []
[]
```

!---

## Customizing Output

The content of each `Output` can customized, see for example for an [Exodus](Exodus.md) output:

```
[Outputs]
  [out]
    type = Exodus
    output_material_properties = true
    # removes some quantities from the output
    hide = 'power_pp pressure_var'
  []
[]
```

!---

## Common Parameters

```text
[Outputs]
  time_step_interval = 10 # this is a time step interval
  [exo]
    type = Exodus
    time_step_interval = 1 # overrides time_step_interval from top-level
  []
  [cp]
    type = Checkpoint # Uses time_step_interval specified from top-level
  []
[]
```

!---

## Output Names

The default naming scheme for output files utilizes the input file name (e.g., input.i) with a suffix
that differs depending on how the output is defined: An "_out" suffix is used for Outputs created
using the short-cut syntax.  sub-blocks use the actual sub-block name as the suffix.

```text
[Outputs]
  exodus = true    # creates input_out.e
  [other]          # creates input_other.e
     type = Exodus
     time_step_interval = 2
  []
  [base]
    type = Exodus
    file_base = out # creates out.e
  []
[]
```

!---

!style fontsize=85%
!include output_types.md

Paraview can read many of these (CSV, Exodus, Nemesis, VTK, GMV)

!---

# [Postprocessor System](syntax/Postprocessors/index.md)

A system for computing a "reduction" or "aggregation" calculation based on the solution variables
that results in a +single+ scalar value.

!---

## Types of Postprocessors

The operation defined in the `::compute...` routine is applied at various locations
depending on the Postprocessor type.

ElementPostprocessor: operates on each element

NodalPostprocessor: operates on each node

SidePostprocessor: operates on each element side on a boundary

InternalSidePostprocessor: operates on internal element sides

InterfacePostprocessor: operates on each element side on subdomain interfaces

GeneralPostprocessor: operates once per execution

!---

## Postprocessor Anatomy

`Postprocessor` is a UserObject, so `initialize`, `execute`, `threadJoin`, and `finalize` methods
can be defined.

`initialize()`\\
This is called once before every execution. Useful to reset accumulated quantities

`execute()`\\
This defines the operation performed on a per element/side/node/mesh (depending on type) basis.
The quadrature integration is often defined there, and users generally do not need to define this.

`PostprocessorValue getValue() const`\\
This is called internally within MOOSE to retrieve the final scalar value, the value returned by
this function is referenced by all other objects that are using the postprocessor.

Most Postprocessor base classes will already define these routines for you!

!---

## Aggregation Routines

If the Postprocessor created has custom data it must be ensured that the value is communicated
properly in (both MPI and thread-based) parallel simulations.

For MPI several utility methods exist to perform common aggregation operations:

- `gatherSum(scalar)`: sum across all processors.
- `gatherMin(scalar)`: min from all processors.
- `gatherMax(scalar)`: max from all processors.

!---

## Built-in Postprocessor Types

MOOSE includes a large number built-in `Postprocessors`: `ElementAverageValue`, `SideAverageValue`,
`ElementL2Error`, `ElementH1Error`, and many more

By default, `Postprocessors` will output to a formatted table on the screen and optionally using
the `[Outputs]` block be stored in CSV file.

```text
[Outputs]
  csv = true
[]
```

!---

## Using a Postprocessor

Postprocessor values are used within an object by creating a `const` reference to a
`PostprocessorValue` and initializing the reference in the initialization list of the object constructor.

In the header, we declare a reference,

!listing PostprocessorDT.h line=PostprocessorValue

In the source, we retrieve a reference to the value of the Postprocessor,

!listing PostprocessorDT.C line=getPostprocessorValue

!---

## Default Postprocessor Values

It is possible to set default values for `Postprocessors` to allow an object to operate without
creating or specifying a `Postprocessor` object.

```cpp
params.addParam<PostprocessorName>("postprocessor", 1.2345, "Doc String");
```

Additionally, a value may be supplied in the input file in lieu of a `Postprocessor` name.

!---

# [VectorPostprocessor System](syntax/VectorPostprocessors/index.md)

A system for "reduction" or "aggregation" calculations based on the solution variables
that results in one or many vectors of values.

!---

## Types of VectorPostprocessors

The operation defined in the `::compute...` routine is applied at various locations
depending on the VectorPostprocessor type.

ElementVectorPostprocessor: operates on each element

NodalVectorPostprocessor: operates on each node

SideVectorPostprocessor: operates on each element side on a boundary

InternalSideVectorPostprocessor: operates on internal element sides

GeneralVectorPostprocessor: operates once per execution

!---

## VectorPostprocessor Anatomy

`VectorPostprocessor` is a UserObject, so `initialize`, `execute`, `threadJoin`, and `finalize` methods
are used for implementing the aggregation operation.

`VectorPostprocessorValue & declareVector(const std::string & vector_name)`
Produces and registers a result vector inside the VectorPostprocessor; `getVectorNames()` returns the
set of declared vector names. Other objects that consume this VectorPostprocessor read its results
via the VectorPostprocessorInterface accessor `getVectorPostprocessorValue(param_name, vector_name)`.

!---

VectorPostprocessor objects operate a bit like Material objects, each vector is declared and then
within the `initialize`, `execute`, `threadJoin`, and `finalize` methods the vectors are updated
with the desired data.

Create a member variable, as a reference, for the vector data

!listing WorkBalance.h line=_pid


Initialize the reference using the `declareVector` method with a name

!listing WorkBalance.C line=declareVector("pid")

!---

## Built-in VectorPostprocessor Types

MOOSE includes a large number built-in `VectorPostprocessors`: `NodalValueSampler`,
`LineValueSampler`, `PointValueSampler`, and many more.

`VectorPostprocessors` output is optionally enabled using the `[Outputs]` block. A CSV file
for each vector and timestep will be created.

```text
[Outputs]
  csv = true
[]
```

!---

## Using a VectorPostprocessor

Postprocessor values are used within an object by creating a `const` reference to a
`VectorPostprocessorValue` and initializing the reference in the initialization list.

!listing LeastSquaresFit.h line=_x_values;

!listing LeastSquaresFit.C line=_x_values(get

!---

# Day 2

## Solid Mechanics, Heat Conduction & Coupling

!---

# Introduction to Solid Mechanics

!row!

!col! width=50%

- Fundamental concepts and principles
- Stress and strain tensors
- Conservation equations

!col-end!

!col! width=50%


!media solid_mechanics/mechanics_potatoes.png
       id=potato
       style=width:100%;
       caption=Reference and Current Configuration

!col-end!

!row-end!

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
\boldsymbol{u} = \boldsymbol{g}\;\mathrm{on}\;\Gamma_{ \boldsymbol{g}}\\
\boldsymbol{\sigma} \cdot \boldsymbol{n}=\boldsymbol{t}\;\mathrm{on}\;\Gamma_{ \boldsymbol{t}}



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
  $\hat{\mathbf{F}}^{-1} = \mathbf{I} - \partial \hat{\mathbf{u}} / \partial \mathbf{x},$
  where $\hat{\mathbf{u}}$ is the incremental displacement and $\mathbf{x}$ is the position in the current (deformed) configuration $\kappa_{n+1}$ (contrast the reference-config gradient $\partial/\partial\mathbf{X}$ used for $\mathbf{F}$).
- Each time step updates strain and rotation incrementally, which is then added to the previous total state.

!---

# Closed Loop Large Deformation Loading Cycle


!media solid_mechanics/closed_loop_large_deform_cycle_loading.png
       id=closed_loop_cycle_loading
       style=width:85%;float:right;padding-top:1.5%;
       caption=Closed loop large deformation loading cycle.

- Initial configuration (A) with dimensions $L \times L$.
- Intermediate stages show stretching ($\Delta y$) and shearing ($\Delta x$).
- Final shape (E) may differ from the start due to residual inelastic effects.

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
- `vol/no_vol` = with/without volumetric locking correction (F-bar)

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

!style! fontsize=85%

+Material System Components+

*Strain Calculator*

- ComputeSmallStrain
- ComputeFiniteStrain
- ComputePlaneSmallStrain

*Elasticity Tensor*

- ComputeIsotropicElasticityTensor
- ComputeElasticityTensor
- CompositeElasticityTensor

*Stress Calculator*

- ComputeLinearElasticStress
- ComputeFiniteStrainElasticStress
- ComputeMultipleInelasticStress

*Automatic Differentiation (AD):*

- Add "AD" prefix: ADComputeSmallStrain

!style-end!

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

# Motivation: One System for Both Regimes

The +traditional MOOSE path+ uses separate kernel and material workflows:

- *Small strain:* `StressDivergenceTensors` + `ComputeSmallStrain` + `ComputeLinearElasticStress`
- *Finite strain:* `StressDivergenceTensors` + `ComputeFiniteStrain` + hyperelastic materials

The +new Lagrangian system+ unifies these into AD-native kernels with explicit stress measures:

- Single kernel interface: `TotalLagrangianStressDivergence` or `UpdatedLagrangianStressDivergence`
- Decoupled kinematics: `ComputeLagrangianStrain` handles both small (linearized) and large deformation
- Stress measures: elastic + objective stresses via modular stress materials

Enable via: `new_system = true`, `formulation = TOTAL|UPDATED`

!---

# Total vs Updated Lagrangian Formulations

!row!

!col! width=50%

+Total Lagrangian+

- Weak form written in *reference* (undeformed) configuration $\kappa_0$
- Deformation gradient $\mathbf{F}$ maps $\kappa_0 \to \kappa_{n+1}$
- Total strain from origin; no accumulated rotation
- Second Piola-Kirchhoff (PK2) stress is work-conjugate to Green-Lagrange strain

!col-end!

!col! width=50%

+Updated Lagrangian+

- Weak form written in *current* (deformed) configuration $\kappa_n$
- Incremental deformation $\hat{\mathbf{F}}$ maps $\kappa_n \to \kappa_{n+1}$
- Strain accumulated incrementally each step
- Cauchy stress and Jaumann rate are naturally corotational

!col-end!

!row-end!

Both yield identical physics; choice depends on problem structure and preferred stress/strain measures.

!---

# Kinematics: Finite vs Small Strain Limit

The Lagrangian system unifies kinematics through a single material: `ComputeLagrangianStrain`.

Set **`large_kinematics = true`** in the strain material or globally to enable finite-deformation kinematics:

!equation
\mathbf{F} = \mathbf{I} + \nabla \mathbf{u}

When `large_kinematics = false` (default), the system linearizes and recovers small-strain theory:

!equation
\boldsymbol{\epsilon} \approx \frac{1}{2} (\nabla \mathbf{u} + (\nabla \mathbf{u})^T)

Both paths share the same strain material class; the flag switches internal kinematics without changing the input structure.

!---

# Deformation Gradient and Stress Measures

Key relationships in the new system:

!equation
\mathbf{P} = \mathbf{F} \mathbf{S}

where $\mathbf{P}$ is the first Piola-Kirchhoff (PK1) stress, $\mathbf{S}$ is the second Piola-Kirchhoff (PK2) stress, and $\mathbf{F}$ is the deformation gradient.

Cauchy (true) stress from PK1:

!equation
\boldsymbol{\sigma} = \frac{1}{|\mathbf{F}|} \mathbf{P} \mathbf{F}^T

- **Total Lagrangian** works naturally with PK2 and Green-Lagrange strain
- **Updated Lagrangian** outputs Cauchy stress and uses incremental strains
- **Objective stresses** (Truesdell, Jaumann, Green-Naghdi) maintain frame invariance during finite rotations

!---

# Available Stress Materials

!style! fontsize=85%

The new system provides elastic and hyperelastic stress calculators:

| Material | Stress Measure | Use Case |
| :- | :- | :- |
| `ComputeLagrangianLinearElasticStress` | PK2 | Linear elasticity (small or large strain) |
| `ComputeStVenantKirchhoffStress` | PK2 | Hyperelastic, St. Venant-Kirchhoff model |
| `ComputeNeoHookeanStress` | PK2 | Hyperelastic, compressible Neo-Hookean |
| `ComputeLagrangianWrappedStress` | Cauchy (objective) | Objective integration of small-strain models |
| `ComputeLagrangianStressBase` subclasses | Custom | User-defined constitutive models |

!style-end!

The `ComputeLagrangianWrappedStress` `objective_rate` parameter selects `truesdell`, `jaumann`, or `green_naghdi` integration for corotational behavior under finite rotation.

!---

# Enabling the New System via Action

The `[Physics/SolidMechanics/QuasiStatic]` action automatically selects the new Lagrangian kernels and materials when you set:

```
[Physics]
  [SolidMechanics]
    [QuasiStatic]
      [all]
        strain = FINITE              # or SMALL
        new_system = true            # Enable Lagrangian kernels
        formulation = TOTAL          # or UPDATED
        add_variables = true
      []
    []
  []
[]
```

- `new_system = true`: Selects `TotalLagrangianStressDivergence` or `UpdatedLagrangianStressDivergence`
- `formulation = TOTAL|UPDATED`: Chooses reference or current-config weak form
- `strain = FINITE|SMALL`: Drives the `large_kinematics` flag in the strain material

!---

# Example: Total Lagrangian, Linear Elastic

Action-based setup with `new_system = true`:

!listing modules/solid_mechanics/test/tests/lagrangian/cartesian/total/action/action_1D.i block=Physics

!---

# Material Configuration

Pair the strain calculator with a stress calculator (from a direct kernel example):

!listing modules/solid_mechanics/test/tests/lagrangian/materials/convergence/stvenantkirchhoff.i block=Materials

The action wires `displacements` to the strain material automatically. No manual coupling needed.

!---

# Direct Kernel Syntax (without action)

For fine-grained control, use kernels + materials directly:

!listing modules/solid_mechanics/test/tests/lagrangian/materials/convergence/stvenantkirchhoff.i block=Kernels

Each displacement component gets its own stress-divergence kernel. The `component` parameter is 0 (x), 1 (y), or 2 (z).

!---

# Stress Material Details

The strain material computes Green-Lagrange strain $\mathbf{E}$ and deformation gradient $\mathbf{F}$. The stress material applies a constitutive relation:

- `ComputeLagrangianLinearElasticStress`: $\mathbf{S} = \mathbb{C} : \mathbf{E}_{\text{elastic}}$ from a supplied elasticity tensor (works in both total and updated Lagrangian)
- `ComputeStVenantKirchhoffStress`: St. Venant-Kirchhoff hyperelastic model (recommended for large-deformation elasticity)
- `ComputeNeoHookeanStress`: compressible Neo-Hookean model for rubber-like materials

!---

# Key Advantages

1. +AD-native+: Full automatic differentiation throughout; no hand-coded Jacobians
2. +Unified kinematics+: One `ComputeLagrangianStrain` handles both small and large deformation via flag
3. +Explicit stress measures+: Choose PK2, Cauchy, or objective stresses via the stress material—not buried in kernel code
4. +Modular materials+: Swap stress calculator without rewriting kinematics or kernels
5. +Frame invariance+: Built-in objective stress options for corotational dynamics

Legacy (`StressDivergenceTensors` + `ComputeSmallStrain`/`ComputeFiniteStrain`) remains fully supported; use `new_system = false` (default) to preserve existing inputs.

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

  $\rho c\frac{\partial T(\vec{x}, t)}{\partial t} = \nabla \cdot [k \nabla T(\vec{x}, t)] + \dot{q}$
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

# Thermal Strain Decomposition

Total strain decomposes into elastic and thermal components:

!equation
\boldsymbol{\epsilon}_{\text{total}} = \boldsymbol{\epsilon}_{\text{elastic}} + \boldsymbol{\epsilon}_{\text{thermal}}

For isotropic thermal expansion:

!equation
\boldsymbol{\epsilon}_{\text{thermal}} = \alpha (T - T_{\text{ref}}) \mathbf{I}

where:

- $\alpha$ = thermal expansion coefficient
- $T$ = current temperature
- $T_{\text{ref}}$ = stress-free reference temperature
- $\mathbf{I}$ = identity tensor

!---

# Eigenstrain Mechanism in MOOSE

In MOOSE, thermal expansion enters as an +eigenstrain+ that is +subtracted+ from total strain in the strain calculator to form the elastic/mechanical strain, which then feeds the stress calculator ($\boldsymbol{\sigma} = \mathcal{C} : \boldsymbol{\epsilon}_{\text{elastic}}$):

!equation
\boldsymbol{\sigma} = \mathcal{C} : (\boldsymbol{\epsilon}_{\text{total}} - \boldsymbol{\epsilon}_{\text{eigen}})

The strain calculator computes:

!equation
\boldsymbol{\epsilon}_{\text{elastic}} = \boldsymbol{\epsilon}_{\text{total}} - \boldsymbol{\epsilon}_{\text{thermal}}

Then the stress is:

!equation
\boldsymbol{\sigma} = \mathcal{C} : \boldsymbol{\epsilon}_{\text{elastic}}

Eigenstrain approach decouples thermal and mechanical strains, simplifying constitutive integration.

!---

# Thermal Expansion Objects

MOOSE provides three material objects for different thermal expansion models:

+ComputeThermalExpansionEigenstrain+

- Constant thermal expansion coefficient
- Simplest model; good for small temperature ranges

+ComputeMeanThermalExpansionFunctionEigenstrain+

- Mean expansion coefficient supplied directly as a function of temperature
- Strain is $\bar{\alpha}(T)\,(T - T_{\text{ref}})$, where the mean coefficient relates to the instantaneous one by $\bar{\alpha}(T) = \frac{1}{T - T_{\text{ref}}} \int_{T_{\text{ref}}}^{T} \alpha(T')\, dT'$

+ComputeInstantaneousThermalExpansionFunctionEigenstrain+

- Instantaneous expansion coefficient $\alpha(T)$ supplied as a function
- Strain accumulated incrementally from $\alpha(T)$
- Higher accuracy for large temperature ranges

!---

# Wiring Temperature Coupling

The SolidMechanics action automatically wires strain calculators when you specify eigenstrain names:

!listing modules/solid_mechanics/test/tests/thermal_expansion/constant_expansion_coeff.i block=Physics

!---

# Wiring Temperature Coupling (cont.)

The thermal expansion material consumes the temperature variable:

!listing modules/solid_mechanics/test/tests/thermal_expansion/constant_expansion_coeff.i block=Materials

The `eigenstrain_name` links the material output to the strain calculator's `eigenstrain_names` list.

!---

# Constrained Heating: Thermal Stress Example

- Two blocks in thermal contact; temperature is imposed by a `FunctionDirichletBC` on the contacting boundaries and conducted through each block by the source-free `HeatConduction` kernel
- Thermal expansion (different $\alpha$ per block) is resisted by the fixed boundaries and the contact constraint, producing thermal stress
- This input also demonstrates FDP with `implicit_geometric_coupling` preconditioning

!equation
\sigma_{\text{th}} = -\alpha E (T - T_{\text{ref}})

!listing modules/combined/test/tests/fdp_geometric_coupling/fdp_geometric_coupling.i block=Kernels

!---

# Coupled Temperature-Displacement

One-way coupling: temperature prescribed, displacement computed.

!equation
\begin{cases}
\nabla \cdot (\mathcal{C} : (\nabla^s \mathbf{u} - \alpha(T)(T - T_{\text{ref}}) \mathbf{I})) + \mathbf{b} = 0 \\
T = T_{\text{prescribed}}
\end{cases}

Two-way coupling: heat conduction evolves temperature; displacement feeds back via work.

!equation
\begin{cases}
\nabla \cdot (\mathcal{C} : (\nabla^s \mathbf{u} - \alpha(T)(T - T_{\text{ref}}) \mathbf{I})) + \mathbf{b} = 0 \\
\rho c \frac{\partial T}{\partial t} = \nabla \cdot (k \nabla T)
\end{cases}

In MOOSE, both are solved monolithically; one-way is recovered by omitting the HeatConduction kernel.

!---

# Coupled Thermo-Mechanics Input

Both temperature and displacement are primary variables:

!listing modules/combined/test/tests/fdp_geometric_coupling/fdp_geometric_coupling.i block=Variables

!---

# Coupled Thermo-Mechanics Input (cont.)

Each block can have different eigenstrain objects:

!listing modules/combined/test/tests/fdp_geometric_coupling/fdp_geometric_coupling.i block=Physics/SolidMechanics/QuasiStatic

Temperature is passed to the QuasiStatic Physics action via the `temperature` parameter; the action forwards it to the eigenstrain calculators.

!---

# Day 3

## Thermal Radiation & Solver Strategies

!---

# Thermal Radiation: Stefan-Boltzmann Fundamentals

Thermal radiation is energy transfer via electromagnetic waves. A perfect (black) body emits power according to:

!equation
q = \sigma T^4

where:

- $\sigma = 5.67 \times 10^{-8}$ W/(m$^2$ K$^4$) is the Stefan-Boltzmann constant
- $T$ is absolute temperature (K)

!---

# Gray-Body Approximation

Real surfaces are gray bodies with emissivity $\varepsilon$ (0 to 1):

!equation
q = \varepsilon \sigma T^4

Gray assumption:

- Emissivity is wavelength-independent
- Valid for most engineering materials over moderate temperature ranges

Additional idealization (often assumed alongside gray):

- Diffuse (Lambertian): emission is direction-independent
- MOOSE's GrayLambert objects combine both the gray and diffuse assumptions

!---

# Surface-to-Ambient Radiation

Radiative boundary condition for a surface at temperature $T$ radiating to ambient (deep space) at $T_{\infty}$:

!equation
q_r = \varepsilon \sigma (T^4 - T_{\infty}^4)

Boundary condition in MOOSE:

!listing modules/heat_transfer/test/tests/radiative_bcs/function_radiative_bc.i block=BCs/bot_right

Emissivity is supplied as a MOOSE Function of time/position (constant in the example shown). It is not a function of the temperature variable; for temperature-dependent emissivity use a material-property-based radiative BC.

!---

# FunctionRadiativeBC: General Surface-to-Ambient

`FunctionRadiativeBC` applies radiative heat flux to a surface:

!equation
-k \nabla T \cdot \hat{n} = \varepsilon \sigma (T^4 - T_{\infty}^4)

Parameters:

- `emissivity_function`: emissivity $\varepsilon$ as function or constant
- `Tinfinity`: far-field temperature $T_{\infty}$ (typically 0 for space)
- Works for planar, curved, and complex geometries

!---

# Cylindrical Surface Radiation

For two coaxial cylinders, an inner surface at temperature $T_s$ (radius $r_s$, emissivity $\varepsilon_s$) and an outer cylinder at $T_f$ (radius $r_f$, emissivity $\varepsilon_f$):

!equation
q_r = \sigma F_e (T_s^4 - T_f^4)

where the exchange factor accounts for radii and emissivities (the form used by MOOSE):

!equation
F_e = \frac{\varepsilon_s \varepsilon_f r_f}{\varepsilon_f r_f + \varepsilon_s r_s (1 - \varepsilon_f)}

!---

# InfiniteCylinderRadiativeBC

Applies radiation between two coaxial cylinders with different emissivities:

!listing modules/heat_transfer/test/tests/radiative_bcs/radiative_bc_cyl.i block=BCs/radiative_bc

Parameters:

- `boundary_radius`: radius of inner surface
- `boundary_emissivity`: emissivity of inner surface
- `cylinder_radius`: radius of outer surface
- `cylinder_emissivity`: emissivity of outer surface

!---

# Gray-Body Enclosure Radiation: View Factors

Radiation in an enclosure with $N$ surfaces depends on view factors $F_{ij}$ — the fraction of energy leaving surface $i$ reaching surface $j$.

Conceptually, the net flux at surface $i$ balances the radiation leaving it against the radiation arriving from the other surfaces. The simple emission-only form below ignores surface reflection; MOOSE instead solves a radiosity linear system that accounts for it.

MOOSE solves for the per-area radiosity $J_i$ (total radiation leaving each surface):

!equation
(\mathbf{I} - (\mathbf{I}-\boldsymbol{\varepsilon})\mathbf{F})\,\mathbf{J} = \boldsymbol{\varepsilon}\,\sigma\,\mathbf{T}^4

The net radiative flux at surface $i$ is then:

!equation
q_i = J_i - \sum_j F_{ij} J_j

Reciprocity: $A_i F_{ij} = A_j F_{ji}$

Summation rule: $\sum_j F_{ij} = 1$

!---

# Computing View Factors

Three methods in heat_transfer module:

1. +Specified/Constant+: Pre-computed externally or from symmetry; user provides matrix
2. +UnobstructedPlanarViewFactor+: Numerical double-area-integral view factors over unobstructed planar surfaces (any relative orientation, line-of-sight only; 2D or 3D)
3. +RayTracingViewFactor+: Ray-tracing with a deterministic angular quadrature for arbitrary/obstructed geometries

!---

# Specified View Factors

Pre-computed matrix input to the gray-body enclosure solver:

!listing modules/heat_transfer/test/tests/gray_lambert_radiator/gray_lambert_cavity.i block=UserObjects/view_factors_uo

The `SpecifiedViewFactor` object defines the view-factor matrix and loads it into a `ViewFactorObjectSurfaceRadiation` (shown next).

!---

# GrayLambert Enclosure Solver

`ViewFactorObjectSurfaceRadiation` solves the gray-body enclosure problem using pre-computed view factors:

!listing modules/heat_transfer/test/tests/gray_lambert_radiator/gray_lambert_cavity.i block=UserObjects/gray_lambert

- `boundary`: list of radiating surfaces
- `fixed_temperature_boundary`: surfaces held at specified $T$
- `adiabatic_boundary`: insulated surfaces (solve for $T$)
- `emissivity`: emissivity of each surface

!---

# Unobstructed Planar View Factors

For parallel or perpendicular planar surfaces:

!listing modules/heat_transfer/test/tests/view_factors/view_factor_2d.i block=UserObjects/unobstructed_vf

Used for simple geometries without obstructions. Faster than ray-tracing.

!---

# Ray-Tracing View Factors

For complex, obstructed geometries:

!row!

!col! width=50%

!listing modules/heat_transfer/test/tests/view_factors/view_factor_2d.i block=UserObjects/vf_study

!col-end!

!col! width=50%

!listing modules/heat_transfer/test/tests/view_factors/view_factor_2d.i block=UserObjects/rt_vf

!col-end!

!row-end!

`ViewFactorRayStudy` sets the quadrature; `RayTracingViewFactor` computes $F_{ij}$ by deterministic ray tracing over an angular quadrature — slower, but handles arbitrary geometry, obstructions, and self-shadowing. Accuracy is controlled by `polar_quad_order`/`azimuthal_quad_order`/`face_order`, not by a random sample size.

!---

# Application: Space Reactor Radiator

Space reactors reject heat via radiators to deep space ($T_{\infty} \approx 3$ K):

+Design Challenge+:

- Surface temperatures 500—1000 K
- Radiation is only mechanism (no convection in vacuum)
- View factor to cold space is ~1 (no obstructions)
- Radiator area must satisfy: $q = \varepsilon \sigma A (T^4 - T_{\infty}^4)$

+MOOSE Approach+:

- Use `FunctionRadiativeBC` on radiator surface with $\varepsilon = 0.85$ (typical)
- Set $T_{\infty} = 0$ or 3 K
- Solve transient heat conduction through reactor structure and radiator

!---

# Space Reactor Example: Coupled System

Thermal model couples:

1. Core (internal heat generation)
2. Radiator pipes (conduction through walls)
3. Radiative BC to space

!equation
\rho c \frac{\partial T}{\partial t} = \nabla \cdot (k \nabla T) + q'''

with BC: $-k \frac{\partial T}{\partial n} = \varepsilon \sigma (T^4 - T_{\infty}^4)$

The nonlinear solver handles the $T^4$ terms automatically with Newton's method.

!---

# Automatic Differentiation for Radiation

AD versions compute the Jacobian automatically:

- `ADFunctionRadiativeBC`
- `ADInfiniteCylinderRadiativeBC`

No need to hand-code derivatives of $T^4$ terms — AD handles them.

!listing modules/heat_transfer/test/tests/radiative_bcs/ad_function_radiative_bc.i block=BCs/bot_right

!---

# Nonlinear Solvers: Newton and JFNK

Newton's method solves:

!equation
\mathbf{J}(\vec{u}_n) \delta\vec{u}_{n+1} = -\vec{R}(\vec{u}_n), \quad \vec{u}_{n+1} = \vec{u}_n + \delta\vec{u}_{n+1}

+Key traits:+

- Quadratic convergence near the solution
- Requires Jacobian: hand-coded, AD, or approximated
- Robust when initial guess is good

!---

# Thermo-Mechanical Nonlinearity: Radiation

Thermal radiation introduces a strong $T^4$ nonlinearity:

!equation
q_r = \sigma \epsilon (T^4 - T_\infty^4)

+Why Newton works well here:+

- The residual is smooth and an exact (or AD) Jacobian $\frac{\partial q_r}{\partial T} = 4\sigma\epsilon T^3$ is available
- These give Newton its quadratic convergence near the solution
- Few iterations typically needed (2--4) once close to the solution
- But the strong $T^4$ nonlinearity has a small convergence radius: far from the solution it can hurt robustness, often needing a good initial guess, line search, or temperature/time stepping

!---

# JFNK: When to Use It

+Jacobian-Free Newton-Krylov approximates:+

!equation
\mathbf{J}\vec{v} \approx \frac{\vec{R}(\vec{u} + \epsilon\vec{v}) - \vec{R}(\vec{u})}{\epsilon}

+When JFNK helps:+

- Jacobian derivation is tedious or error-prone
- Complex multiphysics with weak coupling
- Memory is tight (no explicit matrix storage)
- Development speed > execution speed

+When JFNK struggles:+

- Strong nonlinearities like $T^4$ radiation (better: full Newton)
- Stiff systems needing strong preconditioning

!---

# Newton Implementation Options in MOOSE

!row!

!col! width=33%

+Hand-coded Jacobians+

- Explicit derivatives
- Most efficient
- Tedious for complex physics

!col-end!

!col! width=33%

+Automatic Differentiation+

- Computes Jacobian automatically
- Modern default
- Trade small CPU cost for zero errors

!col-end!

!col! width=33%

+PJFNK (Preconditioned)+

- Approx. Jacobian for preconditioner
- Jacobian-free outer iteration
- Good compromise

!col-end!

!row-end!

!---

# Linear Solvers: Direct vs Iterative

!row!

!col! width=50%

+Direct (LU, MUMPS, SuperLU_DIST)+

- Solve $\mathbf{A}\vec{x} = \vec{b}$ in one step
- Very robust
- Memory and parallel scaling limited

+When to use:+

- Small to medium systems
- Ill-conditioned problems
- Need guaranteed convergence

!col-end!

!col! width=50%

+Iterative Krylov (GMRES, CG)+

- Solve via repeated matrix-vector products
- Need good preconditioner
- Scalable to large systems

+When to use:+

- Large sparse systems
- Good preconditioning available
- Memory limited

!col-end!

!row-end!

!---

# Reading Linear Iteration Counts

Monitor convergence of the inner (linear) solver:

```
[Executioner]
  l_max_its = 200
  l_tol = 1e-5
[]
[Outputs]
  print_linear_residuals = true
[]
```

+Output example:+

```
Linear solve converged in 23 iterations
Linear solve converged in 15 iterations
```

+Diagnosis:+

- Dropping counts: preconditioning is helping
- High counts (> 100): preconditioning is weak
- Max iterations reached: preconditioner not suitable

!---

# Preconditioning: The Solver's Multiplier

Transform the system to reduce condition number:

!equation
\mathbf{M}^{-1}\mathbf{A}\vec{x} = \mathbf{M}^{-1}\vec{b}

+Ideal preconditioner:+

- $\mathbf{M}^{-1} \approx \mathbf{A}^{-1}$ (close to true inverse)
- Cheap to apply
- Improves eigenvalue clustering

+Effect:+

- Good PC: 5--20 linear iterations
- Poor PC: 50--200+ iterations or non-convergence

!---

# PC Zoo: Common Preconditioners

!row!

!col! width=50%

+ILU / LU+

- Incomplete or full factorization
- Very robust
- Good for small problems

!col-end!

!col! width=50%

+Block Jacobi / ASM+

- Solve decoupled blocks per subdomain
- Parallel friendly
- Weaker for tight coupling

!col-end!

!row-end!

!row!

!col! width=50%

+Algebraic Multigrid (AMG)+

- HYPRE BoomerAMG, GAMG
- Fast for elliptic problems
- Excellent parallel scalability

!col-end!

!col! width=50%

+Incomplete Cholesky (ICC)+

- For symmetric systems
- Less robust than LU
- Faster factorization

!col-end!

!row-end!

!---

# MOOSE Preconditioning Block: SMP

Single Matrix Preconditioner — auto-created with `full = true` when `solve_type` is `NEWTON` or `LINEAR` and no `[Preconditioning]` block is provided; assembles one coupled Jacobian:

```
[Preconditioning]
  [smp]
    type = SMP
    full = true        # couple all variables
  []
[]
```

- `full = true`: use all variable coupling (robust; more memory)
- `off_diag_row`/`off_diag_column`: specify coupling structure explicitly
- `solve_type` (set in `[Executioner]`, not here): PJFNK or NEWTON; has no default and must be set explicitly

!---

# PETSc Options in MOOSE

Set linear solver details via `petsc_options_iname/value`:

!listing tutorials/darcy_thermo_mech/step07_adaptivity/problems/step7b_fine.i block=Executioner

!---

# Common PETSc Options

- `-pc_type lu`: direct LU (serial: PETSc built-in; parallel: add `-pc_factor_mat_solver_type mumps`)
- `-pc_type hypre -pc_hypre_type boomeramg`: Algebraic multigrid
- `-pc_type asm -sub_pc_type lu`: Additive Schwarz Method (overlapping subdomains) with LU on each subdomain
- `-pc_type icc`: Incomplete Cholesky (symmetric systems)
- `-ksp_gmres_restart 100`: GMRES window (default 30)

!---

# Field-Split (Block) Preconditioning for Coupled Physics

Partition variables by physics, solve with a different PC per field:

!listing modules/contact/test/tests/fieldsplit/frictionless_mortar_FS.i block=Preconditioning

!---

# Field-Split: Structure & When to Use

+Structure:+

- `topsplit`: Coarse partition (e.g., contact vs interior)
- `splitting`: Sub-partitions (e.g., disp_x/disp_y vs Lagrange multipliers)
- `splitting_type`: How to couple (schur, additive, multiplicative)
- `schur_pre`: Preconditioner for Schur complement (S = full matrix)
- Inner `petsc_options`: Solver for each field (e.g., hypre for displacement)

+When to use:+

- Strongly coupled thermo-mechanical or contact problems
- When single-field preconditioner stalls

!---

# Automatic Scaling: Fix Variable Magnitude Mismatch

Different physics can have vastly different scales (temperature: 300 K, stress: 1e8 Pa):

!equation
\text{Condition number} = \frac{\lambda_{\max}}{\lambda_{\min}} \quad \text{(symmetric/SPD; in general } \sigma_{\max}/\sigma_{\min}\text{)} \quad \rightarrow \text{slower Krylov/Newton convergence}

+Simple activation+ — set one flag in the `[Executioner]`:

!listing tutorials/darcy_thermo_mech/step07_adaptivity/problems/step7b_fine.i line=automatic_scaling

`automatic_scaling = true` computes per-variable scaling factors during setup, improving Jacobian conditioning and solver convergence.

!---

# Manual Scaling (Advanced)

Apply explicit scaling per variable in `[Variables]`:

```
[Variables]
  [pressure]
    scaling = 1e-6  # Scale down large values
  []
  [temperature]
    scaling = 1e-2  # Scale down large values
  []
  [disp_x]
    scaling = 1.0   # Already well-scaled
  []
[]
```

Scales residual and Jacobian by row: $\mathbf{J}' = \text{diag}(s) \, \mathbf{J}$ and $\mathbf{R}' = \text{diag}(s) \, \mathbf{R}$

!---

# Troubleshooting Non-Converging Solves

+Step 1: Identify the failure type+

```
[Debug]
  show_var_residual_norms = true
[]
```

Prints per-variable residual: Which field is stuck?

+Step 2: Enable detailed output+

```
[Outputs]
  [exo]
    type = Exodus
    execute_on = 'LINEAR TIMESTEP_END'
    output_material_properties = true
  []
[]
```

!---

# Troubleshooting: The Debug System

+Step 3: Check the Debug system+

```
[Debug]
  show_material_props = true    # List computed properties
  show_execution_order = ALWAYS # Trace object execution
[]
```

!---

# Linear vs Nonlinear Failure

+Linear solve fails:+ (max iterations hit, no convergence)

- Preconditioner too weak → switch PC, increase ASM overlap, use multigrid
- Matrix singular → check BCs, material properties, mesh quality
- Ill-conditioned → enable automatic_scaling or manual scaling

+Nonlinear solve fails:+ (Newton residual not decreasing)

- Bad initial guess → improve `[ICs]`, use continuation (ramp loading)
- Singular Jacobian → check derivatives, coupling, material definitions
- Physics issue → verify equations, test simpler case first

!---

# Solver Settings Checklist for Thermo-Mechanical

```
[Preconditioning]
  [SMP]
    type = SMP
    full = true           # For coupled problems
  []
[]

[Executioner]
  type = Transient
  solve_type = PJFNK      # Or NEWTON if Jacobian is good
  automatic_scaling = true
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre    boomeramg'

  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-8
  l_tol = 1e-5
[]
```

Start here; refine based on iteration counts and convergence behavior.

!---

# Parameter Studies with Stochastic Tools

Automatically run many simulations with different parameter values in parallel to sweep design space or quantify uncertainty.

!---

## The Problem

Manually editing parameter values and restarting for each case is:

- Slow and tedious for design studies
- Error-prone; easy to forget a value or repeat a case
- Hard to scale to hundreds or thousands of samples

+Solution:+ A parent app drives the child app, injecting a different parameter set each run, and collects results automatically.

!---

## Pipeline Overview

!row!

!col! width=100%

!style! fontsize=75%

```
[Samplers] → generate parameter sets
           ↓
[MultiApps] SamplerFullSolveMultiApp
           ↓  (one per sample, parallelized)
[Transfers] SamplerParameterTransfer → push params to child
           ↓
[Controls] SamplerReceiver (in child) → apply params
           ↓
Child model solves with perturbed params
           ↓
[Transfers] SamplerReporterTransfer ← collect QoI back
           ↓
[Reporters] StatisticsReporter (optional) → compute stats
```

!style-end!

!col-end!

!row-end!

!---

## Samplers: Generating Parameter Sets

`[Samplers]` generate the rows of a parameter matrix. Common strategies:

+CartesianProduct+: Grid sweep; `linear_space_items` is a list of `min step num_steps` triplets, one per column

```
type = CartesianProduct
linear_space_items = '0.5 1 5
                      100 100 3'
```

+LatinHypercube+: Quasi-random sampling (efficiency)

```
type = LatinHypercube
num_rows = 5000
distributions = 'gamma q_0 T_0 s'
```

+MonteCarlo+: Random sampling from distributions

Each row is a sample; each column is a parameter value.

!---

## MultiApp: Run Once Per Sample

`[MultiApps]` `SamplerFullSolveMultiApp` runs the sub-app once for each sample row:

!listing modules/stochastic_tools/examples/parameter_study/main.i block=MultiApps

- `sampler`: points to the `[Samplers]` block
- `input_files`: path to the child `.i`
- `mode`: execution mode (batch-restore for efficient restarts)

!---

## Transfers: Parameter Injection

+SamplerParameterTransfer+ (parent → child):

!listing modules/stochastic_tools/examples/parameter_study/main.i block=Transfers/parameters

Pushes each sample row's values into named sub-app parameters (Materials, Kernels, BCs).

!---

## Transfers: Results Collection

+SamplerReporterTransfer+ (child → parent):

!listing modules/stochastic_tools/examples/parameter_study/main.i block=Transfers/results

Gathers QoI (postprocessors, reporters) from each run back into the parent.

!---

## SamplerReceiver: Stochastic Controller

In the +child app+, a `[Controls]` block of type `SamplerReceiver` receives and applies the parameter values:

!listing modules/stochastic_tools/examples/parameter_study/diffusion.i block=Controls/stochastic

No configuration needed — the `SamplerParameterTransfer` on the parent supplies the parameter paths to set each run.

!---

## Example: Child Input

Postprocessors define the quantities of interest; they are transferred back:

!style! fontsize=80%

!listing modules/stochastic_tools/examples/parameter_study/diffusion.i block=Postprocessors

!style-end!

Materials, BCs, and Kernels have parameters that the parent's `SamplerParameterTransfer` will override.

!---

## Reporters: Collecting & Analyzing Results

+StochasticReporter+: Accumulates samples as columns and QoI rows.

+StatisticsReporter+: Computes statistics (e.g. mean, stddev) and confidence intervals across samples:

!style! fontsize=85%

!listing modules/stochastic_tools/examples/parameter_study/main.i block=Reporters/stats

!style-end!

Output to JSON or CSV for post-processing.

!---

## Quick Start: ParameterStudy Action (Optional Shortcut)

The `[ParameterStudy]` action auto-creates the Sampler, MultiApp, Transfers, and Controls for a basic sweep:

!style! fontsize=80%

```
[ParameterStudy]
  input = 'child.i'
  parameters = 'Materials/mat1/prop_values BCs/bc1/value'
  quantities_of_interest = 'T_avg/value'
  sampling_type = cartesian-product
  linear_space_items = '0.5 1 5 100 100 3'
  output_type = csv
[]
```

!style-end!

Skips the verbose Sampler/MultiApp/Transfers wiring for simple cases; for complex studies, write the blocks by hand.

!---

## Parallelization & Reproducibility

- Samples run in parallel (MPI or thread-based) across the available processes.
- Each run is +independent+; rerun sample 42 and get the same result.
- `mode = batch-restore` avoids expensive mesh rebuilds across samples.
- Useful for sensitivity analysis, uncertainty quantification, design optimization, and surrogate training.

!---

# Troubleshooting

Most mistakes in an input file will cause wrong results,
usually affecting convergence of the solve as well. We cover here two common problems:

- Input file mistakes and how to find them

- Non-convergence of the solver

!---

## Input file mistakes

If a careful review of the input does not find the error,
the next thing to pay attention to is the simulation log.

- Are there any warnings? By default MOOSE will not error on warnings
- Are there any unused parameters? They could be misspelled!

If that does work, it is time to examine how the simulation evolves in MOOSE

!---

## Additional outputs

By default, MOOSE outputs on the end of timesteps

```
[Outputs]
  execute_on = TIMESTEP_END
```

We can change this parameter to output as often as linear iterations!
We make sure to output material properties as well, in case the problem lies there:

```bash
[Outputs]
  [exo]  # filename suffix
    type = Exodus
    execute_on = 'LINEAR TIMESTEP_END'
    output_material_properties = true
  []
[]
```

Add any output you need to understand the root cause!

!---

## Summary of helpful resources

[Documentation for every object](syntax/index.md)

[Troubleshooting failed solves](application_usage/failed_solves.md optional=True)

[Debug system](syntax/Debug/index.md)

[FAQ](https://mooseframework.inl.gov/help/faq/index.html)

[GitHub discussions forum](https://github.com/idaholab/moose/discussions) : please follow the [guidelines](https://github.com/idaholab/moose/discussions/18270) before posting

!---

# Questions?

!---



!---

# Appendix

## Restart & Recovery (optional)

!---

# Restart and Recovery System

!---

## Definitions

+Restart+\\
Running a simulation that uses data from a previous simulation, using different input files

+Recover+\\
Resuming an existing simulation after a premature termination

+Solution file+\\
A mesh format containing field data in addition to the mesh (i.e. a normal output file)

+Checkpoint+\\
A snapshot of the simulation including all meshes, solutions, and stateful data

+N to N+\\
In a restart context, this means the number of processors for the previous and current simulations match

+N to M+\\
In a restart context, different numbers of processors may be used for the previous and current simulations

!---

## Variable Initialization

This method is best suited for restarting a simulation when the mesh in the previous simulation
exactly matches the mesh in the current simulation and only initial conditions need to be set for one
or more variables.

- This method requires only a valid solution file
- MOOSE supports N to M restart when using this method

!---

```text
[Mesh]
  # MOOSE supports reading field data from ExodusII, XDA/XDR, and mesh checkpoint files (.e, .xda, .xdr, .cp)
  file = previous.e
  # This method of restart is only supported on replicated meshes
  parallel_type = replicated
[]

[Variables/nodal]
  family = LAGRANGE
  order = FIRST
  initial_from_file_var = nodal
  initial_from_file_timestep = 10
[]

[AuxVariables/elemental]
  family = MONOMIAL
  order = CONSTANT
  initial_from_file_var = elemental
  initial_from_file_timestep = 10
[]
```

!---

## Checkpoints

Advanced restart and recovery in MOOSE require checkpoint files

Checkpoints are automatically enabled by default and are output every 1 hour of wall time (customizable interval), but can be disabled with:
```text
[Outputs]
  wall_time_checkpoint = false
[]
```

Checkpoints can be output at every time step with the following shortcut syntax:

```text
[Outputs]
  checkpoint = true
[]
```

!---

For more control over the checkpoint system, create a sub-block in the input file that will allow you
to change the file format, suffix, frequency of output, the number of checkpoint files to keep, etc.

- Set `num_files` to at least 2 to minimize the chance of ending up with a corrupt restart file

  !listing outputs/checkpoint/checkpoint_interval.i block=Outputs

!---

## Advanced Restart

This method is best suited for situations when the mesh from the previous simulation and the current
simulation match and the variables and stateful data should be loaded from the previous simulation.

- Support for modifying some variables is supported such as `dt` and `time_step`. By default, MOOSE
  will automatically use the last values found in the checkpoint files
- Only N to N restarts are supported using this method

```text
[Mesh]
  # Serial number should match corresponding Executioner parameter
  file = out_cp/0010-mesh.cpr
  # This method of restart is only supported on replicated (non-distributed) meshes
  parallel_type = REPLICATED
[]

[Problem]
  # Note that the suffix is left off in the parameter below.
  restart_file_base = out_cp/LATEST  # You may also use a specific number here
[]
```

!---

## Reloading Data

It is possible to load and project data onto a different mesh from a solution file usually as an
initial condition in a new simulation.

MOOSE supports this through the use of a SolutionUserObject

!---

## Recover

A simulation that has terminated due to a fault can be recovered simply by using the `--recover`
command-line flag, but it +requires a checkpoint file+.

```bash
./frog-opt -i input.i --recover
```

!---

## Multiapp Restart

When running a multiapp simulation you do +not+ need to enable checkpoint output in each sub app
input file. The parent app stores the restart data for all sub apps in its file.
