[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
[]

[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 3
    ny = 3
    nz = 3
    elem_type = HEX8
  []
  [distort]
    type = ParsedNodeTransformGenerator
    input = gmg
    x_function = 'x + 0.08 * sin(3.0 * y) + 0.05 * sin(2.0 * z)'
    y_function = 'y + 0.06 * sin(4.0 * x) + 0.05 * sin(3.0 * z)'
    z_function = 'z + 0.05 * sin(2.0 * x) + 0.06 * sin(3.0 * y)'
  []
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
  [disp_z]
  []
[]

[AuxVariables]
  [hourglass_force_x]
  []
  [hourglass_force_y]
  []
  [hourglass_force_z]
  []
  [stress_norm]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[Functions]
  [rot_x]
    type = ParsedFunction
    symbol_names = 'k'
    symbol_values = '0.5773502691896258'
    expression = '(cos(pi / 3.0 * t) + (1.0 - cos(pi / 3.0 * t)) / 3.0 - 1.0) * x + ((1.0 - cos(pi / 3.0 * t)) / 3.0 - k * sin(pi / 3.0 * t)) * y + ((1.0 - cos(pi / 3.0 * t)) / 3.0 + k * sin(pi / 3.0 * t)) * z'
  []
  [rot_y]
    type = ParsedFunction
    symbol_names = 'k'
    symbol_values = '0.5773502691896258'
    expression = '((1.0 - cos(pi / 3.0 * t)) / 3.0 + k * sin(pi / 3.0 * t)) * x + (cos(pi / 3.0 * t) + (1.0 - cos(pi / 3.0 * t)) / 3.0 - 1.0) * y + ((1.0 - cos(pi / 3.0 * t)) / 3.0 - k * sin(pi / 3.0 * t)) * z'
  []
  [rot_z]
    type = ParsedFunction
    symbol_names = 'k'
    symbol_values = '0.5773502691896258'
    expression = '((1.0 - cos(pi / 3.0 * t)) / 3.0 - k * sin(pi / 3.0 * t)) * x + ((1.0 - cos(pi / 3.0 * t)) / 3.0 + k * sin(pi / 3.0 * t)) * y + (cos(pi / 3.0 * t) + (1.0 - cos(pi / 3.0 * t)) / 3.0 - 1.0) * z'
  []
[]

[Kernels]
  [stress_x]
    type = StressDivergenceTensors
    variable = disp_x
    component = 0
    use_displaced_mesh = true
  []
  [stress_y]
    type = StressDivergenceTensors
    variable = disp_y
    component = 1
    use_displaced_mesh = true
  []
  [stress_z]
    type = StressDivergenceTensors
    variable = disp_z
    component = 2
    use_displaced_mesh = true
  []
  [hourglass_x]
    type = HourglassStabilization
    variable = disp_x
    save_in = hourglass_force_x
  []
  [hourglass_y]
    type = HourglassStabilization
    variable = disp_y
    save_in = hourglass_force_y
  []
  [hourglass_z]
    type = HourglassStabilization
    variable = disp_z
    save_in = hourglass_force_z
  []
[]

[AuxKernels]
  [stress_norm]
    type = RankTwoScalarAux
    variable = stress_norm
    rank_two_tensor = stress
    scalar_type = L2norm
  []
[]

[BCs]
  [rot_x]
    type = FunctionDirichletBC
    variable = disp_x
    boundary = 'left right bottom top front back'
    function = rot_x
  []
  [rot_y]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = 'left right bottom top front back'
    function = rot_y
  []
  [rot_z]
    type = FunctionDirichletBC
    variable = disp_z
    boundary = 'left right bottom top front back'
    function = rot_z
  []
[]

[Materials]
  [elasticity]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 1.0
    poissons_ratio = 0.3
  []
  [strain]
    type = ComputeFiniteStrain
    decomposition_method = EigenSolution # exact polar decomposition, so a rigid rotation gives an identically zero strain increment
  []
  [stress]
    type = ComputeFiniteStrainElasticStress
  []
[]

[Postprocessors]
  [max_stress]
    type = ElementExtremeValue
    variable = stress_norm
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
  [disp_l2_x]
    type = NodalL2Norm
    variable = disp_x
  []
  [disp_l2_y]
    type = NodalL2Norm
    variable = disp_y
  []
  [disp_l2_z]
    type = NodalL2Norm
    variable = disp_z
  []
[]

[Preconditioning]
  [smp]
    type = SMP
    full = true
  []
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  line_search = none
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  nl_rel_tol = 1e-12
  nl_abs_tol = 1e-13
  dt = 0.25
  num_steps = 4
  [Quadrature]
    type = GAUSS
    element_order = CONSTANT
  []
[]

[Outputs]
  csv = true
[]
