# The decisive failure/cure demo: transient conduction from a checkerboard
# initial condition (pure hourglass content). Physical diffusion must kill it;
# at one-point quadrature without stabilization the mode has ZERO stiffness
# and persists forever. With stabilization it decays.
[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 8
    ny = 8
    elem_type = QUAD4
  []
[]

[Variables]
  [T]
  []
[]

[Functions]
  [checker_fn]
    type = ParsedFunction
    # +/-1 node-parity checkerboard on the 8x8 grid (h = 1/8)
    expression = 'cos(pi*floor(x*8+0.5))*cos(pi*floor(y*8+0.5))'
  []
[]

[ICs]
  [checker]
    type = FunctionIC
    variable = T
    function = checker_fn
  []
[]

[Kernels]
  [dt]
    type = ADTimeDerivative
    variable = T
  []
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

[Postprocessors]
  [hg_energy]
    type = HourglassEnergy
    variable = T
    diffusivity = k
    execute_on = 'INITIAL TIMESTEP_END'
  []
  [Tmax]
    type = NodalExtremeValue
    variable = T
    execute_on = 'INITIAL TIMESTEP_END'
  []
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  dt = 0.002
  num_steps = 25
  [Quadrature]
    type = GAUSS
    order = CONSTANT
  []
[]

[Outputs]
  csv = true
[]
