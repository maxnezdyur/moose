# ==============================================================================
# Day 2 - COUPLED thermo-mechanics: the bimetallic strip (the visual one)
# ------------------------------------------------------------------------------
# A thin 2-D strip is split through its thickness into two bonded layers made of
# DIFFERENT metals. The two layers share the SAME temperature but have DIFFERENT
# thermal-expansion coefficients. Heat the whole strip uniformly and each layer
# wants to grow by a different amount; because they are bonded they cannot, so
# the strip relieves the mismatch by BENDING.
#
# Why differential expansion causes curvature:
#   When heated by dT, an unconstrained layer would stretch axially by alpha*dT.
#   The top layer (larger alpha) wants a longer fiber than the bottom layer.
#   Bonded at the interface, the only way both fibers can keep their preferred
#   lengths is for the strip to curve, putting the higher-expansion layer on the
#   OUTSIDE (longer arc) and the lower-expansion layer on the INSIDE (shorter
#   arc). With the high-expansion layer on top, the center of curvature sits
#   BELOW the strip; clamped at the left, the free right end deflects DOWNWARD.
#
# What this reinforces:
#   * per-block materials (different elasticity AND different alpha per subdomain)
#   * the thermal-expansion EIGENSTRAIN as the heat -> mechanics coupling
#   * a genuine, monolithic heat + mechanics solve (temperature is SOLVED, not
#     prescribed) using the QuasiStatic SolidMechanics action with AD
#   * just-enough constraint: clamp ONE end so the strip is free to bend
# ==============================================================================

# Displacement variables created/used by the SolidMechanics action everywhere.
[GlobalParams]
  displacements = 'disp_x disp_y'
[]

# Long, thin strip (length 1.0 m in x, thickness 0.1 m in y), then split into a
# bottom and a top layer of equal thickness using bounding boxes. The split at
# y = 0.05 lands exactly on an element boundary (ny = 8 -> rows every 0.0125 m).
[Mesh]
  [strip]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 40
    ny = 8
    xmax = 1.0 # strip length (m)
    ymax = 0.1 # strip thickness (m)
  []
  # Bottom layer subdomain (low-expansion metal).
  [bottom_layer]
    type = SubdomainBoundingBoxGenerator
    input = strip
    block_id = 1
    block_name = bottom_layer
    bottom_left = '0   0    0'
    top_right   = '1.0 0.05 0'
  []
  # Top layer subdomain (high-expansion metal).
  [top_layer]
    type = SubdomainBoundingBoxGenerator
    input = bottom_layer
    block_id = 2
    block_name = top_layer
    bottom_left = '0   0.05 0'
    top_right   = '1.0 0.1  0'
  []
[]

# Temperature is a SOLVED field (a real nonlinear Variable), not a prescribed
# AuxVariable. It starts at the stress-free temperature so there is no initial
# eigenstrain and the strip starts flat.
[Variables]
  [temperature]
    initial_condition = 300 # K
  []
[]

# Heat equation for the temperature field. With a uniform volumetric heat source
# and INSULATED edges (no temperature BC -> natural zero-flux), the solution stays
# spatially uniform and simply rises in time: rho*cp*dT/dt = Q. Uniform heating is
# exactly what we want here so that the bending comes purely from the per-layer
# alpha mismatch and not from any thermal gradient.
[Kernels]
  [conduction]
    type = ADHeatConduction
    variable = temperature
  []
  [time_derivative]
    type = ADHeatConductionTimeDerivative
    variable = temperature
  []
  # Uniform internal heating. With rho*cp = 8000*500 = 4e6 J/(m^3 K), a source of
  # 2.4e8 W/m^3 raises the temperature 60 K/s, i.e. 300 K -> 600 K over 5 s.
  [uniform_heating]
    type = BodyForce
    variable = temperature
    value = 2.4e8 # W/m^3
  []
[]

# SolidMechanics QuasiStatic action: adds disp_x/disp_y, the stress-divergence
# kernels, and the (finite-strain) strain calculator. Listing 'eigenstrain' in
# eigenstrain_names tells the strain calculator to subtract the per-layer thermal
# eigenstrain from the total strain, so only the ELASTIC part makes stress.
# use_automatic_differentiation = true makes the heat -> mechanics coupling
# (d stress / d temperature) exact, giving a clean monolithic NEWTON solve.
# FINITE strain is used because a bending strip undergoes finite rotations.
[Physics/SolidMechanics/QuasiStatic]
  [all]
    add_variables = true
    strain = FINITE
    eigenstrain_names = 'eigenstrain'
    use_automatic_differentiation = true
    generate_output = 'vonmises_stress stress_xx' # stress_xx is the bending (axial) stress
  []
[]

# Mechanical constraint: clamp ONLY the left end (both components). This makes the
# strip a cantilever - fixed at the root, free everywhere else - so it is free to
# bend. The thermal Dirichlet-free edges keep the heating uniform (see [Kernels]).
[BCs]
  [clamp_x]
    type = DirichletBC
    variable = disp_x
    boundary = left
    value = 0
  []
  [clamp_y]
    type = DirichletBC
    variable = disp_y
    boundary = left
    value = 0
  []
[]

