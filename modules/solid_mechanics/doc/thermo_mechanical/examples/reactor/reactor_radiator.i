# Day 3 worked example: a space-reactor radiator panel.
# Waste heat enters at the heat-pipe root, conducts along the panel, and is
# rejected to deep space by surface-to-ambient (gray-body) radiation.
#
# Demonstrates: transient conduction with a volumetric source, and
# FunctionRadiativeBC (Stefan-Boltzmann surface-to-ambient) to a 3 K sink.

[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 48
    ny = 16
    xmax = 0.60 # radiator length (m)
    ymax = 0.10 # panel thickness (m)
  []
[]

[Variables]
  [temperature]
    initial_condition = 600 # K
  []
[]

[Kernels]
  [conduction]
    type = ADHeatConduction
    variable = temperature
  []
  [time]
    type = ADHeatConductionTimeDerivative
    variable = temperature
    specific_heat = specific_heat
    density_name = density
  []
  [waste_heat]
    type = BodyForce
    variable = temperature
    value = 2e5 # distributed waste heat (W/m^3)
  []
[]

[Materials]
  [aluminum]
    type = ADGenericConstantMaterial
    prop_names = 'thermal_conductivity specific_heat density'
    prop_values = '180 900 2700' # W/m-K, J/kg-K, kg/m^3
  []
[]

# Surface radiative heat flux q = eps*sigma*(T^4 - Tinf^4) [W/m^2] on the
# radiating faces. View it in ParaView (hotter -> radiates more, since flux ~ T^4)
# and integrate it (below) to see how much heat the panel is shedding to space.
[AuxVariables]
  [radiative_flux]
    order = FIRST
    family = LAGRANGE
  []
[]

[AuxKernels]
  [radiative_flux]
    type = ParsedAux
    variable = radiative_flux
    coupled_variables = 'temperature'
    constant_names = 'eps sigma Tinf'
    constant_expressions = '0.85 5.670374419e-8 3'
    expression = 'eps*sigma*(temperature^4 - Tinf^4)'
    boundary = 'top bottom' # only the radiating surfaces
    execute_on = 'INITIAL TIMESTEP_END'
  []
[]

[BCs]
  [radiating_surfaces]
    type = FunctionRadiativeBC
    variable = temperature
    boundary = 'top bottom'
    emissivity_function = '0.85' # surface emissivity
    Tinfinity = 3 # deep-space sink temperature (K)
  []
  [heat_pipe_root]
    type = DirichletBC
    variable = temperature
    boundary = left
    value = 600 # K
  []
[]

[Postprocessors]
  [peak_temperature]
    type = NodalExtremeValue
    variable = temperature
  []
  # Total heat rejected to deep space through the radiating faces -- "how much
  # heat leaves the system." (2-D, so this is per unit depth: W/m.)
  [radiated_power]
    type = SideIntegralVariablePostprocessor
    variable = radiative_flux
    boundary = 'top bottom'
  []
  # Peak surface flux: the hot region near the heat-pipe root radiates the most.
  [peak_radiative_flux]
    type = NodalExtremeValue
    variable = radiative_flux
    boundary = 'top bottom'
  []
[]

[Executioner]
  type = Transient
  num_steps = 30
  dt = 20
  solve_type = NEWTON
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  nl_rel_tol = 1e-7
[]

[Outputs]
  exodus = true
[]
