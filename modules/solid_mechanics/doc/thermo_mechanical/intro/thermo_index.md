# Heat Conduction & Solid Mechanics for Thermomechanical Analysis

## MOOSE Training

!---

# Overview

This training covers fundamentals of solid mechanics and heat conduction using MOOSE, including:

- Solid mechanics principles and formulations
- Heat conduction implementation
- Numerical solution strategies
- Thermal radiation and solver strategies

!---

# Course Outline

+Day 1 — MOOSE Framework Fundamentals+

- Anatomy of an input file; Mesh, Variables, Kernels
- Materials, boundary conditions, AuxVariables
- Executioners, time stepping, Outputs, Postprocessors

+Day 2 — Solid Mechanics & Heat Conduction+

- Solid mechanics: weak form, small vs. finite strain
- Heat conduction and thermal boundary conditions
- Coupled thermo-mechanics: thermal expansion & thermal stress

+Day 3 — Thermal Radiation & Solver Strategies+

- Black/gray-body radiation; surface-to-ambient & enclosure radiation
- Nonlinear & linear solvers, preconditioning, scaling, troubleshooting
- Inverse problems & design optimization
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

!listing face_info_tri.i block=Mesh link=False

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



!listing /displaced/child.i block=Mesh link=False

!style-end!

!col-end!

!col! width=5%

\\

!col-end!

!col! width=45%

Objects can enforce the use of the displaced mesh within the validParams function.

!style! fontsize=60%

!listing modules/solid_mechanics/src/kernels/StressDivergenceRZTensors.C line=use_displaced_mesh link=False

!listing modules/solid_mechanics/test/tests/volumetric_eigenstrain/volumetric_mechanical.i block=Postprocessors/vol link=False

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

# Meet the Unknown: a Field

A +field+ is a quantity with a value at every point in space and every moment in time — that whole map is what we solve for.

- Temperature is a field: aim a thermometer anywhere, at any time, read a number
- We write it $T(x, y, z, t)$ — one value per location, per instant
- Picture a weather map: temperature painted across a whole region, not one reading
- Displacement, pressure, velocity, and concentration are fields too
- We want the +entire field+, a value everywhere — really infinitely many unknowns

!---

# The Symbols You'll See Today

A quick dictionary — every equation today, and in the slides after, is built from just these.

- $\nabla T$ — +gradient+: the direction $T$ climbs fastest; its length is the steepness
- $\nabla\cdot\vec{v}$ — +divergence+: the net rate a vector $\vec{v}$ flows out of a point
- $\int_\Omega (\cdots)\,dV$ — add a quantity up over the whole domain $\Omega$ ($dV$: a tiny piece of volume)
- $\partial\Omega$ — the +boundary+: the outer surface of the domain
- A +boundary condition+ fixes the field, or its flux, on $\partial\Omega$

!---

# Where the Equation Comes From: a Balance

Every governing equation is just +conservation bookkeeping+ on a tiny chunk of material: what is +stored+ must balance what flows in and out.

!equation
\text{stored} = \text{in} - \text{out} + \text{generated}

Track heat energy and write that balance with calculus, and you get the +heat equation+:

!equation
\rho c_p \pf{T}{t} = \nabla\cdot(k\nabla T) + q

- +Stored+: $\rho c_p \pf{T}{t}$ — how fast energy piles up ($\rho$ density, $c_p$ specific heat)
- +Flow+: $\nabla\cdot(k\nabla T)$ — net heat conducted in, minus out ($k$ conductivity)
- +Generated+: $q$ — heat produced inside; the *same* bookkeeping for momentum gives Newton's law for solids

!---

# The Strong Form — and Why It's Hard

The PDE holds inside the body, but alone it does not pin down a unique answer — we must also fix the +edges+.

!equation
\rho c_p \pf{T}{t} = \nabla\cdot(k\nabla T) + q \;\text{ in }\; \Omega, \qquad T = T_b \;\text{ on }\; \partial\Omega

- +Strong form+ = the PDE inside $\Omega$, together with boundary conditions on $\partial\Omega$
- A +boundary condition+ sets a value ($T = T_b$) or a flux (an insulated face lets no heat through)
- "Strong" because it must hold *exactly, at every single point*
- On a real, complicated shape there is *no pencil-and-paper formula* — so we +approximate+

!---

# The Finite-Element Idea: Calculus $\rightarrow$ Algebra

If we cannot solve it everywhere at once, solve it *piece by piece*.

- +Chop+ the domain into many small, simple *elements* — a mesh, like tiling a floor
- On each element, approximate the field with a *simple function* — often a straight line or flat patch
- The unknowns become a *finite list of numbers*: the field's values at the mesh +nodes+
- Calculus collapses into a big *system of algebraic equations* — the linear algebra you already know
- Catch: a straight-line piece has a slope but +no curvature+, yet the PDE asks for a second derivative

!---

# From "True Everywhere" to "True on Average"

Our simple pieces cannot satisfy the strong form *exactly at every point* — so we ask for less.

- Plug the approximation into the PDE and a leftover error remains: the *residual* $R$
- Weaker demand: drive $R$ to zero *on average*, weighted by a +test function+ $\psi$

!equation
\int_\Omega R\,\psi\,dV = 0 \quad \text{for every } \psi

- This +weighted residual+ is the *weak form*: relaxed, yet enough to pin down the field
- It shifts a derivative off the solution onto $\psi$ — so the straight-line pieces finally qualify

MOOSE builds exactly this weak form, and ties each term to an object you set up — next.

!---

# From PDE to Weak Form

MOOSE is a finite-element framework: it builds an approximate solution from *shape functions* multiplied by coefficients — just like the polynomial fit from Day 1.

- The +strong form+ (the PDE) is what we want to solve
- MOOSE actually solves the +weak form+: the PDE multiplied by a *test function* $\psi$ and integrated over the domain
- This lowers the derivative order on the solution and naturally exposes boundary terms

Every term in the weak form maps to a MOOSE object: +Kernel+ (volume integral), +BoundaryCondition+ (surface integral), +Material+ (coefficients).

!---

# How MOOSE Builds the Weak Form (you don't derive this)

You never carry out these steps by hand — but seeing them once shows where Kernels and BoundaryConditions come from:

1. Write the strong form of the PDE
2. Move every term to one side (set equal to zero)
3. Multiply by a test function $\psi$
4. Integrate over the domain $\Omega$
5. Integrate by parts (divergence theorem) to lower the derivative order and expose boundary integrals

The result is the +weighted residual+ $R(u) = 0$ that MOOSE assembles and drives to zero. The *same* recipe produces the heat-conduction and solid-mechanics weak forms used throughout today.

!---

# Integration by Parts & the Divergence Theorem

For a scalar $\varphi$ and vector $\vec{v}$, the product rule and divergence theorem combine to:

!equation
\int_\Omega \varphi\,(\nabla\cdot\vec{v})\,dV = \int_{\partial\Omega} \varphi\,\vec{v}\cdot\hat{n}\,ds - \int_\Omega \vec{v}\cdot\nabla\varphi\,dV

- Moves a derivative off $\vec{v}$ and onto the test function $\varphi$
- The surface term becomes the +natural (Neumann / flux) boundary condition+

This single identity turns a second-order PDE into a first-order weak form.

!---

# Worked Example: Weak Form of Heat Conduction

+(1)+ Strong form — transient heat conduction with a volumetric source:

!equation
\rho c_p \pf{T}{t} - \nabla\cdot(k\nabla T) - q = 0

+(2)+ Multiply by the test function $\psi$ and +(3)+ integrate over $\Omega$:

!equation
\int_\Omega \psi\,\rho c_p \pf{T}{t}\,dV \;-\; \int_\Omega \psi\,\nabla\cdot(k\nabla T)\,dV \;-\; \int_\Omega \psi\,q\,dV = 0

!---

# Weak Form of Heat Conduction (cont.)

+(4)+ Integrate the conduction term by parts (divergence theorem), grouping each term under the MOOSE object that implements it:

!style! fontsize=85%

!equation
\underbrace{\left(\psi,\, \rho c_p \pf{T}{t}\right)}_{\text{HeatConductionTimeDerivative}} +
\underbrace{\left(\nabla\psi,\, k\nabla T\right)}_{\text{HeatConduction}} -
\underbrace{\langle\psi,\, k\nabla T\cdot\hat{n}\rangle}_{\text{flux BC}} -
\underbrace{\left(\psi,\, q\right)}_{\text{HeatSource}} = 0

!style-end!

- $(\cdot,\cdot)$ = volume inner product $\rightarrow$ +Kernel+; $\langle\cdot,\cdot\rangle$ = surface $\rightarrow$ +BoundaryCondition+
- Each term is exactly one object you add to the input file — nothing hidden

!---

# Shape Functions & the Residual Vector

+What you choose+ — in the `[Variables]` block, the shape-function family: +Lagrange+ (nodal) is the default and fits almost everything; Hermite, Monomial, Hierarchic exist for special cases.

