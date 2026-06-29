# Day 2 worked example: STEADY multi-material heat conduction.
#
# A flat bar is split lengthwise into three named subdomains, each a different
# metal with its OWN thermal conductivity. The two ends are held at fixed
# temperatures, so heat flows in series through the three blocks. Because the
# conductivities differ, the steady temperature profile is PIECEWISE-LINEAR
# with a visible slope KINK at each material interface:
#
#   - lower-conductivity block  -> steeper temperature gradient
#   - higher-conductivity block -> shallower temperature gradient
#
# Key teaching point reinforced by the postprocessors: even though the GRADIENT
# jumps at an interface, the HEAT FLUX (k * grad T) is CONTINUOUS across it --
# the same energy that enters one block must leave it into the next.

[Mesh]
  # Start with one rectangular bar, 3 m long (x) and 0.5 m tall (y).
  # nx = 60 puts an element boundary exactly at x = 1 and x = 2, so the
  # three subdomains and their interfaces line up cleanly.
  [bar]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 60
    ny = 6
    xmax = 3.0
    ymax = 0.5
  []

  # --- Subdomains: carve the bar into three named blocks by x-position ---
  # SubdomainBoundingBoxGenerator reassigns every element whose centroid lies
  # inside the box to the given block id/name.
  [carbon_steel]
    type = SubdomainBoundingBoxGenerator
    input = bar
    bottom_left = '0.0 0.0 0.0'
    top_right = '1.0 0.5 0.0'
    block_id = 1
    block_name = carbon_steel
  []
  [stainless_steel]
    type = SubdomainBoundingBoxGenerator
    input = carbon_steel
    bottom_left = '1.0 0.0 0.0'
    top_right = '2.0 0.5 0.0'
    block_id = 2
    block_name = stainless_steel
  []
  [iron]
    type = SubdomainBoundingBoxGenerator
    input = stainless_steel
    bottom_left = '2.0 0.0 0.0'
    top_right = '3.0 0.5 0.0'
    block_id = 3
    block_name = iron
  []

  # --- Interface sidesets: thin internal surfaces between the blocks ---
  # We need named sides at the material interfaces so we can integrate the
  # heat flux there (to show it is continuous).
  [interface_12]
    type = SideSetsBetweenSubdomainsGenerator
    input = iron
    primary_block = carbon_steel
    paired_block = stainless_steel
    new_boundary = interface_12
  []
  [interface_23]
    type = SideSetsBetweenSubdomainsGenerator
    input = interface_12
    primary_block = stainless_steel
    paired_block = iron
    new_boundary = interface_23
  []
[]

# Single temperature field solved over the whole bar.
[Variables]
  [temperature]
    initial_condition = 400 # K (any value works; this is a steady solve)
  []
[]

# Steady heat conduction:  div( -k grad T ) = 0  (no source, no time term).
[Kernels]
  [conduction]
    type = HeatConduction
    variable = temperature
    # uses the material property "thermal_conductivity" supplied per block below
  []
[]

# --- Per-block materials: each subdomain gets a DIFFERENT conductivity ---
# Values are realistic, contrasting metals (W/m-K) chosen so all three kinks
# are clearly visible. The middle block (stainless) is the least conductive,
# so it shows the steepest gradient.
[Materials]
  [k_carbon_steel]
    type = HeatConductionMaterial
    block = carbon_steel
    thermal_conductivity = 45 # carbon steel
  []
  [k_stainless_steel]
    type = HeatConductionMaterial
    block = stainless_steel
    thermal_conductivity = 15 # stainless steel (lowest k -> steepest slope)
  []
  [k_iron]
    type = HeatConductionMaterial
    block = iron
    thermal_conductivity = 80 # pure iron (highest k -> shallowest slope)
  []
[]

# Fix temperature at the two ends -> heat is driven through the bar in series.
[BCs]
  [hot_end]
    type = DirichletBC
    variable = temperature
    boundary = left
    value = 500 # K
  []
  [cold_end]
    type = DirichletBC
    variable = temperature
    boundary = right
    value = 300 # K
  []
[]

[Postprocessors]
  # Temperature at each interface: confirms the kink locations and lets students
  # read off the unequal temperature drops across the three blocks.
  [T_interface_12]
    type = PointValue
    variable = temperature
    point = '1.0 0.25 0.0'
  []
  [T_interface_23]
    type = PointValue
    variable = temperature
    point = '2.0 0.25 0.0'
  []

  # Heat flux ( k * grad T integrated over the side ). Compare these four:
  # the magnitude is the SAME everywhere -> flux is continuous in series,
  # including ACROSS each material interface.
  [flux_hot_end]
    type = SideDiffusiveFluxIntegral
    variable = temperature
    boundary = left
    diffusivity = thermal_conductivity
  []
  [flux_interface_12]
    type = SideDiffusiveFluxIntegral
    variable = temperature
    boundary = interface_12
    diffusivity = thermal_conductivity
  []
  [flux_interface_23]
    type = SideDiffusiveFluxIntegral
    variable = temperature
    boundary = interface_23
    diffusivity = thermal_conductivity
  []
  [flux_cold_end]
    type = SideDiffusiveFluxIntegral
    variable = temperature
    boundary = right
    diffusivity = thermal_conductivity
  []
[]

# Steady-state, linear conduction -> converges in a single Newton step.
[Executioner]
  type = Steady
  solve_type = NEWTON
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
[]

[Outputs]
  exodus = true
  csv = true # writes the interface temperatures and fluxes for inspection
[]
