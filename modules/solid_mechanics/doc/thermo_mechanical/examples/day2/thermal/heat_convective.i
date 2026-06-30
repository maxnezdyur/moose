# Day 2 thermal example: CONVECTIVE COOLING (Robin boundary condition)
#
# A hot reactor structural wall is held at the core temperature on its inner
# face and cooled by a flowing coolant on its outer face.  The coolant carries
# heat away by CONVECTION, modeled with Newton's law of cooling:
#
#     q'' = h * (T_surface - T_infinity)
#
# This is a Robin (mixed) boundary condition: the surface heat flux depends on
# the unknown surface temperature itself.  The cooled-surface temperature
# therefore "floats" and settles to a value BETWEEN the hot interior and the
# cold coolant, set by the balance of conduction through the wall and
# convection off the surface.
#
# Expected steady result (1-D resistance network, k=18, L=0.1, h=500):
#   T_surface ~ 432 K  (between the 800 K hot face and the 300 K coolant)
#   convective heat removed ~ 13.2 kW over the 0.2 m x 1 m cooled face.

# --- Mesh: a 2-D slab; X is the through-thickness (conduction) direction ------
[Mesh]
  [slab]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 20      # cells across the wall thickness (the conduction direction)
    ny = 8
    xmax = 0.1   # wall thickness (m): left = hot face, right = cooled face
    ymax = 0.2   # wall height (m)
  []
[]

# --- Primary variable: temperature, started at the coolant temperature --------
[Variables]
  [temperature]
    initial_condition = 300 # K (start cold, watch the wall heat up and settle)
  []
[]

# --- Governing equation: transient heat conduction ----------------------------
# rho*cp*dT/dt = div(k grad T).  Two kernels: the conduction (diffusion) term
# and the heat-capacity (time-derivative) term.
[Kernels]
  [conduction]
    type = ADHeatConduction
    variable = temperature
  []
  [time_derivative]
    type = ADHeatConductionTimeDerivative
    variable = temperature
    specific_heat = specific_heat
    density_name = density
  []
[]

# --- Material properties: a representative steel ------------------------------
[Materials]
  [steel]
    type = ADGenericConstantMaterial
    prop_names = 'thermal_conductivity specific_heat density'
    prop_values = '18 466 8000' # W/m-K, J/kg-K, kg/m^3
  []
[]

# --- Boundary conditions: the THREE basic thermal BC types --------------------
# (1) DIRICHLET  : prescribes the VALUE of T on the boundary (a fixed temperature).
# (2) NEUMANN    : prescribes the FLUX through the boundary (here 0 = insulated).
# (3) ROBIN/CONVECTIVE : prescribes flux as a function of the unknown surface T,
#                        h*(T - T_infinity) -- this example's focus.
[BCs]
  # (1) Dirichlet: the inner face is bonded to the hot core -> fixed temperature.
  [hot_face]
    type = ADDirichletBC
    variable = temperature
    boundary = left
    value = 800 # K
  []

  # (2) Neumann: top and bottom edges are adiabatic -> prescribed zero heat flux.
  [insulated_edges]
    type = ADNeumannBC
    variable = temperature
    boundary = 'top bottom'
    value = 0 # W/m^2 (no heat crosses these faces)
  []

  # (3) Robin/convective: the outer face loses heat to the coolant by convection.
  # T_infinity_functor is the far-field coolant temperature; the heat-transfer
  # coefficient functor h sets how strongly the surface couples to the coolant.
  [cooled_surface]
    type = ADConvectiveHeatFluxBC
    variable = temperature
    boundary = right
    T_infinity_functor = 300                 # coolant (far-field) temperature, K
    heat_transfer_coefficient_functor = 500  # h, W/m^2-K
  []
[]

# --- Visualization outputs: extra fields written to Exodus for ParaView -------
# These do NOT affect the solve; they paint the physics onto the mesh so a
# student opening the Exodus file can SEE the wall's properties and how heat
# moves through it -- not just the temperature field.
[AuxVariables]
  # The wall's thermal conductivity k (uniform here: one representative steel).
  # Useful to read off the constant that sets the conductive resistance.
  [thermal_conductivity]
    order = CONSTANT
    family = MONOMIAL
  []
  # Through-wall conductive heat flux q_x = -k dT/dx (W/m^2).  X is the
  # conduction direction, so this is the heat marching from the hot face toward
  # the cooled surface; at steady state it is ~uniform across the wall and
  # balances the convection carried off the cooled face.
  [heat_flux_x]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[AuxKernels]
  # Sample the AD thermal_conductivity material property into its field.
  [thermal_conductivity]
    type = ADMaterialRealAux
    variable = thermal_conductivity
    property = thermal_conductivity
    execute_on = 'initial timestep_end'
  []
  # Compute the x-component of the conductive flux vector -k grad(T).
  [heat_flux_x]
    type = DiffusionFluxAux
    variable = heat_flux_x
    diffusion_variable = temperature
    diffusivity = thermal_conductivity
    component = x
    execute_on = 'initial timestep_end'
  []
[]

# --- Postprocessors: what the instructor reads off the console ----------------
[Postprocessors]
  # Confirms the fixed-temperature (Dirichlet) inner face stays at 800 K.
  [hot_face_temperature]
    type = SideAverageValue
    variable = temperature
    boundary = left
  []
  # The star of the example: the cooled surface temperature settles between
  # the 800 K interior and the 300 K coolant (~432 K at steady state).
  [cooled_surface_temperature]
    type = SideAverageValue
    variable = temperature
    boundary = right
  []
  # Convective heat removed: the conductive flux reaching the cooled surface
  # equals the heat convected away there (W).  Uses the AD diffusive-flux form
  # since the conductivity is an AD material property.
  [convective_heat_removed]
    type = ADSideDiffusiveFluxIntegral
    variable = temperature
    boundary = right
    diffusivity = thermal_conductivity
  []
[]

# --- Executioner: short transient, marched to a near-steady balance -----------
[Executioner]
  type = Transient
  num_steps = 30
  dt = 250        # s; ~7500 s total, several thermal diffusion times
  solve_type = NEWTON
  # Absolute tolerance: as the wall nears steady state the per-step residual is
  # already tiny, so we converge on the absolute residual instead of a (then
  # unreachable) relative drop.
  nl_abs_tol = 1e-9
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
[]
