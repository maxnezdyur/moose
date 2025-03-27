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

# Moose Introduction


A basic MOOSE input file requires six parts, each of which will be covered in greater detail later.

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

- *Nodal Variables*: Standard finite element variables using Lagrange basis
- *Elemental Variables*: Variables defined at element level using monomial basis
- *Scalar Variables*: Variables that are constant in space but evolve in time
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
    coupled_variables = displacement
  []
[]
```

- Coupling indicates dependencies between physics
- Allows MOOSE to construct proper Jacobian terms
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
(\nabla \phi_j, \nabla \psi_i)\quad \forall\,\psi_i

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
ADReal
ADDiffusion::precomputeQpResidual()
{
  return _grad_u[_qp] * _grad_test[_i][_qp];
}
```

!style-end!

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
from `ADIntegratedBC`.

The structure is very similar to Kernels: objects must override `computeQpResidual`

!---

## ADIntegratedBC Object Members

`_u`, `_grad_u`\\
Value and gradient of the variable this Kernel is operating on

`_test`, `_grad_test`\\
Value ($\psi$) and gradient ($\nabla \psi$) of the test functions at the quadrature points

`_phi`, `_grad_phi`\\
Value ($\phi$) and gradient ($\nabla \phi$) of the trial functions at the quadrature points

`_i`, `_j`, `_qp`\\
Current index for test function, trial functions, and quadrature point, respectively

`_normals`:\\
Outward normal vector for boundary element

`_boundary_id`\\
The boundary ID

`_current_elem`, `_current_side`\\
A pointer to the element and index to the current boundary side

!---

## Non-Integrated BC

Non-integrated BCs set values of the residual directly on a boundary or internal side and
should inherit from `ADNodalBC`.

The structure is very similar to Kernels: objects must override `computeQpResidual`.

!---

## NodalBC Object Members

`_u`\\
Value of the variable this Kernel is operating on

`_qp`\\
Current index, used for interface consistency

`_boundary_id`\\
The boundary ID

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

 If you see this you missed a todo:

<!-- TODO Add input files for Bcs  -->

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
    order = LAGRANGE
    family = FIRST
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

## Convergence Rates

Consider the test problem:

!equation
\begin{array}{rl}
\frac{\partial u}{\partial t} - \nabla^2 u &= f
\\
u(t=0)&= u_0
\\
\left. u \right|_{\partial \Omega} &= u_D
\end{array}

for $t=(0,T]$, and $\Omega=(-1,1)^2$, $f$ is chosen so the exact solution is given by
$u = t^3 (x^2 + y^2)$ and $u_0$ and $u_D$ are the initial and Dirichlet boundary conditions
corresponding to this exact solution.

!---

!media darcy_thermo_mech/time_convergence_implicit.png style=width:90%


!---

#  [Time Stepper System](Executioner/TimeSteppers/index.md)

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

Only time points satisfying $t_{start} < t <t_{end}$ are considered.

If a solve fails at step $n$ an additional time point $t_{new} = \frac{1}{2}(t_{n+1}+t_n)$ is
inserted and the step is resolved.

!---

## Composing TimeSteppers

Time steppers can now be composed to follow complex time histories.
By default, the minimum of all the time steps computed by all the time steppers is used!

What steps will be taken, starting at time = 0s?