+How MOOSE uses it+ (you don't do this by hand): it builds the solution from those basis functions $\phi_j$ and forms one +residual+ per unknown — exactly what the Kernels assemble and the solver drives to zero.

!equation
T \approx T_h = \sum_{j=1}^N T_j\,\phi_j, \qquad R_i = \left(\nabla\phi_i,\, k\nabla T_h\right) - \left(\phi_i,\, q\right) = 0

- +Galerkin+ method: the test functions are the same basis, $\psi = \phi_i$

!---

# From Residual to Numbers: Quadrature

MOOSE evaluates each integral element-by-element using +numerical (Gauss) quadrature+:

!equation
\int_{\Omega_e} f\,dV \approx \sum_{q} w_q\, f(\vec{x}_q)\, |J_q|

- $\vec{x}_q$ are quadrature points, $w_q$ the weights, $|J_q|$ the element Jacobian
- MOOSE performs this sum and the global assembly for every +Kernel+ automatically, using the quadrature for your mesh and element type — you never write it
- This is the bridge from the weak form above to the Kernel objects from Day 1

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

+You don't solve this by hand+ — you give MOOSE the material, the body force $\boldsymbol{b}$, and the boundary conditions, and it finds the displacement field $\boldsymbol{u}$.

!---

# The Isotropic Elasticity Tensor $C_{ijkl}$

!style! fontsize=68%

The +4th-order+ stiffness tensor maps strain to stress: 81 components in 3D, reduced by stress/strain symmetry to a $6\times 6$ matrix (Voigt notation).

For an +isotropic+ material it is fixed by just +two+ constants — the Lamé parameters $\lambda$ and $\mu = G$ (shear modulus):

!equation
\mathbb{C}^{\text{iso}} =
\begin{bmatrix}
\lambda + 2\mu & \lambda & \lambda & 0 & 0 & 0 \\
\lambda & \lambda + 2\mu & \lambda & 0 & 0 & 0 \\
\lambda & \lambda & \lambda + 2\mu & 0 & 0 & 0 \\
0 & 0 & 0 & \mu & 0 & 0 \\
0 & 0 & 0 & 0 & \mu & 0 \\
0 & 0 & 0 & 0 & 0 & \mu
\end{bmatrix}

- `ComputeIsotropicElasticityTensor` accepts +any two+ of $\{E,\nu,\lambda,\mu{=}G,K\}$, converting internally to Lamé.
- `youngs_modulus`$=E$, `poissons_ratio`$=\nu$; $\lambda = \tfrac{E\nu}{(1+\nu)(1-2\nu)},\ \mu = G = \tfrac{E}{2(1+\nu)}$.

!listing modules/solid_mechanics/test/tests/elastic_patch/elastic_patch_quadratic.i block=Materials/elast_tensor

!style-end!

!---

# Generalized Hooke's Law: $\boldsymbol{\sigma} = \mathbb{C} : \boldsymbol{\epsilon}$

Linear elasticity is one constitutive line: stress is the elasticity tensor +double-contracted+ ("$:$") with the *elastic* strain.

!equation
\boldsymbol{\sigma} = \mathbb{C} : \boldsymbol{\epsilon}^{\text{el}}
\qquad\Longleftrightarrow\qquad
\sigma_{ij} = C_{ijkl}\,\epsilon^{\text{el}}_{kl}

- Substituting the isotropic tensor gives the explicit form $\;\sigma_{ij} = \lambda\,\epsilon_{kk}\,\delta_{ij} + 2\mu\,\epsilon_{ij}$.
- The $\lambda\,\epsilon_{kk}$ term resists +volume change+; the $2\mu\,\epsilon_{ij}$ term scales each strain component (+shear / distortion+).
- In MOOSE: `ComputeLinearElasticStress` (small strain) or `ComputeFiniteStrainElasticStress` (finite strain) consume the elasticity tensor and the elastic strain to produce the `stress` property.
- Only the +elastic+ strain enters — thermal and other eigenstrains are subtracted first ($\boldsymbol{\epsilon}^{\text{el}} = \boldsymbol{\epsilon} - \boldsymbol{\epsilon}_0$).

!---

# Splitting Stress: Hydrostatic + Deviatoric

Any symmetric stress tensor splits into a +mean (hydrostatic)+ part and a +deviatoric+ part — a decomposition that underlies every plasticity model.

- +Mean stress+ $\;\sigma_m = \tfrac{1}{3}\,\mathrm{tr}(\boldsymbol{\sigma}) = \tfrac{1}{3}\sigma_{kk}\;$ — the isotropic pressure that drives +volume change+.
- +Deviatoric stress+ $s_{ij}$ — the traceless remainder that drives +shape change (distortion)+:

!equation
s_{ij} = \sigma_{ij} - \sigma_m\,\delta_{ij} = \sigma_{ij} - \tfrac{1}{3}\sigma_{kk}\,\delta_{ij}

- The distortion magnitude is captured by the deviator's +second invariant+ $\;J_2 = \tfrac{1}{2}\,s_{ij}s_{ij} \ \ge 0$.
- Why bother? Metals yield from +distortion+, not from pressure — so the yield criterion is built on $s_{ij}$ and $J_2$, never on $\sigma_m$.

!---

# von Mises Equivalent Stress

Collapse the deviatoric state to a single positive scalar that can be compared against a uniaxial yield strength:

!equation
\sigma_{vm} = \sqrt{3\,J_2} = \sqrt{\tfrac{3}{2}\,s_{ij}s_{ij}}

- Equivalently, in principal stresses $\;\sigma_{vm} = \sqrt{\tfrac{1}{2}\big[(\sigma_1-\sigma_2)^2 + (\sigma_2-\sigma_3)^2 + (\sigma_3-\sigma_1)^2\big]}$.
- It is an +invariant+ (orientation-independent) and ignores hydrostatic pressure — one number summarizing how hard the material is being distorted.
- +von Mises (J2) yield+: the material yields when $\sigma_{vm} \ge \sigma_y$. MOOSE's radial-return plasticity drives the trial $\sigma_{vm}$ back onto the yield surface (the $3G$ shear term you saw in the stress-update relation).
- This is exactly the scalar the deck outputs as `vonmises_stress` (a `RankTwoScalarAux` quantity), requested from the action:

```text
[Physics/SolidMechanics/QuasiStatic]
  [all]
    generate_output = 'vonmises_stress'
  []
[]
```

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
  - There is no loading history to track — each solve starts from the reference state.
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
- In MOOSE: `strain = FINITE` turns on this incremental, large-rotation update — you don't implement it.

!---

# Incremental Deformation Gradient

Large-deformation problems are solved +incrementally+: each step MOOSE measures the *change* in shape and rotation, then adds it to the running total.

- This is the +updated Lagrangian+ approach — it follows large rotations correctly
- +What you set:+ `strain = FINITE` turns it on (the incremental update comes with it)
- You never form these increments yourself — you pick the strain formulation and MOOSE does the bookkeeping

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

- *Turn it on* — one parameter on the QuasiStatic action (it passes down to the strain calculator):

  - `volumetric_locking_correction = true`
  - Use it whenever $\nu \rightarrow 0.5$ (near-incompressible); it is the `vol` curve in the comparison below

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

# Three Pieces Every Solid-Mechanics Problem Needs

Solid mechanics in MOOSE is *plug-n-play*: you assemble a model from small,
swappable material classes plus one kernel. Every problem needs the same trio.

- +Strain calculator+ — a `Material` that turns displacements into strain
  $\boldsymbol{\epsilon}$ (e.g. `ComputeSmallStrain`, `ComputeFiniteStrain`)
- +Stress calculator+ — a `Material` that maps strain to stress
  $\boldsymbol{\sigma}$, using the elasticity tensor $\boldsymbol{\mathcal{C}}$
  (e.g. `ComputeLinearElasticStress`)
- +Stress-divergence kernel+ — the equilibrium PDE, weak form, one component per
  displacement (`StressDivergenceTensors`)

!equation
\mathbb{R} = \left( \boldsymbol{\sigma},\ \nabla \phi \right) - \left< \boldsymbol{t},\ \phi \right> - \left( \boldsymbol{b},\ \phi \right) = \boldsymbol{0}

The kernel supplies $\nabla\phi$; the stress material supplies $\boldsymbol{\sigma}$;
the strain material feeds $\boldsymbol{\sigma}(\boldsymbol{\epsilon})$. Swap any one,
keep the rest.

!---

# The QuasiStatic Action: One Block, Many Objects

!style! fontsize=74%

Wiring those pieces by hand is error-prone. The
`[Physics/SolidMechanics/QuasiStatic]` action does it from one short block:

- +Displacement variables+ `disp_x/y/z` at correct order (`add_variables`)
- A +stress-divergence kernel+ per displacement (active coord system)
- The matching +strain material+ for the chosen formulation
- +Output+ AuxVariables/AuxKernels for stresses/strains, plus the correct `use_displaced_mesh` automatically

!style-end!

!listing modules/solid_mechanics/test/tests/finite_strain_elastic/finite_strain_elastic_new_test.i block=Physics/SolidMechanics/QuasiStatic

!---

# QuasiStatic: Key Parameters

!style! fontsize=70%

Set once at the block level; they default for every sub-block.

- `strain = SMALL | FINITE` — kinematics (next slides)
- `incremental = true` — incremental small-strain formulation
- `add_variables = true` — action creates the displacement variables
- `generate_output = '...'` — auto outputs (`stress_xx`, `vonmises_stress`)
- `eigenstrain_names = '...'` — stress-free strains to subtract (thermal)
- `temperature = T` — couple temperature for eigenstrains & properties

!style-end!

!listing modules/solid_mechanics/test/tests/thermal_expansion/constant_expansion_coeff.i block=Physics/SolidMechanics/QuasiStatic

!---

# Reference vs Deformed Configuration

Equilibrium can be written on the +reference+ (undeformed) mesh or the
+deformed+ (current) mesh. The two must stay consistent with the strain measure.

!equation
\nabla_X \cdot \boldsymbol{\sigma}(X) = \boldsymbol{0} \qquad\text{vs}\qquad \nabla_x \cdot \boldsymbol{\sigma}(x) = \boldsymbol{0}

- Small-strain elasticity: stress and test-function gradients on the reference
  mesh $\Rightarrow$ `use_displaced_mesh = false`
- Large deformation (finite strain, creep, plasticity): everything on the
  deformed mesh $\Rightarrow$ `use_displaced_mesh = true`
- `use_displaced_mesh` picks which mesh the kernels run on: reference for small
  strain, deformed for large deformation
- The QuasiStatic action sets it for you based on `strain` — which is why it is
  the recommended way to build a model

!---

# Choosing a Strain Formulation

Three kinematics, selected by two parameters. Pair each with a matching stress
material; the action keeps the mesh choice consistent.

!style! fontsize=85%

!row!

!col! width=33%

+Total small+

`strain = SMALL`

$\boldsymbol{\epsilon}=\tfrac{1}{2}(\nabla\boldsymbol{u}+\nabla\boldsymbol{u}^{T})$.
Path-independent, no stored history. Reference mesh. Pairs with
`ComputeLinearElasticStress`. +Elastic only.+

!col-end!

!col! width=33%

+Incremental small+

`strain = SMALL` `incremental = true`

Same small measure, but builds $\Delta\boldsymbol{\epsilon}$ each step and stores
old state. Needed for history-dependent (inelastic) models at small strain.

!col-end!

!col! width=33%

+Finite+

`strain = FINITE`

Large strains and rotations: incremental $\Delta\boldsymbol{\epsilon}$ with a
rotation increment, evaluated on the deformed mesh. Pairs with finite-strain /
inelastic stress.

!col-end!

!row-end!

!style-end!

+Critical:+ `strain = SMALL` (without `incremental`) keeps +no history+, so it +cannot+ be used with creep or plasticity. For any inelastic model use +incremental small+ (`incremental = true`) or +finite+ strain — the action then stores `stress_old`/`strain_old` for them.

!---

# Inelasticity Teaser: Splitting the Strain

!style! fontsize=82%

The mechanical strain increment splits into a recoverable +elastic+ part and an +inelastic+ part:

!equation
\Delta \boldsymbol{\epsilon}^{\,\text{mech}} = \Delta \boldsymbol{\epsilon}^{\,\text{el}} + \Delta \boldsymbol{\epsilon}^{\,\text{inel}}, \qquad \boldsymbol{\sigma} = \boldsymbol{\mathcal{C}} : \boldsymbol{\epsilon}^{\,\text{el}}

where $\boldsymbol{\epsilon}^{\,\text{inel}} = \boldsymbol{\epsilon}^{\,\text{plastic}} + \boldsymbol{\epsilon}^{\,\text{creep}} + \dots$

- `ComputeMultipleInelasticStress` subtracts the inelastic part, then computes $\boldsymbol{\sigma}$ from what's left
- It drives one or more +`*StressUpdate`+ models that return the inelastic strain increment (e.g. creep + plasticity)
- Most isotropic plasticity and creep models follow this same strain-splitting recipe

!style-end!

!listing modules/solid_mechanics/test/tests/combined_creep_plasticity/combined_creep_plasticity.i block=Materials/creep_plas

!---

# Inelasticity Teaser: History and Why Creep Matters

!style! fontsize=80%

Inelastic response is +path-dependent+ — the material must remember its own state.

- The model carries +state variables+ from step to step automatically — MOOSE stores the old values for you
- Needs an +incremental+ or +finite+ strain formulation — total small strain keeps no history
- +Creep+ accumulates inelastic strain over time under sustained load and heat — the duty cycle of a hot space reactor

```text
[Materials]
  [creep]                         # a *StressUpdate model
    type = PowerLawCreepStressUpdate
    coefficient = 0.5e-7          # temperature- and stress-driven creep rate
    n_exponent = 5                # stress exponent
    activation_energy = 0
  []
[]
```

Takeaway: swap the stress calculator for `ComputeMultipleInelasticStress` plus a creep model — the structure now relaxes and ratchets like real hot hardware.

!style-end!

!---

# Displacement (Dirichlet) BCs

Dirichlet BCs prescribe the *value* of a displacement component on a sideset — the part of the structure you hold fixed.

!equation
u_i = \bar{u}_i \quad \text{on } \Gamma_D

- `DirichletBC` pins one variable (`disp_x`, `disp_y`, ...) to a constant `value` on a `boundary`.
- `boundary` takes sideset names or IDs; use one block per component you want to hold.
- Below the bottom face is clamped in x; an identical `bottom_y` block sets `disp_y = 0`.
- Need motion that varies in time or space? Use `FunctionDirichletBC` instead.

!listing modules/solid_mechanics/tutorials/introduction/mech_step02.i block=BCs/bottom_x

!---

# Applied Loads: the +Pressure+ Action

A pressure acts along a surface's inward normal — a traction (Neumann) load.

!equation
\mathbf{t} = -\,p\,\mathbf{n}

- `[Pressure]` is an +action+: it auto-creates a `Pressure` BC for *every* displacement variable — one block instead of N.
- `function = 1e7*t` ramps the load from zero — parsed-function shorthand (any `FunctionName` slot accepts an inline expression).
- Smaller load increments per step ease convergence.
- For reuse or non-parsed types, define under `[Functions]` and pass the name.

!listing modules/solid_mechanics/tutorials/introduction/mech_step02.i block=BCs/Pressure

!---

# Pinning Out Rigid-Body Modes

If the body can drift or spin freely, the stiffness matrix is +singular+ — the #1 cause of a mechanics solve that will not converge.

- Loads alone do not fix *position*: free translation + rotation are zero-energy (rigid-body) modes that make the Jacobian singular.
- Pin only what you must: 2D needs 3 constraints removed; 3D needs 6 (apply on 3 non-colinear nodes as 3 + 2 + 1 DOFs).
- Single-node BCs beat clamping a whole face: they kill rigid-body modes without +overconstraining+ (e.g. without blocking thermal expansion along the base).

!---

# Pinning Out Rigid-Body Modes (cont.)

`ExtraNodesetGenerator` builds a nodeset at exact coordinates (a node must actually exist there) — here a single `pin` node at the origin.

!row!

!col! width=50%

!listing modules/solid_mechanics/tutorials/introduction/mech_step03a.i block=Mesh/pin link=False

!col-end!

!col! width=50%

!listing modules/solid_mechanics/tutorials/introduction/mech_step03a.i block=BCs link=False

!col-end!

!row-end!

!---

# Visualizing Tensors: Stress & Strain

Stress/strain are rank-two material tensors — copy into AuxVariables to view in Paraview.

- `RankTwoAux` extracts one component via `index_i`/`index_j` (0,1,2) into a `CONSTANT MONOMIAL` aux var — e.g. $\sigma_{xx}$ is (0,0), $\epsilon_{yy}$ is (1,1).
- `RankTwoScalarAux` computes invariants: `scalar_type = VonMisesStress`, `Hydrostatic`, or `L2norm`.
- Shortcut: `generate_output = 'vonmises_stress stress_xx'` on the QuasiStatic Physics wires up these aux vars.

!equation
\sigma_{vm} = \sqrt{\tfrac{3}{2}\, s_{ij}\, s_{ij}}, \qquad s_{ij} = \sigma_{ij} - \tfrac{1}{3}\,\sigma_{kk}\,\delta_{ij}

!row!

!col! width=50%

!listing modules/combined/test/tests/eigenstrain/inclusion.i block=AuxKernels/matl_s11 link=False

!col-end!

!col! width=50%

!listing modules/solid_mechanics/test/tests/auxkernels/ranktwoscalaraux.i block=AuxKernels/vonmises link=False

!col-end!

!row-end!

!---

# Reducing Fields to CSV

For plotting and regression tests you want numbers, not a whole field — collapse an aux var to one value per timestep.

- A `Postprocessor` reduces a field to a single scalar each step; `ElementAverageValue` averages an AuxVariable over the domain.
- Point it at the `vonmises` (or `s11_aux`) aux var built on the previous slide.
- Add `csv = true` to `[Outputs]` to get one column per postprocessor and one row per timestep — ready for plotting.

!listing modules/solid_mechanics/test/tests/auxkernels/ranktwoscalaraux.i block=Postprocessors/vonmises

```text
[Outputs]
  csv = true
[]
```

!---

# Hands-On: Solid Mechanics Examples

We'll switch to the terminal and work through five runnable inputs in `examples/day2/mechanical/`.

- Build up from a bar in tension to creep and finite-strain bending
- Run any of them: `combined-opt -i <file>.i`, then open the Exodus in ParaView

!---

# Go to: `mech_uniaxial.i`

*Uniaxial bar — does stress = E·strain?* The sanity check for the whole pipeline.

- Roller-supported block pulled along $x$; read back $\sigma_{xx}$ and compare to $E\,\epsilon$
- One linear solve (1 Newton iteration) — uniform $\sigma_{xx}\approx 200$ MPa
- `examples/day2/mechanical/mech_uniaxial.i`

!---

# Go to: `mech_pinning.i`

*Pinning out rigid-body modes* — the most common reason a mechanics solve won't converge.

- A pressure-loaded plate held only by the minimal pins that kill the three rigid-body modes
- +Comment out the pins+ and rerun → the solve goes singular: show them the failure
- `examples/day2/mechanical/mech_pinning.i`

!---

# Go to: `mech_small_vs_finite.i`

*Same input, two strain measures* — when does small-strain break?

- Flip `strain = SMALL` ↔ `FINITE` on one block stretched 25%
- FINITE $\epsilon_{xx}=0.223$ (= $\ln 1.25$) vs SMALL $0.25$ — about 12% apart at large stretch
- `examples/day2/mechanical/mech_small_vs_finite.i`

!---

# Go to: `mech_creep.i`

*Creep — strain that grows under a constant load.*

- `ComputeMultipleInelasticStress` + `PowerLawCreepStressUpdate` at a fixed 1000 K
- Hold the stress constant; watch `creep_strain_yy` accumulate over the time steps
- Stateful and path-dependent — the reason we must take time steps
- `examples/day2/mechanical/mech_creep.i`

!---

# Go to: `mech_large_deformation.i`

*A cantilever that really bends* — geometric nonlinearity.

- Finite strain, several load steps, ~8 Newton iterations each — a genuinely nonlinear solve
- Warp by displacement in ParaView and compare against the small-strain prediction
- `examples/day2/mechanical/mech_large_deformation.i`

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

# The Conduction Term: `HeatConduction`

From S1, the conduction term of the energy equation:

!equation
\underbrace{-\nabla\cdot(k\,\nabla T)}_{\texttt{HeatConduction}} + \dots = 0

In MOOSE this is one +Kernel+. Its weak (residual) form is

!equation
R_i = \left(\nabla\psi_i,\; k\,\nabla T\right)

- `diffusion_coefficient` names the material property for $k$ (default: `thermal_conductivity`).
- +`ADHeatConduction`+ is the *same* physics with an exact, automatically-differentiated Jacobian — prefer it for nonlinear (e.g. temperature-dependent) problems.

!listing modules/heat_transfer/tutorials/introduction/therm_step02.i block=Kernels

!---

# Transient: `HeatConductionTimeDerivative`

For time-dependent problems add the storage term

!equation
\underbrace{\rho\, c_p\,\pf{T}{t}}_{\texttt{HeatConductionTimeDerivative}} + \dots = 0

Weak form:

!equation
R_i = \left(\psi_i,\; \rho\, c_p\,\pf{T}{t}\right)

!---

# Transient: `HeatConductionTimeDerivative` (cont.)

- Reads `density` and `specific_heat` material properties (not assumed constant).
- Sits in `[Kernels]` alongside `HeatConduction`; needs a `Transient` executioner.

!listing modules/heat_transfer/tutorials/introduction/therm_step03.i block=Kernels

!---

# Thermal Properties: `HeatConductionMaterial`

!style! fontsize=56%

Supplies the property names the kernels expect:

- `thermal_conductivity` -> $k$, `specific_heat` -> $c_p$ at each quadrature point.
- +Density lives elsewhere+: set it separately, e.g. `GenericConstantMaterial` with `prop_names = 'density'`.
- Constant: `thermal_conductivity = 45.0`.
- +$k(T)$+: use `thermal_conductivity_temperature_function = <fn>` and couple `temp = T`. The problem becomes nonlinear; MOOSE builds the extra Jacobian terms for you.

!style-end!

!listing modules/heat_transfer/tutorials/introduction/therm_step03.i block=Materials

!---

# Volumetric Heat Source: `HeatSource`

Adds a body source $\dot q$ (W/m$^3$) to the right-hand side — e.g. fission or decay heat in a fuel region:

!equation
\rho\, c_p\,\pf{T}{t} = \nabla\cdot(k\,\nabla T) + \dot q

- `value = <const>` for a uniform source, or `function = <fn>` for a spatially/time-varying source.
- Restrict it to a `block` so only the heated region carries the source.

!listing modules/heat_transfer/tutorials/introduction/therm_step03a.i block=Kernels/heat_source

!---

# Thermal Boundary Conditions: the Taxonomy

Three families cover almost everything:

!equation
\begin{aligned}
&\textbf{Dirichlet:} && T = T_D &&\text{(fixed temperature)}\\
&\textbf{Neumann:} && -k\,\hat n\cdot\nabla T = q_n &&\text{(prescribed flux)}\\
&\textbf{Robin:} && -k\,\hat n\cdot\nabla T = h\,(T - T_\infty) &&\text{(convection)}
\end{aligned}

!style! fontsize=85%
```text
[BCs]
  [fixedT]   type = DirichletBC          # or FunctionDirichletBC
  [flux]     type = NeumannBC            # or FunctionNeumannBC; q_n=0 => insulated
  [convect]  type = ConvectiveHeatFluxBC # Robin / Newton cooling
[]
```
!style-end!

- A *Neumann* with `value = 0` (or simply no BC) is a natural +insulated+ wall.

!---

# Convective BC: `ConvectiveHeatFluxBC`

Newton's law of cooling — couples the surface to a far-field fluid temperature:

!equation
\vec{q}\cdot\hat{n} = h\,(T - T_\infty)

- `heat_transfer_coefficient` -> $h$, `T_infinity` -> $T_\infty$ (both may be material properties or constants).
- Adds to the residual on the named `boundary`; no extra variable is solved.
- Larger $h$ pushes the surface toward $T_\infty$; $h\to 0$ recovers an insulated wall.

!listing modules/heat_transfer/test/tests/convective_heat_flux/equilibrium.i block=BCs/right

!---

# The Action: `[Physics/HeatConduction]/HeatConductionCG`

One block builds the whole CG heat-conduction problem for you:

- conduction via `ADHeatConduction`, transient storage via `ADHeatConductionTimeDerivative`, and an optional heat source.
- BCs from short lists: `fixed_temperature_boundaries`, `heat_flux_boundaries`, `insulated_boundaries`, and convective options.

!---

# The Action: `HeatConductionCG` (cont.)

!style! fontsize=80%
!listing modules/heat_transfer/test/tests/physics/test_cg.i block=Physics
!style-end!

- Great for standard setups; drop back to explicit `[Kernels]`/`[BCs]` when you need full control.

!---

# Practical Note: Consistent Units and K vs degC

MOOSE is +unit-agnostic+ — *you* pick a consistent system and every input must agree.

!style! fontsize=90%
```text
SI:  k [W/(m K)]   c_p [J/(kg K)]   rho [kg/m^3]
     q_dot [W/m^3]  h [W/(m^2 K)]   T [K]
```
!style-end!

- Pure linear conduction only sees $\nabla T$, so a constant offset cancels — but +absolute Kelvin is required+ whenever $T$ itself enters: radiation ($\sigma T^4$), any $k(T)$/$c_p(T)$, or a fixed-temperature value.
- For space-reactor work: +always use K+. Mixing degC and K is the most common silent error.

!---

# Example 1 — Geometry & Problem

A reactor fuel-element-like rod generates heat throughout its volume (fission) and is cooled at its outer surface — the classic conduction setup.

!media media/thermo_mechanical/reactor_conduction_geom.png
       style=width:72%;display:block;margin-left:auto;margin-right:auto;
       alt=Axisymmetric reactor rod, internally heated and cooled at its outer surface

- +Domain:+ axisymmetric (RZ) rod — length 0.30 m, radius 0.05 m, steel ($k=18$ W/m·K)
- +Drive:+ volumetric source $q'''=8$ MW/m³; cooled outer surface at 350 K; ends insulated
- +Solve for:+ $T(r,x,t)$ → a radial gradient, hot centerline (peak $\approx 627$ K)

!---

# Hands-On: Heat Conduction in a Reactor Component

Transient conduction in an axisymmetric (RZ) rod with +internal heat generation+, cooled at its outer surface.

- Governing equation: $\rho c_p \pf{T}{t} = \nabla\cdot(k\nabla T) + \dot q$
- Runnable input blocks from the weak-form slides — `reactor/reactor_conduction.i`

!listing modules/solid_mechanics/doc/thermo_mechanical/examples/reactor/reactor_conduction.i block=Mesh

!---

# The Physics: Kernels

!style! fontsize=80%

Three kernels build the weak form: conduction, transient, source.

!listing modules/solid_mechanics/doc/thermo_mechanical/examples/reactor/reactor_conduction.i block=Kernels

- `ADHeatConduction` $\rightarrow (\nabla\psi,\, k\nabla T)$
- `ADHeatConductionTimeDerivative` $\rightarrow (\psi,\, \rho c_p \dot T)$
- `BodyForce` $\rightarrow$ volumetric source $\dot q$ (fission heating)

!style-end!

!---

# Properties & Boundary Conditions

!style! fontsize=62%

!row!

!col! width=50%

!listing modules/solid_mechanics/doc/thermo_mechanical/examples/reactor/reactor_conduction.i block=Materials link=False

!col-end!

!col! width=50%

!listing modules/solid_mechanics/doc/thermo_mechanical/examples/reactor/reactor_conduction.i block=BCs link=False

!col-end!

!row-end!

!style-end!

- `ADGenericConstantMaterial` supplies $k$, $c_p$, $\rho$
- One Dirichlet (coolant) boundary; every other surface is insulated by the natural BC, $k\nabla T\cdot\hat{n}=0$

!---

# Solve & Run

!listing modules/solid_mechanics/doc/thermo_mechanical/examples/reactor/reactor_conduction.i block=Executioner

Run it:

```bash
combined-opt -i reactor/reactor_conduction.i
```

- `Transient` executioner, `solve_type = NEWTON`, algebraic-multigrid preconditioning
- Newton converges quadratically each step (residual $\sim 10^{-5}\rightarrow 10^{-11}$)

!---

# Result

The rod heats from the volumetric source and settles into the classic radial profile: hot centerline, cool surface.

- Hot centerline, cool outer surface — peak $\approx 627$ K against the 350 K coolant
- Inspect `temperature` (and the `peak_temperature` postprocessor) in the Exodus output (ParaView)
- +Try it:+ change the heat-source `value` or the coolant temperature and rerun

!---

# Hands-On: Heat Conduction Examples

Two more runnable inputs in `examples/day2/thermal/` — we'll open and talk through each.

- Both reinforce the conduction lecture: material interfaces and convective cooling
- Run with: `combined-opt -i <file>.i`

!---

# Go to: `heat_multimaterial.i`

*Conduction across different materials.*

- Three blocks in series, each with its own conductivity $k$
- Heat flux $k\,\nabla T$ is continuous, but the temperature +gradient kinks+ at every interface
- `examples/day2/thermal/heat_multimaterial.i`

!---

# Go to: `heat_convective.i`

*Convective (Robin) cooling to a coolant.*

- `ConvectiveHeatFluxBC`: surface flux $= h\,(T - T_\infty)$, not a fixed wall temperature
- The cooled-surface temperature +floats+ — it is part of the solution, set by the conduction/convection balance
- `examples/day2/thermal/heat_convective.i`

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

In short: MOOSE forms $\boldsymbol{\epsilon}_{\text{elastic}} = \boldsymbol{\epsilon}_{\text{total}} - \boldsymbol{\epsilon}_{\text{thermal}}$ and passes it to the stress calculator — you just name the eigenstrain.

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

- A multi-block body is heated; temperature is imposed by a `FunctionDirichletBC` and conducted through each block by the source-free `HeatConduction` kernel
- Thermal expansion (different $\alpha$ per block) is resisted by the fixed boundaries, producing thermal stress

!equation
\sigma_{\text{th}} = -\alpha E (T - T_{\text{ref}})

!listing modules/combined/test/tests/thermal_strain/thermal_strain.i block=Kernels

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

!listing modules/combined/test/tests/thermal_strain/thermal_strain.i block=Variables

!---

# Coupled Thermo-Mechanics Input (cont.)

Each block can have different eigenstrain objects:

!listing modules/combined/test/tests/thermal_strain/thermal_strain.i block=Physics/SolidMechanics/QuasiStatic

Temperature is passed to the QuasiStatic Physics action via the `temperature` parameter; the action forwards it to the eigenstrain calculators.

!---

# Example 2 — Geometry & Problem

A clamped, heated cylinder: temperature drives thermal expansion, but the clamped ends resist it, producing +thermal stress+. Temperature and displacement solve together.

!media media/thermo_mechanical/reactor_thermomech_geom.png
       style=width:50%;display:block;margin-left:auto;margin-right:auto;
       alt=Clamped, heated axisymmetric cylinder that develops thermal stress

- +Domain:+ axisymmetric (RZ) cylinder — length 0.20 m, radius 0.04 m, steel ($E=200$ GPa)
- +Drive:+ hot end ramped to 550 K, cool end 300 K; both ends clamped axially ($u_z=0$)
- +Solve for:+ $T$ and $\mathbf{u}$ together → von Mises stress (peak $\approx 151$ MPa)

!---

# Capstone: Coupled Thermo-Mechanics

The payoff for Day 2: a +single monolithic solve+ of temperature *and* displacement. A heated, axially-constrained reactor component expands against its supports, producing +thermal stress+.

- Conduction sets $T(\mathbf{x},t)$
- Thermal expansion enters as an +eigenstrain+: $\boldsymbol{\epsilon}_{\text{th}} = \alpha\,(T-T_{\text{ref}})\,\mathbf{I}$
- Constrained expansion $\rightarrow$ stress: $\sigma_{\text{th}} \sim -\alpha E\,\Delta T$
- Input file: `reactor/reactor_thermomech.i`

!---

# Variables & the SolidMechanics Action

!listing modules/solid_mechanics/doc/thermo_mechanical/examples/reactor/reactor_thermomech.i block=Physics/SolidMechanics/QuasiStatic

- `temperature` is a primary variable; the action adds `disp_r`, `disp_z`
- `strain = FINITE`, AD enabled, `eigenstrain_names = eigenstrain` wires in thermal expansion
- `generate_output` produces `vonmises_stress`, `stress_yy`, `strain_yy`

!---

# Coupling Through Materials

!listing modules/solid_mechanics/doc/thermo_mechanical/examples/reactor/reactor_thermomech.i block=Materials

- `ADComputeThermalExpansionEigenstrain` reads `temperature` and produces the eigenstrain — the one-way thermal $\rightarrow$ mechanical link
- `ADComputeIsotropicElasticityTensor` plus `ADComputeFiniteStrainElasticStress` close the mechanics — the same objects from the solid-mechanics section

!---

# Heat Transport & Constraints

!style! fontsize=62%

!row!

!col! width=52%

!listing modules/solid_mechanics/doc/thermo_mechanical/examples/reactor/reactor_thermomech.i block=Kernels link=False

!col-end!

!col! width=48%

!listing modules/solid_mechanics/doc/thermo_mechanical/examples/reactor/reactor_thermomech.i block=BCs link=False

!col-end!

!row-end!

!style-end!

A ramped hot end drives conduction; the component is held axially at both ends — that constraint is what converts free expansion into stress.

!---

# Run & Result

```bash
combined-opt -i reactor/reactor_thermomech.i
```

- Monolithic Newton solve; `automatic_scaling = true` balances the temperature ($\sim 10^2$) and displacement ($\sim 10^{-4}$) magnitudes
- At $t = 200$ s: average $T \approx 344$ K, +peak von Mises $\approx 151$ MPa+
- +Try it:+ release the top axial constraint — the stress collapses (now free expansion)

!---

# Hands-On: Coupled Thermo-Mechanics Examples

Two capstone inputs in `examples/day2/coupled/` — temperature drives stress through the thermal-expansion eigenstrain.

- Run with: `combined-opt -i <file>.i`

!---

# Go to: `coupled_thermal_stress.i`

*Constrained heating $\rightarrow$ thermal stress (the simplest coupling).*

- A clamped block is given a prescribed temperature field; blocked thermal expansion becomes elastic strain
- Peak von Mises $\approx 410$ MPa from heating alone — no mechanical load applied
- `examples/day2/coupled/coupled_thermal_stress.i`

!---

# Go to: `coupled_bimetallic.i`

*Bimetallic strip — the classic thermostat.*

- Two bonded layers with mismatched $\alpha$ ($1.2$ vs $2.3\times10^{-5}$/K); heat it uniformly and it +curls+
- Tip deflects $\approx 3$ cm — differential expansion bends the strip
- `examples/day2/coupled/coupled_bimetallic.i`

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

!listing modules/heat_transfer/test/tests/view_factors/view_factor_2d.i block=UserObjects/vf_study link=False

!col-end!

!col! width=50%

!listing modules/heat_transfer/test/tests/view_factors/view_factor_2d.i block=UserObjects/rt_vf link=False

!col-end!

!row-end!

`ViewFactorRayStudy` sets the quadrature; `RayTracingViewFactor` computes $F_{ij}$ by deterministic ray tracing over an angular quadrature — slower, but handles arbitrary geometry, obstructions, and self-shadowing. Accuracy is controlled by `polar_quad_order`/`azimuthal_quad_order`/`face_order`, not by a random sample size.

!---

# Net-Radiation Method: Radiosity

In a vacuum-filled cavity (think the gap between a reactor vessel and its cooling wall) surfaces exchange heat only by radiation. We track the +radiosity+ $J_i$ — total power leaving surface $i$ per unit area (emitted + reflected).

For opaque, +gray, diffuse+ (Lambert) surfaces the reflectivity is $\rho_i = 1-\epsilon_i$, giving one balance per surface:

!equation
J_i = \epsilon_i\,\sigma\,T_i^4 \;+\; (1-\epsilon_i)\sum_{j} F_{i,j}\,J_j

- $\epsilon_i$ emissivity, $\sigma = 5.67\times10^{-8}\ \mathrm{W/m^2K^4}$, $T_i$ surface temperature.
- $F_{i,j}$ = +view factor+: fraction of $i$'s radiation that lands on $j$ (geometry only).
- The sum $\sum_j F_{i,j} J_j$ is the +irradiation+ $H_i$ arriving on $i$.

!---

# Net-Radiation Method: Net Flux

Solve the coupled linear system for all $J_i$, then recover the net flux that couples back into conduction:

!equation
q_i = J_i - H_i = \frac{\epsilon_i}{1-\epsilon_i}\left(\sigma T_i^4 - J_i\right)

- $q_i>0$ leaves the surface (net loss); $q_i<0$ is net gain.
- This $q_i$ is the Neumann flux applied to the temperature solve on each participating wall.
- The whole method is just two balances per surface — radiosity + irradiation — closed by the view factors $F_{i,j}$.

!---

# Enclosure Boundary Types

Each surface in the enclosure declares +how it closes the radiosity system+. MOOSE supports three roles:

!row!

!col! width=50%

+Fixed-temperature (isothermal)+

- $T_i$ known (constant or a function).
- Use for a wall held at a set point.

Closes with the $T$-form:

!equation
\sum_j\!\left(\delta_{i,j}-(1-\epsilon_i)F_{i,j}\right)J_j = \epsilon_i\sigma T_i^4

!col-end!

!col! width=50%

+Adiabatic / reradiating+

- No net flux: $q_i = 0$.
- Floats to a radiative equilibrium $T$.

Closes with the $q$-form:

!equation
\sum_j\!\left(\delta_{i,j}-F_{i,j}\right)J_j = 0

!col-end!

!row-end!

A third type, +variable-temperature+, ties $T_i$ to the conduction variable — that is the surface where $q_i$ feeds back into the heat equation. Adiabatic and isothermal walls need no temperature variable defined on them.

!---

# One-Block Setup: `[GrayDiffuseRadiation]`

The `[GrayDiffuseRadiation]` action (a `RadiationTransferAction`) builds the whole net-radiation problem from a single block. You list the enclosure boundaries, their emissivities, and tag which are adiabatic or fixed-temperature; everything not tagged is variable-temperature.

!style! fontsize=80%
From one block MOOSE assembles:

- a +view-factor user object+ (ray-tracing by default; `analytical` for unobstructed planar surfaces),
- a +net-radiation side user object+ that solves the radiosity system above,
- a +`GrayLambertNeumannBC`+ on each variable-temperature surface applying $q_i$ to the temperature.
!style-end!

`n_patches` splits a boundary into sub-patches for more accurate view factors.

!---

# Input: `[GrayDiffuseRadiation]` Block

!style! fontsize=80%

Square cavity: boundary `4` at 1200 K, `7` adiabatic, rest at variable T, per-surface emissivity.

!listing modules/heat_transfer/test/tests/radiation_transfer_action/radiative_transfer_action.i block=GrayDiffuseRadiation/cavity

- `boundary` / `emissivity` — paired lists over surfaces.
- `fixed_temperature_boundary` / `fixed_boundary_temperatures` — isothermal wall.
- `adiabatic_boundary` — reradiating wall.
- `temperature` — conduction variable on variable-T surfaces.

!style-end!

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

# Example 3 — Geometry & Problem

A radiator panel rejects reactor waste heat to deep space. With no convection in vacuum, +radiation is the only path out+.

!media media/thermo_mechanical/reactor_radiator_geom.png
       style=width:72%;display:block;margin-left:auto;margin-right:auto;
       alt=Radiator panel heated at its root, radiating to space from both faces

- +Domain:+ 2-D aluminum panel — length 0.60 m, thickness 0.10 m
- +Drive:+ heat-pipe root at 600 K + waste heat $q'''=0.2$ MW/m³; faces radiate ($\epsilon=0.85$, $T_\infty=3$ K)
- +Solve for:+ $T(x,y,t)$ → interior peaks $\approx 617$ K, radiating to a cooler tip

!---

# Worked Example: A Space-Reactor Radiator

Reject waste heat to deep space with no convection — only +radiation+. A panel takes heat at its root and radiates from both faces to a 3 K sink.

- $\rho c_p \pf{T}{t} = \nabla\cdot(k\nabla T) + \dot q$, closed by a radiative surface flux
- Surface flux: $q_r = \epsilon\sigma\,(T^4 - T_\infty^4)$
- Input file: `reactor/reactor_radiator.i`

!---

# The Radiative Boundary Condition

!listing modules/solid_mechanics/doc/thermo_mechanical/examples/reactor/reactor_radiator.i block=BCs

- `FunctionRadiativeBC` applies $\epsilon\sigma\,(T^4 - T_\infty^4)$ on the panel faces
- `emissivity_function = '0.85'`, `Tinfinity = 3` K (deep space)
- That $T^4$ term is the mild nonlinearity motivating the solver material ahead

!---

# Run & Result

!style! fontsize=70%

!listing modules/solid_mechanics/doc/thermo_mechanical/examples/reactor/reactor_radiator.i block=Kernels

```bash
combined-opt -i reactor/reactor_radiator.i
```

- Newton resolves the $T^4$ flux in ~2 iters/step
- Steady: interior peaks $\approx 617$ K, radiating toward a cooler tip
- +Try it:+ drop the emissivity — the panel runs hotter

!style-end!

!---

# Automatic Differentiation for Radiation

AD versions compute the Jacobian automatically:

- `ADFunctionRadiativeBC`
- `ADInfiniteCylinderRadiativeBC`

No need to hand-code derivatives of $T^4$ terms — AD handles them.

!listing modules/heat_transfer/test/tests/radiative_bcs/ad_function_radiative_bc.i block=BCs/bot_right

!---

# Nonlinear Root-Finding

Assembling the weak form over every element leaves us with a system of
*algebraic* equations for the unknown coefficients $u_j$.

- Stack the per-test-function residuals into a vector $\vec{R}(\vec{u})$
- The discrete problem is to find the $\vec{u}$ that drives it to zero:

!equation
\vec{R}_i(\vec{u}) = 0, \qquad i = 1, \ldots, N

- This is +nonlinear+ whenever the conductivity $k$, source $f$, or material
  properties depend on $u$ (true for almost every thermo-mechanics problem)
- There is no formula for the root, so we solve it +iteratively+

!---

# Newton's Method

The workhorse iterative root-finder, with fast convergence near the solution.
Given a current iterate $\vec{u}_n$, solve a *linear* system for the update
$\delta\vec{u}_{n+1}$, then correct:

!equation
\begin{aligned}
\mathbf{J}(\vec{u}_n)\, \delta\vec{u}_{n+1} &= -\vec{R}(\vec{u}_n) \\
\vec{u}_{n+1} &= \vec{u}_n + \delta\vec{u}_{n+1}
\end{aligned}

- Each iteration: build $\vec{R}$, build $\mathbf{J}$, do one linear solve, update
- $\vec{R}$ small $\Rightarrow$ residual is "how wrong are we"; $\mathbf{J}$ tells us which way to step
- +Quadratic convergence+: with a good guess the error roughly *squares* each step,
  $\;\lVert e_{n+1}\rVert \le C\,\lVert e_n\rVert^2$

!---

# Newton's Method, Geometrically

!media media/thermo_mechanical/newton_method.png
       style=width:58%;display:block;margin-left:auto;margin-right:auto;
       alt=Newton iteration following tangent lines to the root of the residual

- The +residual+ $R(u)$ measures how wrong the current guess is; we want $R = 0$
- The +tangent slope is the Jacobian+ $J(u_n)$ — it points toward the root
- Each step follows the tangent to where it crosses zero: solve $J\,\delta u = -R$, then update
- Near the solution the error roughly +squares each step+ — a few iterations and you are there

!---

# Where the Jacobian Comes From

The Jacobian is just the sensitivity of each residual entry to each unknown:

!equation
J_{ij}(\vec{u}) = \pf{\vec{R}_i(\vec{u})}{u_j}

Because the discrete solution is $u_h = \sum_k u_k \phi_k$, differentiating with
respect to a single coefficient collapses the sum to one shape function:

!equation
\pf{u_h}{u_j} = \phi_j, \qquad \pf{(\nabla u_h)}{u_j} = \nabla \phi_j

- So $\mathbf{J}$ entries are the same integrals as the residual, with $u_h \to \phi_j$
- Concretely, the diffusion term contributes $\left(\nabla\psi_i,\; k\,\nabla\phi_j\right)$
- Jacobian entries are +integrals of shape-function products+ — one per coupled DOF pair

!---

# Getting the Jacobian: by Hand or with AD

For "simple" terms $\mathbf{J}$ is tedious; with coupled physics and nonlinear
material properties it becomes very hard to derive by hand. MOOSE gives two routes:

!row!

!col! width=50%

+Hand-coded+

Derive every $\pf{k}{u_j}$ yourself. Fast to run, but error-prone and painful for coupled, nonlinear physics.

!col-end!

!col! width=50%

+Automatic differentiation (AD)+

MOOSE builds $\mathbf{J}$ +exactly+ from the residual, for free. The recommended path — every example this week uses AD.

!col-end!

!row-end!

A correct Jacobian is what gives Newton its fast (quadratic) convergence — so let AD build it.

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

# Setting the Solve Type: NEWTON

Each step, MOOSE drives the nonlinear residual to zero with Newton's method, solving $\mathbf{J}\,\delta\vec{u} = -\vec{R}$. The `solve_type` in `[Executioner]` tells it how — we use +NEWTON+ throughout:

- +NEWTON+: uses the +true Jacobian+ directly, built by +AD+. Best convergence and parallel scaling — the right choice for stiff thermo-mechanics, and AD makes a correct Jacobian almost free.
- +LINEAR+: for a residual that is +linear+ in $u$ — one linear solve, no Newton loop needed.

```moose
[Executioner]
  solve_type = NEWTON
[]
```

!---

# The Krylov Linear Solve

Inside *every* Newton step sits a linear solve, $\mathbf{J}\,\delta\vec{u} = -\vec{R}$. For real meshes $\mathbf{J}$ is huge and sparse, so MOOSE solves it +iteratively+ with a +Krylov method+, building the answer in the growing subspace

!equation
\mathcal{K}_j = \{\mathbf{r},\ \mathbf{J}\mathbf{r},\ \mathbf{J}^2\mathbf{r},\ \dots,\ \mathbf{J}^{j-1}\mathbf{r}\}

- Each iteration costs +one matrix-vector product $\mathbf{J}\mathbf{v}$+ — cheap because $\mathbf{J}$ is large but sparse.
- +GMRES+ (default `ksp`): general, nonsymmetric $\mathbf{J}$ — the usual case.
- +CG+: only for +symmetric positive-definite+ $\mathbf{J}$ (e.g. pure conduction) — cheaper, less memory.

Convergence scales with the +condition number+ of $\mathbf{J}$: an ill-conditioned system needs far too many iterations, so the Krylov solve must be +preconditioned+ (sets up direct vs. iterative, next).

!---

# Why a Line Search?

The full Newton step $\delta\vec{u}$ is the *tangent* prediction — exact for a linear problem, but it can +overshoot+ when the physics is strongly nonlinear:

- A radiative boundary flux $q \propto \varepsilon\sigma\,(T^4 - T_\infty^4)$ has Jacobian $\sim 4\varepsilon\sigma T^3$ — a tiny temperature change swings the residual hard.
- Temperature-dependent $k(T)$ and thermal-stress coupling have the same flavor: the full step lands past the root and the residual +grows+, stalling or diverging the solve.

A line search keeps the Newton +direction+ but scales the +length+ by $\alpha$, picking $\alpha$ that actually decreases the residual:

!equation
\vec{u}_{n+1} = \vec{u}_n + \alpha\,\delta\vec{u}_{n+1},\qquad \alpha \in (0,1]

!---

# Line-Search Types

Set with `line_search` in `[Executioner]` (it maps to PETSc's `-snes_linesearch_type`):

!style! fontsize=85%

- +bt+ — +backtracking+: shrink $\alpha$ from 1 until the residual drops "enough". The robust, general-purpose choice; the usual default.
- +l2+: minimize $\lVert\mathbf{R}\rVert$ along the step via a secant fit.
- +cp+ — critical point: for residuals that are the gradient of an energy.
- +basic+ / +none+: take the full step ($\alpha = 1$), no safeguarding — fastest when Newton already converges cleanly.

!style-end!

Watch what it's doing by adding `-snes_linesearch_monitor` to `petsc_options`.

!---

# Recipe: When Newton Diverges

Symptoms: +nonlinear+ residual stalls/grows, or PETSc prints `DIVERGED_LINE_SEARCH`/`NaN`.

1. +Backtracking+ first: `line_search = 'bt'` — for $T^4$/thermal-stress overshoot.
2. Add `-snes_linesearch_monitor` to confirm $\alpha$ is cut.
3. Still failing? +Shrink dt+, improve the +preconditioner+, or verify the Jacobian (+AD+ + `NEWTON`).
4. Rock-solid and want speed? `line_search = 'none'`.

Executioner using `NEWTON` with backtracking:

!listing test/tests/kernels/vector_fe/electromagnetic_coulomb_gauge.i block=Executioner

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

# Preconditioners: a Few Options for Each

!style! fontsize=90%

An +iterative+ (Krylov) solve only converges fast if the system is +preconditioned+ — transform $\mathbf{A}$ so it is easier to solve ($\mathbf{M}^{-1}\mathbf{A}$, with $\mathbf{M}^{-1}\approx\mathbf{A}^{-1}$ but cheap). Good PC $\rightarrow$ 5--20 linear iterations; poor PC $\rightarrow$ 100+. A +direct+ solve needs none.

!row!

!col! width=50%

+Iterative: pick a `-pc_type`+

- `hypre` (BoomerAMG) — algebraic multigrid; the go-to for diffusion / conduction, scales well
- `gamg` — PETSc's own AMG; good for elasticity
- `asm` — additive Schwarz; robust in parallel, simple
- `ilu` — incomplete LU; cheap, best for small / serial

!col-end!

!col! width=50%

+Direct: the solver *is* the PC+

- `-pc_type lu` — one factorization, no preconditioner
- Pick the package: `-pc_factor_mat_solver_package = mumps` (parallel) or `superlu_dist`
- Bulletproof; best for small / medium or nasty ill-conditioned systems

!col-end!

!row-end!

Rule of thumb: start with `hypre` for conduction-dominated problems; fall back to `lu` when a solve will not converge.

!style-end!

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
- `solve_type` is set in `[Executioner]`, not here — use `NEWTON` (see the solve-type slide)

+SMP vs. FDP:+ `FDP` (`type = FDP`) builds the Jacobian by finite-differencing the residual — accurate but expensive, so it is a debugging/verification tool; `SMP` is the production choice for coupled thermo-mechanics.

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

# Convergence Tolerances

Newton nests two loops: an +outer nonlinear+ (Newton) loop, and inside each Newton step an +inner linear+ (Krylov) solve of $\mathbf{J}\,\delta\vec{u}=-\vec{R}$. Each loop has its own stopping rules, all set in `[Executioner]`.

The nonlinear loop stops when the residual norm satisfies:

!equation
\|\vec{R}_n\| < \max\!\big(\,r_{\text{tol}}\cdot\|\vec{R}_0\|,\;\; a_{\text{tol}}\big)

with $r_{\text{tol}} =$ `nl_rel_tol`, $a_{\text{tol}} =$ `nl_abs_tol`, and $\|\vec{R}_0\|$ the residual at the start of the step.

| Parameter | Controls | Default |
| :- | :- | :- |
| `nl_rel_tol` | Nonlinear: relative drop of $\|\vec{R}\|$ from step start | `1e-8` |
| `nl_abs_tol` | Nonlinear: absolute floor on $\|\vec{R}\|$ | `1e-50` |
| `nl_max_its` | Max Newton iterations before the step fails | `50` |
| `l_tol` | Linear: relative tolerance for each Krylov solve | `1e-5` |
| `l_max_its` | Max Krylov iterations per Newton step | `10000` |

- Default `nl_abs_tol = 1e-50` is effectively off, so the +relative+ tolerance drives convergence; tighten `nl_abs_tol` when the residual scale is tiny (e.g. near steady state).
- The linear solve only has to be "good enough" to take one Newton step, so `l_tol = 1e-5` is loose on purpose.

!---

# Reading SNES / KSP Output

MOOSE prints the nonlinear and linear residual norms every iteration. Add these PETSc flags (e.g. `petsc_options = '-snes_converged_reason'` in `[Executioner]`) for more detail:

- `-snes_monitor` / `-ksp_monitor` -- print the nonlinear (+SNES+) and linear (+KSP+) residual norm at each iteration.
- `-snes_converged_reason` / `-ksp_converged_reason` -- print +why+ the solve stopped, e.g. `CONVERGED_FNORM_RELATIVE` or `DIVERGED_MAX_IT`.

```text
 0 Nonlinear |R| = 1.000e+02
      0 Linear |R| = 1.000e+02
      4 Linear |R| = 7.3e-04        <- inner Krylov loop drives |R| down to l_tol
 1 Nonlinear |R| = 3.8e-01
 2 Nonlinear |R| = 9.0e-09
 Nonlinear solve converged due to CONVERGED_FNORM_RELATIVE iterations 2
```

- +Nonlinear stagnation+: the left-justified `Nonlinear |R|` numbers stall (drop little each Newton step) or you hit `nl_max_its` -- usually a wrong or poor Jacobian.
- +Linear stagnation+: within a single step the indented `Linear |R|` numbers barely fall or hit `l_max_its` -- usually a poor preconditioner or ill-conditioned matrix.

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

# Predictors: Seed Newton With a Better Guess

By default each time step starts Newton from the *old* solution. For a fast-ramping
load that guess is stale, so Newton burns extra iterations climbing back to the answer.

A +Predictor+ extrapolates from previous solution iterates to seed the next step
*closer* to the converged state — fewer Newton iterations per step, more robust solves.

!---

# Predictors (cont.)

!style! fontsize=78%

- Lives under `[Executioner][Predictor]`.
- +`SimplePredictor`+: linear extrapolation along the last solution change, scaled by `scale` (0 = off, 1.0 = full step).
- +`AdamsPredictor`+: quadratic, uses three back solutions; auto-added by the `AB2PredictorCorrector` integrator.

!listing test/tests/predictors/simple/predictor_test.i block=Executioner

!style-end!

!---

# IterationAdaptiveDT: Grow When Easy, Recover When Hard

!style! fontsize=72%

Same stepper as Day 1, now a +robustness tool+. It sizes `dt` from how many
iterations the *last converged* step needed, targeting `optimal_iterations`.

- Easy solve (iters below the window): next `dt` $\times$ `growth_factor`.
- Hard but converged (iters above the window): next `dt` $\times$ `cutback_factor`.
- `iteration_window` (default `optimal_iterations`/5) is the dead-band; `linear_iteration_ratio` folds linear iters into the count.

!listing test/tests/time_steppers/iteration_adaptive/adapt_tstep_shrink_init_dt.i block=Executioner

!style-end!

!---

# IterationAdaptiveDT: Grow When Easy, Recover When Hard (cont.)

+Recovery on a failed solve+ (Newton diverges or hits `nl_max_its`): MOOSE discards the
step, shrinks `dt` $\times$ `cutback_factor_at_failure` (a base-`TimeStepper` knob), and
*retries the same time* — automatic, no restart.

- `dtmin` is the floor: if a cutback would drop below it, the run aborts instead of looping forever. `dtmax` caps growth.

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
  solve_type = NEWTON     # true Jacobian (AD builds it for you)
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

# Two Ways to Couple: One Jacobian vs. Separate Apps

Recall the field-split slides — two ways to make two physics talk:

!row!

!col! width=50%

+Monolithic+

- All physics in +one+ Newton system, one mesh, one Jacobian
- +Wins when+: very strong coupling, you want the exact Jacobian and tight tolerances in a single solve

!col-end!

!col! width=50%

+Loose (MultiApp)+

- Each physics is its +own app+ — own mesh, own solver, own `.i`
- Apps +exchange fields+ and iterate
- +Wins when+: different meshes/codes/timescales, modularity, and +reuse of a validated input unchanged+ (e.g. a CFD/Navier-Stokes input)

!col-end!

!row-end!

Anchor: solid heat transfer (built all week) $\leftrightarrow$ Navier-Stokes coolant $=$ +conjugate heat transfer (CHT)+. You do not rewrite either app — you +wire them together+.

!---

# The MultiApp Tree: Parent + Sub-Apps

!style! fontsize=76%

A +MultiApp+ lets a +parent+ solve launch one or more +sub-app+ solves. Each sub-app is a +complete standalone input+ — its own mesh and solver — that does not know it is coupled and is +runnable alone+.

!listing modules/solid_mechanics/doc/thermo_mechanical/examples/coupling/cht_solid_parent.i block=MultiApps

Key `[MultiApps]` fields:

- `type` — `TransientMultiApp` marches in time +with+ the parent; `FullSolveMultiApp` solves to completion on each call
- `input_files` — the sub-app `.i`
- `positions` — where each sub-app sits in space
- `execute_on` — +when+ the parent runs the sub

Forward pointer: +Optimization+ and +Stochastic Tools+ (next) are this same machinery, specialized.

!style-end!

!---

# Transfers: Moving Fields Between Apps

Transfers write into +AuxVariables+ (never solved variables). The receiving app declares an `[AuxVariables]` placeholder with an `initial_condition`.

Direction is +explicit+: `to_multi_app` = parent $\rightarrow$ sub; `from_multi_app` = sub $\rightarrow$ parent. Two-way coupling = one of each. (The two-way `[Transfers]` block is on the worked-example slide.)

Choose the transfer by +geometry+: `GeneralFieldShapeEvaluation` (interpolate), `GeneralFieldNearestLocation` (robust/extrapolation), `Copy` (identical meshes); +Reporter+ transfers move scalars.

+Pitfalls+: match `execute_on` between a MultiApp and its Transfers; get the direction right; declare the receiving AuxVariable.

!---

# Closing the Loop: Fixed-Point (Picard) Iteration

Default loose coupling exchanges fields +once per step+ (`fixed_point_max_its = 1`). Strong two-way coupling (CHT) needs +iteration+: re-exchange and re-solve until the interface stops changing.

```text
[Executioner]
  type = Steady
  fixed_point_max_its = 10     # iterate the coupling
  fixed_point_rel_tol = 1e-7
  # relaxation_factor = 0.5    # damp if the interface oscillates
[]
```

The knobs: `fixed_point_max_its` (iteration budget), `fixed_point_rel_tol` / `fixed_point_abs_tol` (when the loop is converged), and `relaxation_factor` (optional damping for stiff coupling).

Set fixed-point +only on the parent+; the sub just runs its normal solver. If the sub needs a smaller `dt`, use `sub_cycling` so it takes several steps per parent step.

!---

# Worked Example — CHT: Heated Solid $\leftrightarrow$ Coolant

A heated solid wall is cooled by an adjacent fluid channel across a shared `interface`. +The trick+: both sides apply `CoupledConvectiveHeatFluxBC` whose far-field `T_infinity` is the +other+ app's interface temperature, delivered as an AuxVariable — only +temperature+ crosses (robust Robin-Robin, no flux post-processing).

Roles: +PARENT+ = the solid heat-transfer app from Days 1-2; +SUB+ = the fluid, an explicit stand-in for a real Navier-Stokes/CFD solve.

+Swap the fluid sub for a real NS input and the parent's `[MultiApps]`/`[Transfers]` blocks do not change.+ A real/monolithic CHT lives at `modules/navier_stokes/test/tests/finite_volume/ins/cht/conjugate_heat_transfer/cht_rob-rob.i`.

!---

# CHT: How the Two Apps Are Wired

Two Transfers carry the interface temperature each way; the sub turns it into a Robin BC (the parent's `[MultiApps]` block is on the MultiApp-tree slide).

!style! fontsize=66%

!row!

!col! width=50%

+Parent: two-way Transfers+

!listing modules/solid_mechanics/doc/thermo_mechanical/examples/coupling/cht_solid_parent.i block=Transfers link=False

!col-end!

!col! width=50%

+Sub: receive T, apply the Robin BC+

!listing modules/solid_mechanics/doc/thermo_mechanical/examples/coupling/cht_fluid_sub.i block=BCs link=False

!col-end!

!row-end!

!style-end!

!---

# Worked Example — Run & What to Watch

```bash
combined-opt -i coupling/cht_solid_parent.i
```

+What to watch+:

- The +fixed-point (Picard) relative residual+ drops monotonically across iterations: $2.11\times10^{2} \rightarrow 1.97\times10^{1} \rightarrow \dots \rightarrow 3.0\times10^{-6}$ — `CONVERGED_RELATIVE` in 9 iterations
- The two interface-average postprocessors settle to a +stable coupled pair+: `T_solid_interface_avg` $= 576.36$ K and `T_fluid_interface_avg` $= 528.64$ K — separated only by the $\approx 48$ K finite-`htc` contact-resistance jump (raise `htc` to shrink it)
- +Two exodus outputs+ (parent solid, sub fluid) — open both to see heat flow from the hot solid into the cooler fluid

+Try it+:

1. Set `fixed_point_max_its = 1` (pure loose) and watch the interface mismatch appear
2. Raise `q_source` or lower `htc`
3. +Capstone+: replace `cht_fluid_sub.i` with a real Navier-Stokes input and re-run — the parent is +untouched+

!---

# Optimization: Calibrate & Design

The optimization module wraps a *minimization* around the same FEM solve you already run:

!equation
\min_{p}\; f(\mathbf{u}, p) \quad \text{subject to}\quad \mathbf{R}(\mathbf{u}, p) = 0

- $\mathbf{R}=0$ is the residual you've solved all week; $p$ are design parameters (a source, a property, a load), $\mathbf{u}$ is $T$ or displacement
- +Two jobs:+ *inverse problems / calibration* (tune $p$ so the model matches measurements) and *design* (tune $p$ to make an objective best)
- It is the mirror of what comes next: optimization +converges to the single best $p$+, while stochastic tools +explore the whole space+

!---

# The Optimization Loop

A driver (PETSc/TAO) proposes parameters, the forward model scores them, and a gradient says which way to step:

!media media/thermo_mechanical/opt_loop.png
       style=width:86%;display:block;margin-left:auto;margin-right:auto;
       alt=Optimization loop: pick parameters, forward solve, objective, adjoint gradient, repeat

- The +objective+ is usually a least-squares data misfit, $f=\tfrac12\sum_i (T_i-\tilde{T}_i)^2$, optionally plus a regularization term
- Stop when $f$ is small (the model matches the data) or the gradient vanishes

!---

# Cheap Gradients: the Adjoint

Gradient-based optimization needs $\nabla_p f$. Three ways to get it:

- +Gradient-free+ (e.g. Nelder-Mead): no derivatives, but cost explodes with the number of parameters
- +Finite difference+: one extra forward solve *per parameter* — fine for testing, too slow at scale
- +Adjoint+ (the win): +one+ extra solve gives the *whole* gradient, at a cost +independent of the number of parameters+

The adjoint reuses the forward Jacobian (transposed) driven by the data misfit — and MOOSE can build it automatically.

!---

# Architecture & Key Objects

An (almost empty) main-app runs the optimizer; the real physics runs as +sub-apps+ — the same MultiApp machinery from Day 1.

!style! fontsize=82%

- +`Optimize` executioner+ — drives TAO, the optimization algorithm
- +`OptimizationReporter`+ — holds the parameters, objective, bounds, and gradient
- +forward (+ adjoint) sub-app+ — a `FullSolveMultiApp` running the model; `SteadyAndAdjoint` / `TransientAndAdjoint` solve both systems at once
- +`OptimizationData`+ — samples the model at sensor points and forms the misfit
- +`ReporterPointSource`+ — injects the misfit as the adjoint load; reporter transfers shuttle data between the apps

!style-end!

!---

# Worked Example — Geometry & Problem

Flip Example 1 around: you measured temperatures, now +recover the unknown fission heat source+ that produced them.

!media media/thermo_mechanical/opt_inverse_geom.png
       style=width:74%;display:block;margin-left:auto;margin-right:auto;
       alt=Fuel rod with four thermocouples and an unknown heat source to recover

- +Known:+ four thermocouple temperatures inside the rod, the conductivity, and the cooled-surface BC
- +Unknown:+ the volumetric source magnitude $q'''$
- +Goal:+ find the $q'''$ whose simulated temperatures best match the four readings

!---

# Worked Example — The Inverse Setup

Two inputs; you drive the main one and it spawns the forward+adjoint sub-app.

!style! fontsize=68%

!row!

!col! width=48%

+Main — the optimizer+

!listing modules/solid_mechanics/doc/thermo_mechanical/examples/reactor/reactor_inverse_source.i block=OptimizationReporter link=False

!col-end!

!col! width=4%

$\quad$

!col-end!

!col! width=48%

+Sub-app — forward + adjoint+

!listing modules/solid_mechanics/doc/thermo_mechanical/examples/reactor/reactor_inverse_forward_and_adjoint.i block=Executioner link=False

!col-end!

!row-end!

!style-end!

- `GeneralOptimization` declares one parameter `q`; `SteadyAndAdjoint` solves the forward and adjoint together
- The source is a `ParsedOptimizationFunction` (`expression = 'q'`); `ElementOptimizationSourceFunctionInnerProduct` returns the gradient

!---

# Worked Example — Run & Result

```bash
combined-opt -i reactor/reactor_inverse_source.i
```

- Start from a guess $q''' = 1\times10^{6}$; the data misfit is large
- +Two TAO iterations later:+ recovered +$q''' = 8.0\times10^{6}$+ — the true value — with the objective driven to $\sim10^{-23}$
- A linear inverse problem like this snaps straight to the answer; nonlinear ones take more steps, but the loop is identical
- +Try it:+ perturb a thermocouple reading and watch the recovered $q'''$ shift

!---

# Inversion Types → Exploring the Space

Same machinery, different targets:

- +Force inversion+ (our example: a source or load) — the parameter sits in the load; often linear and fast
- +Material inversion+ (conductivity, convective $h$) — the parameter sits in the Jacobian; nonlinear and harder
- +Design optimization+ — minimize mass or cost subject to a temperature or stress limit

+Optimization+ pins down the single best, calibrated parameter set by following gradients downhill. +Stochastic tools+ do the opposite job: instead of converging to one point, they +sample across the whole space+ to quantify uncertainty and rank sensitivity — exploration, not convergence.

!---

# Parameter Studies with Stochastic Tools

Automatically run many simulations with different parameter values in parallel to sweep design space or quantify uncertainty.

!---

# Stochastic Tools: Explore the Parameter Space

Where optimization converged to one best answer, stochastic tools ask the opposite question: across the *range* of plausible inputs, what is the *spread* of outcomes?

!media media/thermo_mechanical/stochastic_pipeline.png
       style=width:74%;display:block;margin-left:auto;margin-right:auto;
       alt=Stochastic pipeline: distributions, sampler, many parallel forward runs, statistics

- +Distributions+ describe the uncertain inputs (a source, a conductivity, a coolant temperature)
- A +Sampler+ draws $N$ parameter sets; a +MultiApp+ runs the model once per set, in parallel
- +Reporters+ collect the outputs into statistics — means, spreads, and sensitivity (which input matters most)

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

# Where Uncertainty Enters: the `[Distributions]` Block

Samplers "draw" parameter values from a +distribution+ you declare in `[Distributions]`.

- A distribution describes the spread of one uncertain input (PDF/CDF/quantile).
- Samplers pull rows from one or more distributions; each row is one sub-app run.
- Common `stochastic_tools` types: +Uniform+, +Normal+, +Lognormal+, +Weibull+, +Gamma+, +TruncatedNormal+.

!listing modules/stochastic_tools/test/tests/distributions/uniform.i block=Distributions

Pick `Uniform` for a known min/max, `Normal` for a mean plus measured scatter. One distribution per input.

!---

# Sensitivity: Which Inputs Actually Matter?

Before a full uncertainty propagation, ask *which* parameters drive the output. Two samplers answer two different questions.

!row!

!col! width=50%

+Sobol+ (`type = Sobol`)

- *How much of the output variance does each input explain?*
- Gives first-order + total Sobol indices, including interactions.
- Accurate but sample-hungry (pairs two Monte Carlo sets).

!col-end!

!col! width=50%

+Morris+ (`type = MorrisSampler`)

- *Cheap screening: which inputs are negligible vs influential/nonlinear?*
- Elementary-effects "one-at-a-time" trajectories.
- A good first pass when you have many parameters.

!col-end!

!row-end!

!equation
S_i = \frac{\mathrm{Var}\big(\mathbb{E}[Y \mid X_i]\big)}{\mathrm{Var}(Y)}

The Sobol index $S_i$ is the fraction of output variance attributable to input $X_i$. Screen with Morris first, then quantify the survivors with Sobol.

!---

# Surrogates: Train Once, Evaluate Cheaply

UQ can need thousands of evaluations. A +surrogate+ is a cheap stand-in fit to a modest set of real runs, then queried millions of times.

- A `Trainer` runs the full model on sampler rows and produces fit data.
- A model in the `[Surrogates]` block replaces the FE solve for evaluation.
- +PolynomialChaos+: a polynomial fit; mean, variance, and Sobol indices come out analytically.
- +NearestPointSurrogate+: returns the training point closest to the query — a fast lookup.

!listing modules/stochastic_tools/test/tests/surrogates/poly_chaos/main_2d_mc.i block=Surrogates

Pointer, not a deep dive: reach for these when a full Monte Carlo sweep of your thermo-mechanics model is too expensive. Start from the `stochastic_tools` examples.

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
