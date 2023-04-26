[Mesh]
  [square]
    type = GeneratedMeshGenerator
    nx = 2
    ny = 2
    dim = 2
  []
  uniform_refine = 4
[]

[Variables]
  [u]
    order = FIRST
    family = LAGRANGE
  []
[]

[Kernels]
  [diff]
    type = Diffusion
    variable = u
  []
[]

[DiracKernels]
  [reporter_point_source]
    type = ReporterPointSource
    variable = u
    value_name = 'reporterData1/u'
    point_name = 'reporterData1/pts'
  []
[]

[BCs]
  [left]
    type = DirichletBC
    variable = u
    boundary = 3
    value = 0
  []
  [right]
    type = DirichletBC
    variable = u
    boundary = 1
    value = 1
  []
[]


[Reporters]
  [reporterData1]
    type = ConstantReporter
    real_vector_names = 'u'
    real_vector_values = '5 5 5'
    point_vector_names = 'pts'
    point_vector_values = '0.2 0.3 0 0.2 0.8 0 0.0 0.0 0.0'
  []

[]

[Executioner]
  type = Steady
  solve_type = 'PJFNK'
[]

[Outputs]
  exodus = true
[]
