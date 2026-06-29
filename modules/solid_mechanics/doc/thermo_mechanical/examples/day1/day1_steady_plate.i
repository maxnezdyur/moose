# ============================================================================
# DAY 1 - GOOD EXAMPLE #1: "The anatomy of a MOOSE input file"
#
# The simplest COMPLETE heat-conduction problem: steady-state temperature in a
# small 2-D rectangular plate. The left edge is held hot, the right edge cold,
# and heat flows steadily from left to right. The top and bottom edges are left
# alone (insulated - see the [BCs] note below).
#
# Read this file top-to-bottom: every block is one of the Day-1 systems.
#   [Mesh] -> [Variables] -> [Kernels] -> [Materials] -> [BCs]
#   -> [Executioner] -> [Postprocessors] -> [Outputs]
# ============================================================================

# --- [Mesh] -----------------------------------------------------------------
# Defines the geometry / discretization. GeneratedMeshGenerator builds a simple
# rectangular grid for us and AUTO-NAMES the four edges: left, right, top,
# bottom. We refer to those names later in [BCs].
[Mesh]
  [plate]
    type = GeneratedMeshGenerator
    dim = 2          # 2-D problem
    nx = 20          # number of elements in the x-direction
    ny = 10          # number of elements in the y-direction
    xmax = 2.0       # plate width  (m)
    ymax = 1.0       # plate height (m)
  []
[]

# --- [Variables] ------------------------------------------------------------
# The unknown(s) we solve for. Here a single field "temperature" that lives at
# every node. initial_condition seeds the nonlinear solve.
[Variables]
  [temperature]
    initial_condition = 300 # (K) - a reasonable starting guess
  []
[]

# --- [Kernels] --------------------------------------------------------------
# Kernels are the PDE terms acting on the domain interior (the "physics").
# HeatConduction is the steady diffusion term  -div(k * grad T); it needs a
# material property named 'thermal_conductivity' (supplied in [Materials]).
[Kernels]
  [conduction]
    type = HeatConduction
    variable = temperature
  []
[]

# --- [Materials] ------------------------------------------------------------
# Materials provide the coefficients the kernels ask for. GenericConstantMaterial
# declares property values that are constant in space and time. HeatConduction
# looks up 'thermal_conductivity' by name.
[Materials]
  [plate_props]
    type = GenericConstantMaterial
    prop_names = 'thermal_conductivity'
    prop_values = '40' # W/m-K
  []
[]

# --- [BCs] ------------------------------------------------------------------
# Boundary conditions on the mesh edges named by [Mesh].
# DirichletBC fixes the VALUE of the variable on a boundary.
#   left  -> hot edge (500 K)
#   right -> cold edge (300 K)
# NOTE ON TOP/BOTTOM: we deliberately specify NOTHING there. In a finite-element
# heat problem, "no BC" is the NATURAL boundary condition: zero diffusive flux,
# i.e. a perfectly INSULATED edge. So top and bottom are insulated for free.
[BCs]
  [hot_left]
    type = DirichletBC
    variable = temperature
    boundary = left
    value = 500 # (K)
  []
  [cold_right]
    type = DirichletBC
    variable = temperature
    boundary = right
    value = 300 # (K)
  []
[]

# --- [Executioner] ----------------------------------------------------------
# How to solve the assembled system. Steady = solve once to equilibrium.
# solve_type = NEWTON uses the full Jacobian; the petsc options pick an LU
# direct preconditioner (robust and exact for a small problem like this).
[Executioner]
  type = Steady
  solve_type = NEWTON
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
[]

# --- [Postprocessors] -------------------------------------------------------
# Scalar quantities extracted from the solution (reported to screen + CSV).
#   ElementAverageValue       -> volume-averaged temperature over the plate
#   SideDiffusiveFluxIntegral -> total heat flux through the cold (right) edge
#   PointValue                -> temperature sampled at the plate center
[Postprocessors]
  [avg_temperature]
    type = ElementAverageValue
    variable = temperature
  []
  [heat_out_right]
    type = SideDiffusiveFluxIntegral
    variable = temperature
    diffusivity = thermal_conductivity
    boundary = right
  []
  [center_temperature]
    type = PointValue
    variable = temperature
    point = '1.0 0.5 0.0'
  []
[]

# --- [Outputs] --------------------------------------------------------------
# Where results go. exodus = true writes the field for visualization (Paraview);
# csv = true writes the [Postprocessors] table.
[Outputs]
  exodus = true
  csv = true
[]
