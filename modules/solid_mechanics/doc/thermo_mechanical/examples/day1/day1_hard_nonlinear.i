# =============================================================================
# DAY 1 - A GENUINELY NONLINEAR STEADY PROBLEM
#
# The other steady example converges in ONE Newton iteration because it is
# linear. This one is steady too, but it is *nonlinear*: both the conductivity
# and a self-heating source depend on temperature (built with AD ParsedMaterials),
# so MOOSE's Newton solver must take many nonlinear iterations to converge.
# Watch the "N Nonlinear" count in the console - that is the whole point.
#
# Run:  combined-opt -i day1_hard_nonlinear.i
# =============================================================================

[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 24
    ny = 24
  []
[]

[Variables]
  [T]
    initial_condition = 300 # a cold guess, far from the hot wall - makes Newton work
  []
[]

[Kernels]
  # -div(k(T) grad T) = q(T) ; both terms use AD so the Jacobian is exact
  [conduction]
    type = ADHeatConduction
    variable = T
  []
  [self_heating]
    type = ADMatHeatSource
    variable = T
    material_property = volumetric_heat
  []
[]

[Materials]
  # Conductivity rises sharply with temperature (radiative-like) -> nonlinear
  [k]
    type = ADParsedMaterial
    property_name = thermal_conductivity
    coupled_variables = 'T'
    constant_names = 'k0 a'
    constant_expressions = '2.0 8.0e-8'
    expression = 'k0 + a*T^3'
  []
  # Self-heating source grows with temperature (kept sub-critical so it converges)
  [q]
    type = ADParsedMaterial
    property_name = volumetric_heat
    coupled_variables = 'T'
    constant_names = 'q0 Tc'
    constant_expressions = '2.0e4 300.0' # tuned sub-critical: converges in ~12 nonlinear iterations
    expression = 'q0*exp((T - 300.0)/Tc)'
  []
[]

[BCs]
  [hot]
    type = ADDirichletBC
    variable = T
    boundary = left
    value = 800
  []
  [cold]
    type = ADDirichletBC
    variable = T
    boundary = right
    value = 300
  []
[]

[Executioner]
  type = Steady
  solve_type = NEWTON
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-9
  nl_max_its = 50
[]

[Postprocessors]
  [max_temperature]
    type = NodalExtremeValue
    variable = T
  []
[]

[Outputs]
  exodus = true
[]
