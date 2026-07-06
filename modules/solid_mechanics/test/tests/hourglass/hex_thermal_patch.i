# Phase 8 patch test: linear temperature field on distorted HEX8 elements at
# one-point quadrature with stabilization; machine-precision recovery and
# vanishing stabilization energy.
[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 3
    ny = 3
    nz = 3
    elem_type = HEX8
  []
  [distort]
    type = ParsedNodeTransformGenerator
    input = gen
    x_function = 'x + 0.06*sin(3.1*y+0.7)*sin(2.2*z+0.4)*if(x*(1-x)>0.001,1,0)'
    y_function = 'y + 0.06*sin(2.7*x+0.3)*sin(1.9*z+0.8)*if(y*(1-y)>0.001,1,0)'
    z_function = 'z + 0.06*sin(2.3*x+0.5)*sin(2.9*y+0.2)*if(z*(1-z)>0.001,1,0)'
  []
[]

[Variables]
  [T]
  []
[]

[Functions]
  [exact]
    type = ParsedFunction
    expression = '0.3 + 1.7*x - 0.9*y + 0.6*z'
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
    boundary = 'left right top bottom front back'
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
  [gamma_ortho]
    type = HourglassGammaOrthogonality
    execute_on = 'INITIAL'
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
