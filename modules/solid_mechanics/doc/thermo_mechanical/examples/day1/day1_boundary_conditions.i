# ==========================================================================
# Day-1 GOOD example #3 -- "Boundary conditions & functions"
#
# A 2-D metal plate heated and cooled in DIFFERENT ways on each of its four
# named edges. The point of this input is to show the BREADTH of the [BCs]
# system: every edge uses a different boundary-condition object, and one of
# them is driven by a [Functions] ParsedFunction. Watch how the steady-state
# temperature field is shaped by the mix of fixed temperature, applied flux,
# convective cooling, and a ramped/position-dependent boundary value.
#
# Day-1 systems demonstrated:
#   [Mesh] [Variables] [Kernels] [Materials] [BCs] [Functions]
#   [AuxVariables]/[AuxKernels] [Postprocessors] [Executioner] [Outputs]
# ==========================================================================

# --- [Mesh] -----------------------------------------------------------------
# GeneratedMeshGenerator builds a structured rectangle and AUTO-NAMES the four
# sides left/right/top/bottom. We then RenameBoundaryGenerator them to physical
# names so the [BCs] below read like the physics (named mesh sides).
[Mesh]
  [plate]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 40
    ny = 20
    xmax = 0.5 # plate length in x (m)
    ymax = 0.2 # plate height in y (m)
  []
  [name_sides]
    type = RenameBoundaryGenerator
    input = plate
    old_boundary = 'left right top bottom'
    new_boundary = 'hot_wall convective_wall flux_wall ramped_wall'
  []
[]

# --- [Variables] ------------------------------------------------------------
# The single unknown we solve for: temperature (K). Start the whole plate at a
# uniform initial temperature for the transient.
[Variables]
  [temperature]
    initial_condition = 300 # (K)
  []
[]

# --- [Functions] ------------------------------------------------------------
# A ParsedFunction supplies an analytic value at any (x, y, z, t). Here it is a
# boundary temperature that RAMPS up in time and also VARIES with position (x):
# warmer as time advances, and warmer toward the right end of the edge.
[Functions]
  [ramp_temperature]
    type = ParsedFunction
    expression = '300 + 40*t + 120*x' # (K) -- time ramp + spatial gradient
  []
[]

# --- [Kernels] --------------------------------------------------------------
# The PDE terms acting on every element of the domain.
[Kernels]
  # Spatial heat conduction: needs the 'thermal_conductivity' material property.
  [conduction]
    type = HeatConduction
    variable = temperature
  []
  # Transient storage term (rho * c_p * dT/dt): needs specific_heat + density.
  [time_derivative]
    type = HeatConductionTimeDerivative
    variable = temperature
  []
[]

# --- [Materials] ------------------------------------------------------------
# Constant material properties consumed by the kernels and BCs above/below.
[Materials]
  # Bulk solid properties used by the conduction + time kernels.
  [solid]
    type = GenericConstantMaterial
    prop_names = 'thermal_conductivity specific_heat density'
    prop_values = '50 450 7800' # W/m-K, J/kg-K, kg/m^3 (steel-like)
  []
  # Convective-cooling properties consumed by the ConvectiveHeatFluxBC below.
  # ConvectiveHeatFluxBC reads T_infinity and h as MATERIAL properties.
  [coolant]
    type = GenericConstantMaterial
    prop_names = 'coolant_temperature heat_transfer_coefficient'
    prop_values = '290 750' # far-field T (K), h (W/m^2-K)
  []
[]

# --- [BCs] ------------------------------------------------------------------
# The star of this example: four different boundary-condition objects, one per
# named edge, showing the variety the [BCs] system offers.
[BCs]
  # (1) Fixed-temperature wall (Dirichlet): hold this edge at a constant value.
  [hot_side]
    type = DirichletBC
    variable = temperature
    boundary = hot_wall
    value = 600 # held at 600 K
  []

  # (2) Applied surface heat flux (Neumann): prescribe the flux through an edge
  # instead of its temperature. Positive value drives heat INTO the plate.
  [surface_heating]
    type = NeumannBC
    variable = temperature
    boundary = flux_wall
    value = 8000 # (W/m^2)
  []

  # (3) Convective cooling (Robin): flux = h * (T_infinity - T). Models the edge
  # losing heat to a coolant; both h and T_infinity come from materials above.
  [convective_cooling]
    type = ConvectiveHeatFluxBC
    variable = temperature
    boundary = convective_wall
    T_infinity = coolant_temperature
    heat_transfer_coefficient = heat_transfer_coefficient
  []

  # (4) Function-driven fixed temperature: a Dirichlet value supplied by the
  # ParsedFunction, so this edge's temperature ramps in time and varies in x.
  [ramped_side]
    type = FunctionDirichletBC
    variable = temperature
    boundary = ramped_wall
    function = ramp_temperature
  []
[]

# --- [AuxVariables] / [AuxKernels] ------------------------------------------
# Auxiliary field that is COMPUTED (not solved). Here a FunctionAux samples the
# same ParsedFunction onto a field so you can visualize the boundary driver.
[AuxVariables]
  [ramp_field]
  []
[]
[AuxKernels]
  [evaluate_ramp]
    type = FunctionAux
    variable = ramp_field
    function = ramp_temperature
  []
[]

# --- [Postprocessors] -------------------------------------------------------
# Scalar diagnostics reduced from the field each step (written to CSV).
[Postprocessors]
  # Domain-average temperature.
  [avg_temperature]
    type = ElementAverageValue
    variable = temperature
  []
  # Hottest node anywhere in the plate.
  [max_temperature]
    type = NodalExtremeValue
    variable = temperature
    value_type = max
  []
  # Temperature at a specific probe location.
  [probe_center]
    type = PointValue
    variable = temperature
    point = '0.25 0.1 0'
  []
  # Net conductive heat leaving through the convectively cooled edge.
  [flux_out_convective_wall]
    type = SideDiffusiveFluxIntegral
    variable = temperature
    boundary = convective_wall
    diffusivity = thermal_conductivity
  []
[]

# --- [Executioner] ----------------------------------------------------------
# Short transient: a few time steps let the in-time ramp function take effect
# before the field settles toward steady state.
[Executioner]
  type = Transient
  num_steps = 10
  dt = 1.0
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

# --- [Outputs] --------------------------------------------------------------
# Exodus for the field (visualize the BC-shaped temperature map); CSV for the
# postprocessor time histories.
[Outputs]
  exodus = true
  csv = true
[]
