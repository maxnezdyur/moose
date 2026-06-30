# Day 2 example: CREEP at elevated temperature (the payoff of inelasticity).
#
# A square block is pulled by a CONSTANT tensile load and held at a FIXED
# elevated temperature. Even though the load never changes, the material keeps
# deforming with time -- this time-dependent, irreversible flow is CREEP.
#
# Physics demonstrated:
#   * ComputeMultipleInelasticStress wrapping a PowerLawCreepStressUpdate model
#   * power-law creep rate:  d(creep_strain)/dt = A * vonMises^n * exp(-Q/(R*T))
#   * STATEFUL / PATH-DEPENDENT material: the accumulated creep strain is a
#     stored state variable. The answer at this step depends on the whole load
#     history, not just the current load -- that is why we must take time steps.
#
# Try this after running: change the temperature or the load and watch how
# strongly the creep rate responds (n=4 in stress, Arrhenius in temperature).

# 'displacements' is needed by the SolidMechanics action and the strain materials.
[GlobalParams]
  displacements = 'disp_x disp_y'
[]

# A simple 2D block. Boundaries auto-named bottom/top/left/right.
[Mesh]
  [block]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 6
    ny = 6
    xmax = 0.1 # m
    ymax = 0.1 # m
  []
[]

# Fixed elevated temperature supplied to the creep model. It is a constant
# AuxVariable: the initial_condition sets it and nothing changes it, so the
# whole block sits at 1000 K for the entire run (hot enough for steel to creep).
[AuxVariables]
  [T]
    initial_condition = 1000 # K
  []
  # Scalar accumulated (equivalent) creep strain that the radial-return model
  # carries forward as its stored state. It only ever increases -- the cleanest
  # single field for watching creep "switch on" and march upward in time.
  [effective_creep_strain]
    order = CONSTANT
    family = MONOMIAL
  []
[]

# Copy the scalar creep state out of the material system into the AuxVariable
# above so it lands in the Exodus file for visualization.
[AuxKernels]
  [effective_creep_strain]
    type = MaterialRealAux
    variable = effective_creep_strain
    property = effective_creep_strain
    execute_on = 'initial timestep_end'
  []
[]

# SolidMechanics QuasiStatic action: adds disp_x/disp_y, the stress-divergence
# kernels, and the strain material. strain = FINITE gives an INCREMENTAL strain,
# which the inelastic (creep) stress update requires. generate_output asks the
# action to build the AuxVariables we want to visualize / post-process.
[Physics/SolidMechanics/QuasiStatic]
  [all]
    add_variables = true
    strain = FINITE
    # Components to visualize in ParaView:
    #   * total strain_yy = elastic_strain_yy + creep_strain_yy (watch the split shift
    #     from elastic to creep with time at ~constant stress)
    #   * creep_strain_xx is the lateral creep strain: creep is deviatoric / volume
    #     preserving, so it contracts sideways (~ -0.5 * creep_strain_yy)
    generate_output = 'vonmises_stress stress_yy strain_yy creep_strain_yy creep_strain_xx elastic_strain_yy'
  []
[]

[Materials]
  # Linear elastic response (steel-like): sets the elasticity tensor.
  [elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 2.0e11 # Pa
    poissons_ratio = 0.3
  []

  # The stress calculator. ComputeMultipleInelasticStress does the radial-return
  # loop: it splits the strain increment into elastic + inelastic parts so the
  # listed inelastic_models can flow. Here the only inelastic mechanism is creep.
  [radial_return_stress]
    type = ComputeMultipleInelasticStress
    inelastic_models = 'creep'
  []

  # Power-law (Norton) creep, the inelastic model itself.
  #   creep rate = coefficient * vonMises^n_exponent * exp(-activation_energy/(R*T))
  # The strong stress (n=4) and Arrhenius temperature dependence are what make
  # creep "switch on" only when both load and temperature are high.
  [creep]
    type = PowerLawCreepStressUpdate
    coefficient = 1.0e-16 # A (units consistent with Pa and s)
    n_exponent = 4 # stress exponent
    activation_energy = 3.0e5 # Q (J/mol)
    # gas_constant defaults to 8.3143 J/mol-K
    temperature = T # reads the fixed 1000 K field above
  []
[]

# Constant tensile pull on the top face, held flat in time so the LOAD never
# changes -- any continuing deformation is therefore purely creep. A Pressure
# is compressive, so a negative factor pulls outward (tension). The bottom is
# held in y and the left edge in x (symmetry rollers) to remove rigid motion.
[BCs]
  [pull_top]
    type = Pressure
    variable = disp_y
    boundary = top
    factor = -20.0e6 # 20 MPa tension
  []
  [bottom_y]
    type = DirichletBC
    variable = disp_y
    boundary = bottom
    value = 0.0
  []
  [left_x]
    type = DirichletBC
    variable = disp_x
    boundary = left
    value = 0.0
  []
[]

# Full off-diagonal Jacobian -> robust Newton convergence for the nonlinear
# (stress-dependent) creep update.
[Preconditioning]
  [smp]
    type = SMP
    full = true
  []
[]

# Track the creep story over time: average accumulated creep strain (should
# grow steadily), the von Mises stress (stays ~constant under fixed load), and
# the top-face displacement (creeps outward). Compare creep vs elastic strain.
[Postprocessors]
  [avg_creep_strain_yy]
    type = ElementAverageValue
    variable = creep_strain_yy
  []
  [avg_elastic_strain_yy]
    type = ElementAverageValue
    variable = elastic_strain_yy
  []
  [avg_vonmises]
    type = ElementAverageValue
    variable = vonmises_stress
  []
  [top_disp_y]
    type = NodalExtremeValue
    variable = disp_y
    boundary = top
  []
[]

# Transient: creep is rate-dependent, so we MUST march in time. Each step
# carries forward the stored creep strain (the path-dependent state).
[Executioner]
  type = Transient
  solve_type = NEWTON
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  automatic_scaling = true
  num_steps = 10
  dt = 1.0
  nl_rel_tol = 1e-8
[]

[Outputs]
  exodus = true
[]
