[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 2
    ny = 2
    nz = 2
  []
  [distort]
    type = ParsedNodeTransformGenerator
    input = gen
    constant_names = 'amp'
    constant_expressions = '0' # cli_args raises amp to bend the grid into general hexahedra
    x_function = 'x + amp * (0.35 * x * y + 0.15 * y * y + 0.20 * y * z)'
    y_function = 'y + amp * (0.25 * x * y - 0.20 * x * x + 0.18 * x * z)'
    z_function = 'z + amp * (0.30 * x * z + 0.22 * y * z - 0.16 * x * y)'
  []
[]

[Functions]
  [affine_x]
    type = ParsedFunction
    expression = '2.0e-3 * x + 7.0e-4 * y + 3.0e-4 * z + 1.0e-4'
  []
  [affine_y]
    type = ParsedFunction
    expression = '5.0e-4 * x + 3.0e-3 * y + 9.0e-4 * z + 2.0e-4'
  []
  [affine_z]
    type = ParsedFunction
    expression = '4.0e-4 * x + 6.0e-4 * y + 1.5e-3 * z + 1.5e-4'
  []
[]

[Variables]
  [disp_x]
    order = FIRST
    family = LAGRANGE
  []
  [disp_y]
    order = FIRST
    family = LAGRANGE
  []
  [disp_z]
    order = FIRST
    family = LAGRANGE
  []
[]

[AuxVariables]
  [hourglass_force_x]
    order = FIRST
    family = LAGRANGE
  []
  [hourglass_force_y]
    order = FIRST
    family = LAGRANGE
  []
  [hourglass_force_z]
    order = FIRST
    family = LAGRANGE
  []
  [stress_xx]
    order = CONSTANT
    family = MONOMIAL
  []
  [stress_yy]
    order = CONSTANT
    family = MONOMIAL
  []
  [stress_zz]
    order = CONSTANT
    family = MONOMIAL
  []
  [stress_xy]
    order = CONSTANT
    family = MONOMIAL
  []
  [stress_yz]
    order = CONSTANT
    family = MONOMIAL
  []
  [stress_zx]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[Kernels]
  [stress_divergence_x]
    type = StressDivergenceTensors
    variable = disp_x
    component = 0
    displacements = 'disp_x disp_y disp_z'
  []
  [stress_divergence_y]
    type = StressDivergenceTensors
    variable = disp_y
    component = 1
    displacements = 'disp_x disp_y disp_z'
  []
  [stress_divergence_z]
    type = StressDivergenceTensors
    variable = disp_z
    component = 2
    displacements = 'disp_x disp_y disp_z'
  []
  [hourglass_x]
    type = HourglassStabilization
    variable = disp_x
    penalty = 0.1
    save_in = 'hourglass_force_x'
  []
  [hourglass_y]
    type = HourglassStabilization
    variable = disp_y
    penalty = 0.1
    save_in = 'hourglass_force_y'
  []
  [hourglass_z]
    type = HourglassStabilization
    variable = disp_z
    penalty = 0.1
    save_in = 'hourglass_force_z'
  []
[]

[AuxKernels]
  [stress_xx]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_xx
    index_i = 0
    index_j = 0
  []
  [stress_yy]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_yy
    index_i = 1
    index_j = 1
  []
  [stress_zz]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_zz
    index_i = 2
    index_j = 2
  []
  [stress_xy]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_xy
    index_i = 0
    index_j = 1
  []
  [stress_yz]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_yz
    index_i = 1
    index_j = 2
  []
  [stress_zx]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_zx
    index_i = 2
    index_j = 0
  []
[]

[BCs]
  [affine_disp_x]
    type = FunctionDirichletBC
    variable = disp_x
    boundary = 'left right bottom top back front'
    function = affine_x
  []
  [affine_disp_y]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = 'left right bottom top back front'
    function = affine_y
  []
  [affine_disp_z]
    type = FunctionDirichletBC
    variable = disp_z
    boundary = 'left right bottom top back front'
    function = affine_z
  []
[]

[Materials]
  [elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 1.0e6
    poissons_ratio = 0.3
  []
  [strain]
    type = ComputeSmallStrain
    displacements = 'disp_x disp_y disp_z'
  []
  [stress]
    type = ComputeLinearElasticStress
  []
[]

[Postprocessors]
  [stress_xx_avg]
    type = ElementAverageValue
    variable = stress_xx
  []
  [stress_yy_avg]
    type = ElementAverageValue
    variable = stress_yy
  []
  [stress_zz_avg]
    type = ElementAverageValue
    variable = stress_zz
  []
  [stress_xy_avg]
    type = ElementAverageValue
    variable = stress_xy
  []
  [stress_yz_avg]
    type = ElementAverageValue
    variable = stress_yz
  []
  [stress_zx_avg]
    type = ElementAverageValue
    variable = stress_zx
  []
  [stress_xx_max]
    type = ElementExtremeValue
    variable = stress_xx
    value_type = max
  []
  [stress_xx_min]
    type = ElementExtremeValue
    variable = stress_xx
    value_type = min
  []
  [stress_xy_max]
    type = ElementExtremeValue
    variable = stress_xy
    value_type = max
  []
  [stress_xy_min]
    type = ElementExtremeValue
    variable = stress_xy
    value_type = min
  []
  [hourglass_force_x_norm]
    type = NodalL2Norm
    variable = hourglass_force_x
  []
  [hourglass_force_y_norm]
    type = NodalL2Norm
    variable = hourglass_force_y
  []
  [hourglass_force_z_norm]
    type = NodalL2Norm
    variable = hourglass_force_z
  []
[]

[Executioner]
  type = Steady
  solve_type = NEWTON
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  nl_rel_tol = 1.0e-10
  nl_abs_tol = 1.0e-9
  [Quadrature]
    type = GAUSS
    element_order = CONSTANT
  []
[]

[Outputs]
  csv = true
[]