```bash
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
  interval = 10 # this is a time step interval
  [exo]
    type = Exodus
    interval = 1 # overrides interval from top-level
  []
  [cp]
    type = Checkpoint # Uses interval specified from top-level
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
     interval = 2
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

`Real getValue()`\\
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
[Output]
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

`virtual VectorPostprocessorValue & getVector (const std::string &vector_name)`
This is called internally within MOOSE to retrieve the final vector value for the given name, the
value returned by this function is referenced by all other objects that are using the vector
postprocessor.

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
[Output]
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


# Contact Methods Overview

!row!

!col! width=50%

+Node-Face Method+

- Point-to-surface discretization
- Traditional contact approach
- Uses penalty or Lagrange multiplier enforcement
- Computationally efficient
- Challenges with non-matching meshes

!col-end!

!col! width=50%

+Mortar Method+

- Surface-to-surface discretization
- Weak enforcement of constraints
- Better for non-matching meshes
- More accurate interface field distribution
- Uses separate space of Lagrange multipliers

!col-end!

!row-end!

!---

# Node-Face Method: General Formulation

!row!

!col! width=48%

+Concept+

- Discrete nodes on secondary surface
- Project onto primary surface elements
- Enforce constraints at node locations
- Simple to implement
- Historically common in FEM

!media node_face_contact.svg
       style=width:90%;

!col-end!

!col! width=48%

+Mathematical Approach+

!equation
\begin{aligned}
g_n &= (\mathbf{x}_{\text{secondary}} - \mathbf{x}_{\text{primary}}) \cdot \mathbf{n} \\
R &= \sum_{i=1}^{n_p} \delta u \cdot \lambda_i
\end{aligned}

where:

- $g_n$ is the normal gap
- $\mathbf{n}$ is the surface normal
- $R$ is the residual contribution
- $\delta u$ is the test function for the field variable
- $\lambda_i$ is the interface quantity at point $i$
- $n_p$ is the number of interface points

!col-end!

!row-end!

!---

# Mortar Method: General Formulation

!row!

!col! width=48%

+Concept+

- Uses weighted integrals over contact surface
- Introduces Lagrange multiplier field
- Enforces constraints in weak sense
- Maintains patch test consistency
- Handles non-matching meshes naturally

!media mortar_contact.png
 style=width:90%;

!col-end!

!col! width=48%

+Mathematical Approach+

!equation
\begin{aligned}
\int_{\Gamma_c} \lambda \cdot c(u_1, u_2) \, d\Gamma &= 0 \\
\int_{\Gamma_c} \delta\lambda \cdot c(u_1, u_2) \, d\Gamma &= 0
\end{aligned}

where:

- $\Gamma_c$ is the interface surface
- $\lambda$ is the Lagrange multiplier field
- $c(u_1, u_2)$ is the constraint function
- $u_1, u_2$ are the primary field variables
- $\delta\lambda$ is the test function for $\lambda$

!col-end!

!row-end!


!---

# Mechanical Contact: Fundamentals

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

# Gap Heat Transfer: Fundamentals

!row!

!col! width=48%

+Basic Principle+

The heat leaving one body must equal that entering another:

!equation
\int_{\Gamma_i} h_{\text{gap}} \Delta T , dA_i = \int_{\Gamma_j} h_{\text{gap}} \Delta T , dA_j

+Total Gap Conductance+

!equation
h_{\text{gap}} = h_{\text{contact}} + h_{\text{gas}} + h_{\text{radiation}}

!col-end!

!col! width=48%

+Heat Transfer Mechanisms:+

- *Contact conductance*

  - Direct solid-solid conduction
  - Depends on: pressure, roughness, hardness

- *Gas conductance*

  - Through gap medium
  - Depends on: gap width, gas properties

- *Radiation*

  - Important at high temperatures
  - Depends on: surface emissivities

!col-end!

!row-end!

!---

# Gap Heat Transfer: Mathematical Model

!row!

!col! width=48%

+Gap Conductance Model+

!equation
h_{\text{gap}} = h_g + h_s + h_r

!col-end!

!col! width=48%

+Components+

- $h_{\text{gap}}$: Total conductance across the gap
- $h_g$: Gas conductance
- $h_s$: Solid-solid (contact) conduction
- $h_r$: Radiative conductance

!col-end!

!row-end!

!---


## Gas Conductance

!equation
h_g = \frac{k_g}{d_g}


where:

- $k_g$ is the thermal conductivity of the gap (gas)
- $d_g$ is the gap distance


!---

## Solid-Solid Contact Conductance

A pressure-dependent model for the solid-solid contact conduction is given by:

!equation
C_T = \alpha \, k_{\text{harm}} \frac{P}{H_{\text{harm}}}


where:

- $\alpha$ is a fitting parameter,
- $P$ is the contact pressure,
- $k_{\text{harm}}$ is the harmonic mean of the thermal conductivities:

  !equation
  k_{\text{harm}} = \frac{2 k_1 k_2}{k_1 + k_2}

- $H_{\text{harm}}$ is the harmonic mean of the material hardnesses.



!---


## Radiative Heat Transfer Conductance


!equation
q_r = \sigma F_e (T_s^4 - T_f^4) \sim h_r (T_s - T_f)

!equation
h_r = \sigma F_e \frac{(T_s^4 - T_f^4)}{(T_s - T_f)} = \sigma F_e (T_s^2 + T_f^2)(T_s + T_f)


!row!

!col! width=50%



+Variables & Emissivity+

- $\sigma$ is the Stefan-Boltzmann constant
- $F_e$ is the emissivity function
- $T_s$ is the surface temperature
- $T_f$ is the far-field temperature
- $h_r$ is the radiative gap conductance

!col-end!

!col! width=50%



Cartesian systems:

!equation
F_e = \frac{1}{\left(\frac{1}{e_s} + \frac{1}{e_f} - 1\right)}


Axisymmetric systems:

!equation
F_e = \frac{e_s e_f r_f}{e_f r_f + e_s r_s (1-e_f)}

!col-end!

!row-end!


!---

## Summary

Combining all components, the overall gap conductance model becomes:

!equation
h_{\text{gap}} = \frac{k_g}{d_g} + C_T + \sigma F_e \left( T_s^2 + T_f^2 \right) \left( T_s + T_f \right)


where:

- $\frac{k_g}{d_g}$ represents gas conduction ($h_g$),
- $C_T$ represents pressure-dependent solid-solid conduction ($h_s$),
- $\sigma F_e \left( T_s^2 + T_f^2 \right) \left( T_s + T_f \right)$ represents radiative conductance ($h_r$).


!---

# Node-Face vs. Mortar for Heat Transfer

!row!

!col! width=48%

+Node-Face Heat Transfer+

- Point evaluation of temperatures
- Heat flux applied at discrete points
- Conservation:

!equation
q_i = h \cdot (T_{\text{secondary}} - T_{\text{primary}}) \cdot A_i

where:

- $q_i$ is heat flux at node $i$
- $A_i$ is the tributary area
- $h$ is the gap conductance
- $T$ are nodal temperatures

!col-end!

!col! width=48%

+Mortar Heat Transfer+

- Integrated heat flux balance
- Weak enforcement through test functions
- Conservation:

!equation
\int_{\Gamma_c} \psi \cdot \lambda \cdot (T_1 - T_2) \, d\Gamma = 0

where:

- $\psi$ is the test function
- $\Gamma_c$ is the contact interface
- $\lambda$ is the Lagrange multiplier field
- $T_1, T_2$ are temperature fields

!col-end!

!row-end!

!---

# Node-Face vs. Mortar for Mechanical Contact

+Complementarity Conditions:+

!equation
g_n \leq 0 \\ \lambda \geq 0 \\ \lambda\, g_n = 0

!row!

!col! width=48%

+Node-Face Mechanical Contact+

- *Mechanism:* Contact forces are applied at discrete nodes.

- *Penalty Contact Force:*
  Given a penalty ($\kappa$) contact traction is computed as

  !equation
  t_n = \kappa\,\max(g_n, 0)

- *Residual Assembly:*
  The contribution to the displacement residual is given by the virtual work of the penalty forces:

  !equation
  R_c = \sum_{i=1}^{n_p} \delta \mathbf{u}_i \cdot (t_n \mathbf{n}_i)



!col-end!

!col! width=48%

+Mortar Mechanical Contact+

- *Mechanism:* Contact pressures are distributed continuously over the contact interface.
- *Constraint Enforcement:*

  !equation
  \int_{\Gamma_c} \psi\, \lambda\, g_n\, d\Gamma = 0

  *This formulation implicitly incorporates the complementarity conditions in a weak sense.*


!col-end!

!row-end!

!---

# Contact in MOOSE: Node-Face

!row!

!col! width=45%

+Node-Face Contact Setup+

!style! fontsize=85%

```

