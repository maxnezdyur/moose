# Phase 6: k(T) = 2 + 0.5 T, one-point quadrature + stabilization with the
# consistent d(eps)/dT tangent. Gates: FD-vs-analytic Jacobian agreement and
# quadratic Newton convergence.
[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 6
    ny = 6
    elem_type = QUAD4
  []
  [distort]
    type = ParsedNodeTransformGenerator
    input = gen
    x_function = 'x + 0.05*sin(3.1*y+0.7)*if(x*(1-x)>0.001,1,0)'
    y_function = 'y + 0.05*sin(2.7*x+0.3)*if(y*(1-y)>0.001,1,0)'
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
    value = 5
  []
  [hg]
    type = HourglassCorrectionDiffusion
    variable = T
    diffusivity = k
  []
[]

[Materials]
  [k]
    type = ADParsedMaterial
    property_name = k
    expression = '2.0 + 0.5*T'
    coupled_variables = 'T'
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

[Postprocessors]
  [T_avg]
    type = ElementAverageValue
    variable = T
  []
  [T_max]
    type = NodalExtremeValue
    variable = T
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
  nl_rel_tol = 1e-13
  [Quadrature]
    type = GAUSS
    order = CONSTANT
  []
[]

[Outputs]
  csv = true
[]
