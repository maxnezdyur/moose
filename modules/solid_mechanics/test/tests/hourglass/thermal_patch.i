# Phase 4 gate: linear patch test on distorted quads at one-point quadrature
# WITH stabilization. Must recover T = 0.3 + 1.7x - 0.9y to machine precision
# with hourglass energy ~ 0 (gamma annihilates affine fields).
[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 4
    ny = 4
    elem_type = QUAD4
  []
  [distort]
    type = ParsedNodeTransformGenerator
    input = gen
    x_function = 'x + 0.07*sin(3.1*y+0.7)*if(x*(1-x)>0.001,1,0)'
    y_function = 'y + 0.07*sin(2.7*x+0.3)*if(y*(1-y)>0.001,1,0)'
  []
[]

[Variables]
  [T]
  []
[]

[Functions]
  [exact]
    type = ParsedFunction
    expression = '0.3 + 1.7*x - 0.9*y'
  []
[]

[Kernels]
  [cond]
    type = ADMatDiffusion
    variable = T
    diffusivity = k
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
  [err]
    type = NodalL2Error
    variable = T
    function = exact
  []
  [hg_energy]
    type = HourglassEnergy
    variable = T
    diffusivity = k
  []
[]

[Executioner]
  type = Steady
  solve_type = NEWTON
  nl_rel_tol = 1e-14
  [Quadrature]
    type = GAUSS
    order = CONSTANT
  []
[]

[Outputs]
  csv = true
[]