[Contact]
  [mechanical_contact]
    secondary = secondary_boundary
    primary = primary_boundary
    model = frictionless
    formulation = penalty
    penalty = 1e8
    normal_smoothing_distance = 0.1
    normalize_penalty = true
  []
[]

```

!style-end!

!col-end!

!col! width=5%
\\
!col-end!

!col! width=45%

+Key Parameters:+

- primary/secondary: boundary IDs
- `model`: frictionless, coulomb, glued
- `formulation`: penalty or kinematic
- `penalty`: contact stiffness
- `normal_smoothing_distance`: stabilization
- `normalize_penalty`: scales by element size


!col-end!

!row-end!

!---

# Contact in MOOSE: Mortar

!row!

!col! width=50%

+Mortar Contact Setup+

!style! fontsize=85%

```
[Contact]
  [mortar_contact]
    secondary = 10
    primary = 20
    model = coulomb
    formulation = mortar
    c_normal = 1e1
    c_tangential = 1e3
    normalize_c = true
    correct_edge_dropping = true
    use_dual = true
  []
[]
```

!style-end!

!col-end!

!col! width=5%
\\
!col-end!

!col! width=50%

+Key Parameters:+

!style! fontsize=85%

- primary/secondary: boundary IDs
- `model`: frictionless, coulomb, glued
- `formulation`: mortar
- `friction_coefficient`: for coulomb friction
- `c_normal`: convergence parameter for normal contact
- `c_tangential`: convergence parameter for normal tangential contact
- `normalize_c`: Normalize by weighting function norm (recommended to set to true)
- `correct_edge_dropping`: improves contact modeling (recommended to set to true)
- `use_dual`: enables dual mortar approach (recommended to set to true)

!style-end!

!col-end!

!row-end!

!---



# Gap Heat Transfer in MOOSE: Node-Face

<!-- TODO  I do not know the best options here or what people normally use -->

!row!

!col! width=45%

+Node-Face Gap Heat Transfer Setup+

!style! fontsize=85%

```
[ThermalContact]
  [thermal_contact]
    type = GapHeatTransfer
    variable = temp
    primary = primary_boundary
    secondary = secondary_boundary
    gap_conductivity = 1.0
    gap_conductivity_function = gap_cond
    min_gap = 0.001
  []
