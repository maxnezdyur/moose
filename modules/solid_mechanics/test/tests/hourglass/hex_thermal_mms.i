# Phase 8 MMS: T = sin(pi x) sin(pi y) sin(pi z), k = 2.5
N = 4

[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 3
    nx = ${N}
    ny = ${N}
    nz = ${N}
    elem_type = HEX8
  []
  [distort]
    type = ParsedNodeTransformGenerator
    input = gen
    x_function = 'x + ${fparse 0.35/N}*sin(3.1*y+0.7)*sin(2.2*z+0.4)*if(x*(1-x)>0.001,1,0)'
    y_function = 'y + ${fparse 0.35/N}*sin(2.7*x+0.3)*sin(1.9*z+0.8)*if(y*(1-y)>0.001,1,0)'
    z_function = 'z + ${fparse 0.35/N}*sin(2.3*x+0.5)*sin(2.9*y+0.2)*if(z*(1-z)>0.001,1,0)'
  []
[]

[Variables]
  [T]
  []
[]

[Functions]
  [exact]
    type = ParsedFunction
    expression = 'sin(pi*x)*sin(pi*y)*sin(pi*z)'
  []
  [source]
    type = ParsedFunction
    expression = '3*pi*pi*2.5*sin(pi*x)*sin(pi*y)*sin(pi*z)'
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
    boundary = 'left right top bottom front back'
    function = exact
  []
[]

[Postprocessors]
  [L2]
    type = ElementL2Error
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
