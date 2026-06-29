# =============================================================================
# Day 1 - GOOD example #2: "Time stepping"
# =============================================================================
# A square block of metal starts at a uniform temperature. An internal
# (volumetric) heat source -- ramped on in time by a [Function] -- warms the
# block up, while one edge is held cold like a cooled surface. We watch the
# temperature climb step-by-step in time.
#
# Day-1 systems demonstrated (block -> concept):
#   [Mesh]                       the geometry / grid we solve on
#   [Variables]                  the unknown field (temperature) + initial value
#   [Functions]                  a time-dependent recipe (heat-source ramp)
#   [Kernels]                    the PDE terms, including the TRANSIENT term
#   [Materials]                  properties the kernels need (k, cp, rho)
#   [BCs]                        boundary conditions (a cooled edge)
#   [AuxVariables]/[AuxKernels]  a derived field computed from the solution
#   [Postprocessors]             scalar quantities tracked over time (max, avg)
#   [Executioner] (+timestepping)how we march forward in time (dt, num_steps)
#   [Outputs]                    Exodus (field) + CSV (postprocessor history)
# =============================================================================

# ---- [Mesh] : the domain ----------------------------------------------------
# A 1 m x 1 m square, 20x20 elements. GeneratedMeshGenerator auto-creates the
# boundary names left / right / top / bottom that the [BCs] block refers to.
[Mesh]
  [block]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 20
    ny = 20
    xmax = 1.0 # m
    ymax = 1.0 # m
  []
[]

# ---- [Variables] : the unknown we solve for ---------------------------------
# 'temperature' (K). initial_condition sets the UNIFORM starting field at t=0,
# which is essential for a transient: the time march needs somewhere to start.
[Variables]
  [temperature]
    initial_condition = 300 # K (uniform, room temperature)
  []
[]

# ---- [Functions] : a time-dependent recipe ----------------------------------
# ParsedFunction evaluates a math expression. Here the heat-source intensity
# ramps linearly from 0 up to 6e5 W/m^3 over the first 100 s, then holds flat.
# This drives the heat-up and shows how Functions feed other systems.
[Functions]
  [power_ramp]
    type = ParsedFunction
    expression = 'if(t < 100, 6e5 * t / 100, 6e5)' # W/m^3
  []
[]

# ---- [Kernels] : the terms of the heat-conduction PDE -----------------------
#   rho*cp*dT/dt  -  div(k grad T)  =  q'''
# Each kernel below is one term of that equation.
[Kernels]
  # Spatial conduction term: needs a 'thermal_conductivity' material property.
  [conduction]
    type = HeatConduction
    variable = temperature
  []
  # *** TRANSIENT TERM *** rho*cp*dT/dt -- this is what makes the problem
  # time-dependent. It reads the specific_heat and density material properties.
  [time_derivative]
    type = HeatConductionTimeDerivative
    variable = temperature
    specific_heat = specific_heat
    density_name = density
  []
  # Volumetric heat source q''' driven by the [Functions] ramp above.
  [heat_source]
    type = HeatSource
    variable = temperature
    function = power_ramp
  []
[]

# ---- [Materials] : properties the kernels need ------------------------------
# GenericConstantMaterial supplies constant-valued properties by name. The
# kernels above look these names up: thermal_conductivity (conduction),
# specific_heat + density (the transient term).
[Materials]
  [metal]
    type = GenericConstantMaterial
    prop_names = 'thermal_conductivity specific_heat density'
    prop_values = '50 500 8000' # W/m-K, J/kg-K, kg/m^3
  []
[]

# ---- [BCs] : boundary conditions --------------------------------------------
# Hold the right edge at the cold starting temperature, mimicking a cooled
# surface that carries heat away. The other edges are left insulated (the
# natural/default "do-nothing" condition = zero flux).
[BCs]
  [cooled_edge]
    type = DirichletBC
    variable = temperature
    boundary = right
    value = 300 # K
  []
[]

# ---- [AuxVariables] / [AuxKernels] : a derived field ------------------------
# AuxVariables are NOT solved for; they are computed from other quantities.
# Here ParsedAux converts the solved temperature (K) into Celsius for output.
[AuxVariables]
  [temperature_celsius]
  []
[]
[AuxKernels]
  [to_celsius]
    type = ParsedAux
    variable = temperature_celsius
    coupled_variables = 'temperature'
    expression = 'temperature - 273.15' # K -> degC
  []
[]

# ---- [Postprocessors] : scalars tracked over time ---------------------------
# Reduced to single numbers each step; written to CSV to plot a time history.
[Postprocessors]
  # Hottest node in the whole block (the centerline heats most).
  [max_temperature]
    type = NodalExtremeValue
    variable = temperature
    value_type = max
  []
  # Volume-averaged temperature of the block.
  [avg_temperature]
    type = ElementAverageValue
    variable = temperature
  []
[]

# ---- [Executioner] : how we march in TIME -----------------------------------
# type = Transient turns on time stepping.
#   start_time : clock value at the first step
#   dt         : the (constant) time-step size, in seconds
#   num_steps  : how many steps to take  (end_time = start_time + dt*num_steps)
# So 50 steps of 10 s integrates from t = 0 s out to t = 500 s.
# (For adaptive control you would instead nest a [TimeSteppers] block here.)
[Executioner]
  type = Transient
  solve_type = NEWTON
  start_time = 0
  dt = 10          # s  -- the time-step size
  num_steps = 50   # take 50 steps -> t_final = 500 s
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

# ---- [Outputs] : where results go -------------------------------------------
# exodus -> spatial fields (temperature, temperature_celsius) every step.
# csv    -> the postprocessor time history (max & avg temperature vs time).
[Outputs]
  exodus = true
  csv = true
[]
