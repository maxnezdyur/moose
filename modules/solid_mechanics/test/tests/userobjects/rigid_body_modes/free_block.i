[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  [square]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 10
    ny = 10
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

[Physics/SolidMechanics/QuasiStatic]
  [all]
    strain = SMALL
    add_variables = true
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
  [pull_right]
    type = NeumannBC
    variable = disp_x
    boundary = 'right'
    value = 1e4
  []
  [pull_left]
    type = NeumannBC
    variable = disp_x
    boundary = 'left'
    value = -1e4
  []
[]

[UserObjects]
  [rigid_body_modes]
    type = RigidBodyModes
    displacements = 'disp_x disp_y'
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
