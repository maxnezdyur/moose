# Phase 0/1 driver: one distorted QUAD4, conduction only, no BCs.
# Used to dump the assembled Jacobian and count zero eigenvalues:
#   full (2x2) integration -> exactly one zero eigenvalue (constant mode)
#   one-point integration  -> two zero eigenvalues (constant + hourglass)
[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 1
    ny = 1
    elem_type = QUAD4
  []
  # distort: move node (1,1) to (1.4, 0.8) and node (0,1) to (-0.2, 1.3)
  [move1]
    type = MoveNodeGenerator
    input = gen
    node_id = '2 3'
    new_position = '1.4 0.8 0 -0.2 1.3 0'
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
  # nonzero residual so SNES assembles the Jacobian (no-BC system is singular;
  # the run is only used to dump the matrix, divergence is expected)
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

[Problem]
  kernel_coverage_check = false
[]

[Executioner]
  type = Steady
  solve_type = NEWTON
[]

[Outputs]
[]
