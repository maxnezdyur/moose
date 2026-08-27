[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 4
    ny = 4
    elem_type = QUAD4
  []
  [distort]
    type = ParsedNodeTransformGenerator
    input = gmg
    x_function = 'x + 0.09 * sin(3.0 * y)'
    y_function = 'y + 0.07 * sin(4.0 * x)'
  []
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
[]

[AuxVariables]
  [hourglass_force_x]
  []
  [hourglass_force_y]
  []
  [stress_norm]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[Functions]
  [rot_x]
    type = ParsedFunction
    expression = '(cos(pi / 3.0 * t) - 1.0) * x - sin(pi / 3.0 * t) * y'
  []
  [rot_y]
    type = ParsedFunction
    expression = 'sin(pi / 3.0 * t) * x + (cos(pi / 3.0 * t) - 1.0) * y'
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
    boundary = 'left right bottom top'
    function = rot_x
  []
  [rot_y]
    type = FunctionDirichletBC
    variable = disp_y
    boundary = 'left right bottom top'
    function = rot_y
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
  [disp_l2_x]
    type = NodalL2Norm
    variable = disp_x
  []
  [disp_l2_y]
    type = NodalL2Norm
    variable = disp_y
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
