[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 10
  xmax = 1
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

[Materials]
  [diffusivity_u]
    type = GenericConstantMaterial
    prop_names = D_u
    prop_values = 2.0
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
    value = 1
    preset = true
  []
[]

[AutoResidualTag]
[]

[AuxVariables]
  [res_diffusion_u]
  []
  [res_source_u]
  []
  [res_left_u]
  []
  [res_right_u]
  []
[]

[AuxKernels]
  [res_diffusion_u_aux]
    type = TagVectorAux
    variable = res_diffusion_u
    v = u
    vector_tag = residual_tag_diffusion_u
  []
  [res_source_u_aux]
    type = TagVectorAux
    variable = res_source_u
    v = u
    vector_tag = residual_tag_source_u
  []
  [res_left_u_aux]
    type = TagVectorAux
    variable = res_left_u
    v = u
    vector_tag = residual_tag_left_u
  []
  [res_right_u_aux]
    type = TagVectorAux
    variable = res_right_u
    v = u
    vector_tag = residual_tag_right_u
  []
[]

[Postprocessors]
  [res_norm_diffusion_u]
    type = ElementL2Norm
    variable = res_diffusion_u
    execute_on = 'TIMESTEP_END'
  []
  [res_norm_source_u]
    type = ElementL2Norm
    variable = res_source_u
    execute_on = 'TIMESTEP_END'
  []
  [res_norm_left_u]
    type = ElementL2Norm
    variable = res_left_u
    execute_on = 'TIMESTEP_END'
  []
  [res_norm_right_u]
    type = ElementL2Norm
    variable = res_right_u
    execute_on = 'TIMESTEP_END'
  []
  [total_residual]
    type = Residual
    residual_type = FINAL
    execute_on = 'TIMESTEP_END'
  []
[]

[Executioner]
  type = Steady
  solve_type = NEWTON
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
[]

[Outputs]
  csv = true
  execute_on = 'final'
[]
