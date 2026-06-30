# Day 2 coupling example: constrained thermal stress (the simplest coupling).
#
# A single elastic block is heated and clamped on all sides. Because the block
# cannot expand freely, the thermal expansion is converted into mechanical
# (elastic) strain, which produces stress. This isolates the one mechanism that
# couples heat to mechanics: the thermal-expansion EIGENSTRAIN.
#
# The chain demonstrated here is:
#   prescribed temperature field  ->  thermal eigenstrain  ->  resisted by BCs
#   ->  elastic strain  ->  stress (von Mises).
#
# Here the temperature is a PRESCRIBED field (no heat equation is solved), so the
# example stays focused purely on the eigenstrain -> stress step. The reactor
# capstone (reactor_thermomech.i) adds the full heat-conduction solve on top.

# The displacement variables the SolidMechanics action will create and use.
[GlobalParams]
  displacements = 'disp_x disp_y'
[]

# A single square block of elements (one subdomain).
[Mesh]
  [generated]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 20
    ny = 20
    xmax = 1.0 # m
    ymax = 1.0 # m
  []
[]

# Prescribed temperature field. This is an AuxVariable (it is NOT solved for);
# the mechanics simply reads it. A linear gradient in x gives a spatially varying
# thermal stress, which is more instructive than a single uniform value.
[AuxVariables]
  [T]
  []
  # The xx component of the thermal eigenstrain (the coupling quantity). It is a
  # rank-2 material property, so it is output as a constant-per-element field.
  [eigenstrain_xx]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[AuxKernels]
  # Impose the temperature field. Stress-free temperature is 300 K; this ramps
  # from 300 K on the left face to 600 K on the right face.
  [set_temperature]
    type = FunctionAux
    variable = T
    function = '300 + 300*x'
    execute_on = 'INITIAL TIMESTEP_BEGIN'
  []
  # Pull the xx component out of the thermal_expansion eigenstrain tensor so the
  # stress-free expansion the heat wants is visible next to the elastic strain
  # and stress it produces.
  [eigenstrain_xx]
    type = RankTwoAux
    rank_two_tensor = thermal_expansion
    variable = eigenstrain_xx
    index_i = 0
    index_j = 0
    execute_on = 'INITIAL TIMESTEP_END'
  []
[]

# SolidMechanics QuasiStatic action: adds the displacement variables, the
# stress-divergence kernels, and the strain calculator. Listing the eigenstrain
# in eigenstrain_names is what tells the strain calculator to SUBTRACT the
# thermal (eigen) strain from the total strain:
#     elastic_strain = total_strain - thermal_eigenstrain
# Only the elastic part generates stress, so a clamped, heated block (total
# strain ~ 0) is left with a compressive elastic strain and thus stress.
[Physics/SolidMechanics/QuasiStatic]
  [all]
    add_variables = true
    strain = SMALL
    eigenstrain_names = thermal_expansion
    # Outputs (auto-created AuxVariables, visible in the Exodus). strain_xx and
    # elastic_strain_xx make the coupling chain visible alongside the eigenstrain
    # output added below, since these three obey
    #     strain_xx (total) = elastic_strain_xx + eigenstrain_xx
    #   - strain_xx: the TOTAL strain, ~0 because the block is clamped.
    #   - elastic_strain_xx: what is left over (compressive); this is what makes
    #     the stress, so it tracks stress_xx / vonmises_stress.
    generate_output = 'vonmises_stress stress_xx stress_yy '
                      'strain_xx elastic_strain_xx'
  []
[]

# Mechanical constraint: clamp every face in its normal direction. This resists
# the thermal expansion (the block cannot grow) so the expansion shows up as
# elastic strain / stress rather than as free displacement.
[BCs]
  [left_x]
    type = DirichletBC
    variable = disp_x
    boundary = left
    value = 0
  []
  [right_x]
    type = DirichletBC
    variable = disp_x
    boundary = right
    value = 0
  []
  [bottom_y]
    type = DirichletBC
    variable = disp_y
    boundary = bottom
    value = 0
  []
  [top_y]
    type = DirichletBC
    variable = disp_y
    boundary = top
    value = 0
  []
[]

[Materials]
  # Linear elastic stiffness (steel-like).
  [elasticity]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 200e9 # Pa
    poissons_ratio = 0.3
  []
  # Thermal-expansion eigenstrain: the stress-free strain produced by heating
  # above the stress_free_temperature. This is the ONLY object that couples the
  # temperature field T into the mechanics.
  [thermal_expansion]
    type = ComputeThermalExpansionEigenstrain
    temperature = T
    thermal_expansion_coeff = 1e-5 # 1/K
    stress_free_temperature = 300 # K
    eigenstrain_name = thermal_expansion
  []
  # Hooke's law on the ELASTIC strain (total minus the eigenstrain above).
  [stress]
    type = ComputeLinearElasticStress
  []
[]

# Consider all off-diagonal Jacobian entries for the (Newton) preconditioner.
[Preconditioning]
  [SMP]
    type = SMP
    full = true
  []
[]

# Steady solve: there is no time dependence, the temperature field is fixed.
[Executioner]
  type = Steady
  solve_type = NEWTON
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
[]

[Postprocessors]
  # Peak von Mises stress in the block - the headline result of the example.
  [max_vonmises]
    type = ElementExtremeValue
    variable = vonmises_stress
  []
  # Average prescribed temperature, for context.
  [average_temperature]
    type = ElementAverageValue
    variable = T
  []
[]

[Outputs]
  exodus = true
[]
