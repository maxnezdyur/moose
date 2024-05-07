speed = 1.0
mag = 2.0
time = 3.0
r = 0.3

[Mesh]
  [mesh]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 125
    ny = 25
    xmin = -2
    xmax = 3
    ymin = -0.5
    ymax = 0.5
  []

[]

[AuxVariables]
  [nn_T]
  []
  [diff]
  []
[]

[AuxKernels]
  [PINN]
    type = LibtorchScriptNeuralNetAux
    variable = nn_T
    filename = heat_pinn.pth
    output_name = u
    parameter_names = 'speed magnitude r'
    parameter_values = '${speed}  ${mag} ${r}'
    execute_on = 'INITIAL TIMESTEP_BEGIN'
  []
  [diff]
    type = ParsedAux
    variable = diff
    expression = 'nn_T - T'
    coupled_variables = 'nn_T T'
  []
[]
[Variables]
  [T]
  []
[]

[Kernels]
  [conduction]
    type = Diffusion
    variable = T
  []
  [time]
    type = TimeDerivative
    variable = T
  []
  [source]
    type = BodyForce
    variable = T
    function = '2 * ${mag} * exp(-((x - ${speed} * t) ^ 2 + (y) ^ 2) / ${r}^2) / (pi * ${r}^2)'
  []
[]

[BCs]
  [zero_bottom]
    type = DirichletBC
    variable = T
    value = 0
    boundary = bottom
  []
[]

[Executioner]
  type = Transient
  scheme = bdf2
  solve_type = NEWTON
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  dt = '${fparse time/10.0}'
  end_time = ${time}
[]

[Postprocessors]
  [l2_error]
    type = ElementL2Difference
    variable = T
    other_variable = nn_T
  []
  [T_norm]
    type = ElementL2Norm
    variable = T
  []
  [l2_rel_error]
    type = ParsedPostprocessor
    expression = 'l2_error / T_norm * 100'
    pp_names = 'l2_error T_norm'
    execution_order_group = 1
  []
[]

[Outputs]
  exodus = true
  csv = true
[]