[]

```

!style-end!

!col-end!

!col! width=5%
\\
!col-end!

!col! width=45%

+Key Parameters:+

- `variable`: temperature variable
- primary/secondary: boundary IDs
- `gap_conductivity`: constant thermal conductance
- `gap_conductivity_function`: function for variable conductance
- `min_gap`: regularization parameter to avoid numerical issues

!col-end!

!row-end!


!---

# Gap Heat Transfer in MOOSE: Mortar

!row!

!col! width=45%

+Mortar-Based Heat Transfer Setup+

!style! fontsize=65%

```
[Constraints]
  [thermal_contact]
    type = ModularGapConductanceConstraint
    variable = lambda
    secondary_variable = temperature
    primary_boundary = primary
    primary_subdomain = primary_block
    secondary_boundary = secondary
    secondary_subdomain = primary_block
    gap_flux_models = 'radiation pressure gas'
    use_displaced_mesh = true
    correct_edge_dropping = true
  []
[]
```

!style-end!


!col-end!

!col! width=5%
\\
!col-end!

!col! width=45%

+Key Parameters:+

- `variable`: Lagrange multiplier field
- primary_variable/secondary_variable: temperature variables for the primary and secondary sides, respectively
- primary_boundary/primary_subdomain: primary side boundary and subdomain identifiers
- secondary_boundary/secondary_subdomain: secondary side boundary and subdomain identifiers
- `gap_flux_models`: specifies the gap flux models (e.g., 'radiation pressure gas')
- `correct_edge_dropping`: improves contact modeling (recommended to set to true)

!col-end!

!row-end!

!---




# Saddle Point Problems in Mortar Contact

!row!

!col! width=50%

+Mathematical Formulation+

- Saddle point problems arise in constrained optimization
- Characterized by indefinite system matrices:
  $\begin{pmatrix} A & B^T \\ B & 0 \end{pmatrix} \begin{pmatrix} u \\ λ \end{pmatrix} = \begin{pmatrix} f \\ g \end{pmatrix}$
- System has both positive and negative eigenvalues
- Stability depends on satisfying the inf-sup condition

!col-end!

!col! width=50%

+Connection to Mortar Contact+

- Mortar method for contact naturally creates saddle points
- In contact problems:

  - $A$ is the stiffness matrix
  - $B$ represents contact constraints
  - $λ$ are Lagrange multipliers
- Challenge: maintaining stability while accurately representing contact interface

!col-end!

!row-end!

!---



# Combined Thermomechanical Problems

- Coupled physics
- Implementation strategies

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

- SuperLU_DIST
- MUMPS

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

*Preconditioner Types:*

- *ASM (Additive Schwarz Method):*

  - Domain decomposition approach
  - Solves local problems on subdomains
  - Configurable overlap between subdomains

- *GAMG (Geometric Algebraic MultiGrid):*

  - Hierarchical approach using coarse and fine grids
  - Rapidly eliminates low-frequency error components
  - Excellent scalability for elliptic problems

- *hypre:*

  - Library of high-performance preconditioners
  - Includes BoomerAMG (algebraic multigrid)
  - Highly effective for elliptic equations
  - Strong parallel scaling properties


!---



# Linear Solver Configuration

*PETSc Configuration:*

```
[Executioner]
# Direct solver
 petsc_options_iname='-pc_type -pc_factor_mat_solver_package'
 petsc_options_value='lu mumps'

