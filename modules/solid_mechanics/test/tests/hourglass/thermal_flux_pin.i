# Steady checkerboard demonstration: flux-driven conduction with a single
# pinned node (the standard pure-Neumann anchoring trick). On a regular mesh
# the one-point operator couples only diagonal neighbors, so the node graph
# splits into two independent sublattices; a full Dirichlet edge would anchor
# both parities, but a single pin anchors only ONE sublattice - the other
# keeps a free constant and the assembled operator has a true checkerboard
# null vector. Fully integrated (or stabilized) the same problem is well
# posed.
[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 8
    ny = 8
    elem_type = QUAD4
  []
  [pin_node]
    type = ExtraNodesetGenerator
    input = gen
    new_boundary = pin
    coord = '0 0'
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
  # balanced fluxes: in on the left, out on the right (compatible steady state)
  [influx]
    type = NeumannBC
    variable = T
    boundary = left
    value = 1
  []
  [outflux]
    type = NeumannBC
    variable = T
    boundary = right
    value = -1
  []
  [pin]
    type = DirichletBC
    variable = T
    boundary = pin
    value = 0
  []
[]

[Postprocessors]
  [T_max]
    type = NodalExtremeValue
    variable = T
  []
  [T_min]
    type = NodalExtremeValue
    variable = T
    value_type = min
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
  [Quadrature]
    type = GAUSS
    order = CONSTANT
  []
[]

[Outputs]
  csv = true
  exodus = true
[]
