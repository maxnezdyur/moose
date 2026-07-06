# Phase 5 MMS: T = sin(pi x) sin(pi y), k = 2.5 -> f = 2 pi^2 k T
N = 8

[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = ${N}
    ny = ${N}
    elem_type = QUAD4
  []
  [distort]
    type = ParsedNodeTransformGenerator
    input = gen
    # distortion scales with h so refinement keeps element shape quality
    x_function = 'x + ${fparse 0.45/N}*sin(3.1*y+0.7)*if(x*(1-x)>0.001,1,0)'
    y_function = 'y + ${fparse 0.45/N}*sin(2.7*x+0.3)*if(y*(1-y)>0.001,1,0)'
  []
[]

[Variables]
  [T]
  []
[]

[Functions]
  [exact]
    type = ParsedFunction
    expression = 'sin(pi*x)*sin(pi*y)'
  []
  [source]
    type = ParsedFunction
    expression = '2*pi*pi*2.5*sin(pi*x)*sin(pi*y)'
  []
[]

[Kernels]
  [cond]
    type = ADMatDiffusion
    variable = T
    diffusivity = k
  []
  [src]
    type = ADBodyForce
    variable = T
    function = source
  []
  [hg]
    type = HourglassCorrectionDiffusion
    variable = T
    diffusivity = k
  []
[]

[Materials]
  [k]
    type = ADGenericConstantMaterial
    prop_names = 'k'
    prop_values = 2.5
  []
[]

[BCs]
  [all]
    type = FunctionDirichletBC
    variable = T
    boundary = 'left right top bottom'
    function = exact
  []
[]

[Postprocessors]
  [L2]
    type = ElementL2Error
    variable = T
    function = exact
  []
  [H1]
    type = ElementH1SemiError
    variable = T
    function = exact
  []
[]

[Executioner]
  type = Steady
  solve_type = NEWTON
  nl_rel_tol = 1e-13
  [Quadrature]
    type = GAUSS
    order = CONSTANT
  []
[]

[Outputs]
  csv = true
[]