# Iterative solver
 petsc_options_iname = '-pc_type -sub_pc_type '
 petsc_options_value = 'asm lu '
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

!row!

!col! width=50%

+Key Components+

- `ReferenceResidualProblem` object
- Reference vector from physically relevant quantities
- Tagging key objects with `extra_vector_tags`

!col-end!

!col! width=0%

+Example Input+

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

!col-end!

!row-end!

!---



# Real-World Applications

- Industrial examples
- Performance optimization

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
more variables.

- This method requires only a valid solution file
- MOOSE supports N to M restart when using this method

!---

```text
[Mesh]
  # MOOSE supports reading field data from ExodusII, XDA/XDR, and mesh checkpoint files (.e, .xda, .xdr, .cp)
  file = previous.e
  # This method of restart is only supported on serial meshes
  distribution = serial
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
simulation match and the variables and stateful data should be loaded from the pervious simulation.

- Support for modifying some variables is supported such as `dt` and `time_step`. By default, MOOSE
  will automatically use the last values found in the checkpoint files
- Only N to N restarts are supported using this method

```text
[Mesh]
  # Serial number should match corresponding Executioner parameter
  file = out_cp/0010-mesh.cpr
  # This method of restart is only supported on serial meshes
  distribution = serial
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

## Using the [Debug system](syntax/Debug/index.md)

To look for an issue during setup, we can list the objects created by MOOSE for numerous systems. For example, for material properties,

```bash
[Debug]
  show_material_props = true
[]
```

For a general log on the entire setup:

```bash
[Debug]
  show_actions = true
[]
```

!---

To look for an issue during the execution,

```bash
[Debug]
  show_execution_order = ALWAYS
[]
```

This will output to the console, the execution of all MOOSE's objects, in their respective nodal/elemental/side loops on the mesh.

!---

## Troubleshooting failed solves

A comprehensive list of techniques is available in the [documentation](application_usage/failed_solves.md)

First, you should diagnose the non-convergence by printing the residuals for all variables:

```bash
[Debug]
  show_var_residual_norms = true
[]
```

You can then identify which variable is not converging.
Equation scaling issues have been covered earlier. Let's explore two other common causes:

- initialization

- bad mesh

!---

Make sure to initialize every nonlinear variable using the `[ICs]` block.
To check initialization, use the [Exodus](Exodus.md) output:

```bash
[Outputs]
  exodus = true
  execute_on = INITIAL
[]
```

!---

Meshing is hard. We have some tools to help in the [MeshGenerator system](syntax/Mesh/index.md) but generally you should:

- visually inspect your mesh. Look for unsupported features: non-conformality (except from libMesh refinement), overlapping cells...
- use the [MeshDiagnosticsGenerator](MeshDiagnosticsGenerator.md) and turn on the relevant checks
- use `show_info = true` in the [FileMeshGenerator](FileMeshGenerator.md) and verify that the output is as expected
- replace your mesh with a simple MOOSE-generated rectangular mesh to check if the mesh is at fault

!---

## Summary of helpful resources

[Documentation for every object](syntax/index.md)

[Troubleshooting failed solves](application_usage/failed_solves.md)

[Debug system](syntax/Debug/index.md)

[FAQ](https://mooseframework.inl.gov/help/faq/index.html)

[GitHub discussions forum](https://github.com/idaholab/moose/discussions) : please follow the [guidelines](https://github.com/idaholab/moose/discussions/18270) before posting

!---

# Questions?

!---

