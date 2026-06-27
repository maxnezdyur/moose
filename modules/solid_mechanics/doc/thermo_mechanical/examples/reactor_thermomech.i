# Day 2 capstone: coupled thermo-mechanics of a reactor component (axisymmetric).
# A heated, axially-constrained component expands against its supports, producing
# thermal stress. Temperature and displacement are solved monolithically.
#
# Demonstrates: the QuasiStatic SolidMechanics action (finite strain, AD),
# thermal-expansion eigenstrain coupling, heat conduction, and von Mises output.

[GlobalParams]
  displacements = 'disp_r disp_z'
[]

[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 12
    ny = 60
    ymax = 0.20 # component length (m), axial = Y
    xmax = 0.04 # component radius (m)
  []
  coord_type = RZ
[]

[Variables]
  [temperature]
    initial_condition = 300 # K
  []
[]

[Physics/SolidMechanics/QuasiStatic]
  [all]
    # Adds the displacement variables, stress-divergence kernels, and the
    # strain/stress materials in the autodetected (RZ) coordinate system.
    add_variables = true
    strain = FINITE
    eigenstrain_names = eigenstrain
    use_automatic_differentiation = true
    generate_output = 'vonmises_stress stress_yy strain_yy'
  []
[]

[Kernels]
  [heat_conduction]
    type = ADHeatConduction
    variable = temperature
  []
  [heat_conduction_time_derivative]
    type = ADHeatConductionTimeDerivative
    variable = temperature
  []
[]

[BCs]
  [hot_end]
    type = FunctionDirichletBC
    variable = temperature
    boundary = bottom
    function = 'if(t<0, 300 + 250*(t+1), 550)' # ramp to 550 K, then hold
  []
  [cool_end]
    type = DirichletBC
    variable = temperature
    boundary = top
    value = 300
  []
  [hold_bottom_z]
    type = DirichletBC
    variable = disp_z
    boundary = bottom
    value = 0
  []
  [hold_top_z]
    type = DirichletBC
    variable = disp_z
    boundary = top
    value = 0 # axially constrained -> thermal stress develops
  []
  [axis]
    type = DirichletBC
    variable = disp_r
    boundary = left
    value = 0 # RZ symmetry on the centerline
  []
[]

[Materials]
  [thermal]
    type = ADGenericConstantMaterial
    prop_names = 'thermal_conductivity specific_heat density'
    prop_values = '18 466 8000'
  []
  [elasticity_tensor]
    type = ADComputeIsotropicElasticityTensor
    youngs_modulus = 200e9 # Pa
    poissons_ratio = 0.3
  []
  [elastic_stress]
    type = ADComputeFiniteStrainElasticStress
  []
  [thermal_strain]
    type = ADComputeThermalExpansionEigenstrain
    stress_free_temperature = 300
    eigenstrain_name = eigenstrain
    temperature = temperature
    thermal_expansion_coeff = 1e-5 # 1/K
  []
[]

[Postprocessors]
  [average_temperature]
    type = ElementAverageValue
    variable = temperature
  []
  [max_vonmises]
    type = ElementExtremeValue
    variable = vonmises_stress
  []
[]

[Executioner]
  type = Transient
  start_time = -1
  end_time = 200
  dt = 10
  solve_type = NEWTON
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  automatic_scaling = true
[]

[Outputs]
  exodus = true
[]
