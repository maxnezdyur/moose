# Phase 2 gate: gamma orthogonality (gamma.1 = gamma.x = gamma.y = 0) at
# machine precision on DISTORTED quads. A wrong projection passes on squares
# and fails here.
[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 8
    ny = 8
    elem_type = QUAD4
  []
  [distort]
    type = ParsedNodeTransformGenerator
    input = gen
    x_function = 'x + 0.06*sin(3.1*y+0.7)*if(x*(1-x)>0.001,1,0)'
    y_function = 'y + 0.06*sin(2.7*x+0.3)*if(y*(1-y)>0.001,1,0)'
  []
[]

[Variables]
  [T]
  []
[]

[Problem]
  kernel_coverage_check = false
  solve = false
[]

[Postprocessors]
  [gamma_ortho]
    type = HourglassGammaOrthogonality
    execute_on = 'INITIAL'
  []
[]

[Executioner]
  type = Steady
[]

[Outputs]
  csv = true
  execute_on = 'INITIAL'
[]
