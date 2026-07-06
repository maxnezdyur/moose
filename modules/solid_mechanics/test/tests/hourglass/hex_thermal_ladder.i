# Phase 8 driver: single distorted HEX8, conduction. Reused for eigenvalue
# dumps (with/without stabilization) via cli overrides.
[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 1
    ny = 1
    nz = 1
    elem_type = HEX8
  []
  [move]
    type = MoveNodeGenerator
    input = gen
    node_id = '6 3 4'
    new_position = '1.35 0.85 1.2  -0.15 1.25 0.1  0.1 -0.2 0.9'
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
[]

[Materials]
  [k]
    type = ADGenericConstantMaterial
    prop_names = 'k'
    prop_values = 2.5
  []
[]

[Problem]
  kernel_coverage_check = false
[]

[Postprocessors]
  [gamma_ortho]
    type = HourglassGammaOrthogonality
    execute_on = 'INITIAL'
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
  csv = true
[]
