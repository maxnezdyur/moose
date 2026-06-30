# ==============================================================================
# CHT coupling example - THE SOLID PARENT APP (owns the coupling)
# ------------------------------------------------------------------------------
# This is the PARENT app in a conjugate-heat-transfer (CHT) MultiApp coupling. It
# models a HEATED SOLID WALL (heat conduction + a volumetric heat source) that
# sits next to a coolant channel. The fluid channel is a SEPARATE app
# (cht_fluid_sub.i, a deliberately simple heat-conduction stand-in for a real
# CFD/Navier-Stokes solve) launched as a sub-app from here.
#
# WHAT THIS FILE TEACHES (the point of the example):
#   * [MultiApps]  - one FullSolveMultiApp that runs the fluid sub-app.
#   * [Transfers]  - TWO field transfers across the shared interface:
#                      to_multi_app   : send THIS solid's interface temp -> sub
#                      from_multi_app : pull the FLUID's interface temp  <- sub
#   * fixed-point (Picard) iteration ON THE PARENT - the [Executioner] repeats
#     "solve solid -> push temp -> solve fluid -> pull temp" until the interface
#     temperature stops changing. That convergence IS the conjugate-heat-transfer
#     coupling: a single continuous interface temperature seen by both sides.
#
# COUPLING SCHEME (Robin-Robin / Dirichlet-free):
#   Each side applies a convective (Robin) BC at the interface in which the
#   far-field temperature is the OTHER side's interface temperature:
#     solid BC:  q = htc * (T_solid_surface - T_fluid_if)     (T_fluid_if from sub)
#     fluid BC:  q = htc * (T_fluid_surface - T_solid_if)     (T_solid_if from parent)
#   With matched htc and a coincident interface, the two surface temperatures
#   drive toward equality as the Picard loop converges -> continuous T.
#
# Geometry: heated solid wall occupies x in [0, 1], y in [0, 1]. Its RIGHT face
# (x = 1) is the shared 'interface' with the fluid (fluid lives in x in [1, 2]),
# so the two interface faces are COINCIDENT and the field transfers map
# point-to-point (both apps at positions '0 0 0', same coordinate system). The
# LEFT face (x = 0) is the insulated outer wall.
# ==============================================================================

# Coupling / physics constants (kept at top, day2 style). htc MUST match the sub.
# (See the htc/convergence note in cht_fluid_sub.i: htc is tuned so the Picard
# loop converges within 10 iterations.)
k_solid = 20.0 # solid wall thermal conductivity                 [W/(m.K)]
q_source = 200.0 # volumetric heat source in the wall              [W/m^3]
htc = 12.0 # interface heat-transfer coefficient (gap)       [W/(m^2.K)]
T_solid_outer = 600.0 # fixed (cooled) outer-wall temperature at x = 0     [K]
T_fluid_if_init = 520.0 # initial guess for the fluid interface temperature [K]

[Mesh]
  # The heated solid wall block, x in [0,1]. Its right face (x = 1) is the
  # coincident interface with the fluid sub-app.
  [wall]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 20
    ny = 20
    xmin = 0.0
    xmax = 1.0
    ymin = 0.0
    ymax = 1.0
  []
  # Name the right face (x = 1) 'interface' - the CHT coupling sideset. It must
  # use the SAME name as the sub-app's interface so the transfers line up.
  [name_interface]
    type = RenameBoundaryGenerator
    input = wall
    old_boundary = 'right'
    new_boundary = 'interface'
  []
[]

[Variables]
  # Solid wall temperature - the field this parent app solves for.
  [T]
    initial_condition = 560.0
  []
[]

[AuxVariables]
  # The FLUID's interface temperature, PULLED from the sub-app each fixed-point
  # iteration (from_multi_app transfer below). The solid's Robin BC uses it as
  # the far-field temperature. initial_condition seeds the very first solve,
  # before any transfer has run.
  [T_fluid_if]
    initial_condition = ${T_fluid_if_init}
  []
[]

[Kernels]
  # Steady heat conduction in the solid: -div(k grad T) = q_source.
  [conduction]
    type = HeatConduction
    variable = T
  []
  # Volumetric heat source (the wall is internally heated).
  [heat_source]
    type = HeatSource
    variable = T
    value = ${q_source}
  []
[]

