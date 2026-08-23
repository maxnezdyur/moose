[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
[]

[Mesh]
  [cube]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 8
    ny = 8
    nz = 8
  []
  [corner_a]
    type = ExtraNodesetGenerator
    input = cube
    new_boundary = 'corner_a'
    coord = '0 0 0'
  []
  [corner_b]
    type = ExtraNodesetGenerator
    input = corner_a
    new_boundary = 'corner_b'
    coord = '1 0 0'
  []
  [corner_c]
    type = ExtraNodesetGenerator
    input = corner_b
    new_boundary = 'corner_c'
    coord = '0 1 0'
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
  [fix_a_x]
    type = DirichletBC
    variable = disp_x
    boundary = 'corner_a'
    value = 0.0
  []
  [fix_a_y]
    type = DirichletBC
    variable = disp_y
    boundary = 'corner_a'
    value = 0.0
  []
  [fix_a_z]
    type = DirichletBC
    variable = disp_z
    boundary = 'corner_a'
    value = 0.0
  []
  [fix_b_y]
    type = DirichletBC
    variable = disp_y
    boundary = 'corner_b'
    value = 0.0
  []
  [fix_b_z]
    type = DirichletBC
    variable = disp_z
    boundary = 'corner_b'
    value = 0.0
  []
  [fix_c_z]
    type = DirichletBC
    variable = disp_z
    boundary = 'corner_c'
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
    displacements = 'disp_x disp_y disp_z'
  []
[]

[Postprocessors]
  [num_linear_iterations]
    type = NumLinearIterations
    execute_on = 'timestep_end'
  []
  [disp_x_corner]
    type = PointValue
    variable = disp_x
    point = '1 1 1'
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
