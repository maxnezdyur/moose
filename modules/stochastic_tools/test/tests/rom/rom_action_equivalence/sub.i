S = 10
D = 10
L = 5

[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 50
  xmax = ${L}
[]

[Variables]
  [u]
  []
[]

[Kernels]
  [diffusion_u]
    type = MatDiffusion
    variable = u
    diffusivity = D_u
  []
  [source_u]
    type = BodyForce
    variable = u
    value = 1.0
  []
[]

[Functions]
  [du]
    type = ParsedFunction
    expression = 'D * D * x + 1'
    symbol_names = D
    symbol_values = ${D}
  []
[]

[Materials]
  [diffusivity_u]
    type = GenericFunctionMaterial
    prop_names = D_u
    prop_values = du
  []
[]

[BCs]
  [left_u]
    type = DirichletBC
    variable = u
    boundary = left
    value = 0
    preset = true
  []
  [right_u]
    type = DirichletBC
    variable = u
    boundary = right
    value = ${S}
    preset = true
  []
[]

[Executioner]
  type = Steady
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_factor_shift_type -pc_factor_mat_solver_type'
  petsc_options_value = 'lu       NONZERO               strumpack'
  nl_abs_tol = 1e-8
  nl_rel_tol = 1e-18
[]

[Controls]
  [stochastic]
    type = SamplerReceiver
  []
[]
