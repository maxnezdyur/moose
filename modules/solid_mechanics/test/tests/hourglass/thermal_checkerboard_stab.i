# Phase 1b: the failure at mesh scale. 8x8 distorted quads, one-point
# quadrature, NO stabilization: the assembled conduction operator carries
# hourglass null-space content and the solution checkerboards (or the direct
# solve hits zero pivots). Run with the stabilization kernel to see the cure.
[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 8
    ny = 8
    elem_type = QUAD4
  []
  # skew the interior so gamma != h anywhere (a wrong projection hides on
  # rectangular meshes)
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

[Kernels]
  [cond]
    type = ADMatDiffusion
    variable = T
    diffusivity = k
  []
  [src]
    type = ADBodyForce
    variable = T
    value = 1
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
  [left]
    type = DirichletBC
    variable = T
    boundary = left
    value = 0
  []
  [right]
    type = DirichletBC
    variable = T
    boundary = right
    value = 1
  []
[]

[Executioner]
  type = Steady
  solve_type = NEWTON
  [Quadrature]
    type = GAUSS
    order = CONSTANT
  []
[]

[Outputs]
  exodus = true
[]
