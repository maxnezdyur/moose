[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  [square]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 128
    ny = 128
  []
  [pin_x]
    type = ExtraNodesetGenerator
    input = square
    new_boundary = 'pin_x'
    coord = '0.5 0'
  []
  [pin_y_left]
    type = ExtraNodesetGenerator
    input = pin_x
    new_boundary = 'pin_y_left'
    coord = '0 0'
  []
  [pin_y_right]
    type = ExtraNodesetGenerator
    input = pin_y_left
    new_boundary = 'pin_y_right'
    coord = '1 0'
  []
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
  [temperature]
  []
[]

[Kernels]
  [heat_conduction]
    type = Diffusion
    variable = temperature
  []
[]

[Physics/SolidMechanics/QuasiStatic]
  [all]
    strain = SMALL
    temperature = temperature
    eigenstrain_names = thermal_expansion
    rigid_body_near_null_space = true
  []
[]

[Materials]
  [elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 1e6
    poissons_ratio = 0.3
  []
  [elastic_stress]
    type = ComputeLinearElasticStress
  []
  [thermal_expansion]
    type = ComputeThermalExpansionEigenstrain
    temperature = temperature
    stress_free_temperature = 0.0
    thermal_expansion_coeff = 1e-4
    eigenstrain_name = thermal_expansion
  []
[]

[BCs]
  [fix_x]
    type = DirichletBC
    variable = disp_x
    boundary = 'pin_x'
    value = 0.0
  []
  [fix_y]
    type = DirichletBC
    variable = disp_y
    boundary = 'pin_y_left pin_y_right'
    value = 0.0
  []
  [temp_pin]
    type = DirichletBC
    variable = temperature
    boundary = 'pin_y_left'
    value = 0.0
  []
  [temp_flux_left]
    type = NeumannBC
    variable = temperature
    boundary = 'left'
    value = -100.0
  []
  [temp_flux_right]
    type = NeumannBC
    variable = temperature
    boundary = 'right'
    value = 100.0
  []
[]

[Postprocessors]
  [num_linear_iterations]
    type = NumLinearIterations
    execute_on = 'timestep_end'
  []
  [disp_x_right]
    type = PointValue
    variable = disp_x
    point = '1 0.5 0'
    execute_on = 'timestep_end'
  []
  [temperature_right]
    type = PointValue
    variable = temperature
    point = '1 0.5 0'
    execute_on = 'timestep_end'
  []
[]

[Executioner]
  type = Steady
  solve_type = NEWTON
  line_search = none
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'gamg'
  l_max_its = 200
  l_tol = 1e-6
  nl_rel_tol = 1e-9
  nl_abs_tol = 1e-11
[]

[Outputs]
  csv = true
[]