[Materials]
  # ---- Bottom layer: low-expansion metal (steel-like) ----
  [elasticity_bottom]
    type = ADComputeIsotropicElasticityTensor
    youngs_modulus = 200e9 # Pa
    poissons_ratio = 0.3
    block = bottom_layer
  []
  [thermal_expansion_bottom]
    type = ADComputeThermalExpansionEigenstrain
    temperature = temperature
    thermal_expansion_coeff = 1.2e-5 # 1/K  (smaller -> grows less)
    stress_free_temperature = 300
    eigenstrain_name = eigenstrain
    block = bottom_layer
  []
  # Re-declare the bottom-layer alpha as a constant material property ONLY so it
  # can be visualized as a field (see [AuxKernels]/alpha). It does not feed the
  # solve - the real coefficient is the one in thermal_expansion_bottom above.
  [alpha_bottom]
    type = ADGenericConstantMaterial
    prop_names  = 'alpha_field'
    prop_values = '1.2e-5'
    block = bottom_layer
  []

  # ---- Top layer: high-expansion metal (aluminum-like) ----
  # Same eigenstrain_name as the bottom layer, but a LARGER coefficient. This
  # alpha mismatch across the bond is the entire source of the curvature.
  [elasticity_top]
    type = ADComputeIsotropicElasticityTensor
    youngs_modulus = 70e9 # Pa
    poissons_ratio = 0.33
    block = top_layer
  []
  [thermal_expansion_top]
    type = ADComputeThermalExpansionEigenstrain
    temperature = temperature
    thermal_expansion_coeff = 2.3e-5 # 1/K  (larger -> grows more -> goes on the outside)
    stress_free_temperature = 300
    eigenstrain_name = eigenstrain
    block = top_layer
  []
  # Top-layer alpha as a constant material property, again ONLY for visualization.
  # Same property name as alpha_bottom but a LARGER value, so the alpha field shows
  # two flat plateaus and the bond line where they meet.
  [alpha_top]
    type = ADGenericConstantMaterial
    prop_names  = 'alpha_field'
    prop_values = '2.3e-5'
    block = top_layer
  []

  # Hooke's law on the elastic strain (total minus the eigenstrain). One stress
  # material covers both blocks; each block uses its own elasticity tensor above.
  [stress]
    type = ADComputeFiniteStrainElasticStress
  []

  # Thermal properties for the heat solve (shared by both layers so the strip
  # heats uniformly). rho*cp sets the heating rate together with the source above.
  [thermal_props]
    type = ADGenericConstantMaterial
    prop_names  = 'thermal_conductivity specific_heat density'
    prop_values = '50                   500           8000' # W/m-K, J/kg-K, kg/m^3
  []
[]

# Extra fields written to Exodus so a student can SEE the coupling and the setup
# in ParaView. These are diagnostics only - they do not affect the solve.
[AuxVariables]
  # Thermal eigenstrain components (element-wise constant, like a material prop).
  [eigenstrain_xx] # axial: this layer mismatch is what drives the bending
    order = CONSTANT
    family = MONOMIAL
  []
  [eigenstrain_yy] # through-thickness: shows the eigenstrain is isotropic per layer
    order = CONSTANT
    family = MONOMIAL
  []
  # The per-layer thermal-expansion coefficient as a literal field.
  [alpha]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[AuxKernels]
  # Pull the xx/yy components out of the AD eigenstrain tensor (the heat -> mechanics
  # coupling). Both jump at the bond line: the top (alpha = 2.3e-5) grows more than
  # the bottom (alpha = 1.2e-5) for the same temperature rise.
  [eigenstrain_xx]
    type = ADRankTwoAux
    rank_two_tensor = eigenstrain
    variable = eigenstrain_xx
    index_i = 0
    index_j = 0
    execute_on = 'initial timestep_end'
  []
  [eigenstrain_yy]
    type = ADRankTwoAux
    rank_two_tensor = eigenstrain
    variable = eigenstrain_yy
    index_i = 1
    index_j = 1
    execute_on = 'initial timestep_end'
  []
  # Paint the literal alpha value onto each layer so you can see the two numbers
  # (1.2e-5 vs 2.3e-5) and exactly WHERE each metal sits.
  [alpha]
    type = ADMaterialRealAux
    variable = alpha
    property = alpha_field
    execute_on = timestep_end
  []
[]

[Postprocessors]
  # Headline result: transverse (y) deflection of the free end, mid-thickness.
  # Negative because the high-expansion layer is on top -> the strip curls down.
  [tip_deflection]
    type = PointValue
    variable = disp_y
    point = '1.0 0.05 0'
  []
  # Peak von Mises stress (largest near the bonded interface and the clamp).
  [max_vonmises]
    type = ElementExtremeValue
    variable = vonmises_stress
  []
  # Average temperature, to confirm the uniform heat-up (300 K -> 600 K).
  [average_temperature]
    type = ElementAverageValue
    variable = temperature
  []
[]

# Short heat-up transient: 5 steps of 1 s carry the strip from 300 K to 600 K,
# so students watch the bend grow step by step. Monolithic full NEWTON.
[Executioner]
  type = Transient
  solve_type = NEWTON
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  end_time = 5
  dt = 1
  automatic_scaling = true
[]

[Outputs]
  exodus = true
[]