[BCs]
  # Shared interface (x = 1): convective/Robin coupling to the fluid. The solid
  # surface exchanges heat with the fluid's interface temperature (T_fluid_if,
  # received from the sub) through the gap conductance htc. This is the mirror of
  # the BC the fluid sub-app applies -> Robin-Robin.
  [interface_convection]
    type = CoupledConvectiveHeatFluxBC
    variable = T
    boundary = 'interface'
    T_infinity = T_fluid_if # far-field = fluid interface temp (from sub-app)
    htc = ${htc}
  []
  # Outer wall (x = 0): fixed (cooled) temperature. Giving the solid a finite
  # through-thickness conductance on BOTH faces (this Dirichlet wall + the Robin
  # interface) is what makes the Picard loop converge quickly; a fully insulated
  # outer wall pins all the heat flux through the gap and makes the fixed-point
  # iteration converge very slowly. This also keeps the problem well-posed.
  [outer_wall]
    type = DirichletBC
    variable = T
    boundary = 'left'
    value = ${T_solid_outer}
  []
[]

[Materials]
  # Constant solid conductivity (+ thermal_conductivity_dT the kernel expects).
  [solid_props]
    type = HeatConductionMaterial
    thermal_conductivity = ${k_solid}
    specific_heat = 1.0
  []
[]

[MultiApps]
  # Launch the fluid channel as a sub-app. FullSolveMultiApp = solve the sub to
  # steady state each time it is executed. With Picard iteration on, this runs
  # once per fixed-point iteration. execute_on = TIMESTEP_END so the sub solves
  # AFTER the solid solve within each iteration.
  [fluid]
    type = FullSolveMultiApp
    input_files = 'cht_fluid_sub.i'
    positions = '0 0 0' # same coordinate frame -> point-to-point transfers
    execute_on = 'TIMESTEP_END'
  []
[]

[Transfers]
  # (1) PUSH: send the solid's interface temperature INTO the sub's T_solid_if
  #     aux variable, restricted to the shared interface sideset on both sides.
  [solid_temp_to_fluid]
    type = MultiAppGeneralFieldShapeEvaluationTransfer
    to_multi_app = fluid
    source_variable = T # solid temperature on this (parent) app
    variable = T_solid_if # target aux var in the sub-app
    # Shape-evaluation reads the source field at the TARGET points, so only the
    # destination is boundary-restricted (from_boundaries is not supported here).
    # The target interface (x=1) is coincident with the solid edge, so the field
    # is sampled exactly at the solid surface.
    to_boundaries = 'interface'
  []
  # (2) PULL: bring the fluid's interface temperature OUT of the sub (its solved
  #     field T) into this parent's T_fluid_if aux variable.
  [fluid_temp_from_fluid]
    type = MultiAppGeneralFieldShapeEvaluationTransfer
    from_multi_app = fluid
    source_variable = T # fluid temperature in the sub-app
    variable = T_fluid_if # target aux var on this (parent) app
    to_boundaries = 'interface'
  []
[]

[Postprocessors]
  # Solid-side interface average temperature (computed here on the parent).
  [T_solid_interface_avg]
    type = SideAverageValue
    variable = T
    boundary = 'interface'
  []
  # Fluid-side interface average temperature, evaluated from the field
  # (T_fluid_if) we pulled out of the sub. At Picard convergence this should
  # match T_solid_interface_avg -> a single continuous interface temperature.
  [T_fluid_interface_avg]
    type = SideAverageValue
    variable = T_fluid_if
    boundary = 'interface'
  []
[]

[Executioner]
  type = Steady
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
  # An absolute tolerance is important under Picard: once the loop nears
  # convergence the solid solve starts from a tiny residual, so a pure relative
  # tolerance becomes unreachable (it would chase the roundoff floor). nl_abs_tol
  # lets each inner solve declare success.
  nl_rel_tol = 1e-9
  nl_abs_tol = 1e-8

  # ---- FIXED-POINT (Picard) iteration lives on the PARENT only -------------
  # Each fixed-point iteration: solve solid -> push interface T to fluid ->
  # solve fluid -> pull interface T back. Repeat until the coupled interface
  # temperature stops changing (the relative fixed-point residual drops).
  fixed_point_max_its = 10
  fixed_point_rel_tol = 1e-7
  fixed_point_abs_tol = 1e-9
  # relaxation_factor = 0.5  # uncomment if the interface temperature oscillates
[]

[Outputs]
  exodus = true
[]
