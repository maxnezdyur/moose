# Day 2 worked example: heat conduction in a reactor fuel-element-like rod
# (axisymmetric, RZ about the X axis). Internal (fission) heat generation is
# carried out through the cooled outer surface, giving the classic radial
# temperature profile: hot centerline, cool surface.

[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 60
    ny = 16
    xmax = 0.30 # rod length (m), axial = X
    ymax = 0.05 # rod radius (m)
  []
  coord_type = RZ
  rz_coord_axis = X
[]

[Variables]
  [temperature]
    initial_condition = 350 # start near coolant temperature (K)
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
  [fission_heat]
    type = BodyForce
    variable = temperature
    value = 8e6 # volumetric heat generation (W/m^3)
  []
[]

[Materials]
  [steel]
    type = ADGenericConstantMaterial
    prop_names = 'thermal_conductivity specific_heat density'
    prop_values = '18 466 8000' # W/m-K, J/kg-K, kg/m^3
  []
[]

[BCs]
  [cooled_surface]
    type = DirichletBC
    variable = temperature
    boundary = top # outer radius, in contact with coolant
    value = 350 # (K)
  []
[]

[Postprocessors]
  [peak_temperature]
    type = NodalExtremeValue
    variable = temperature
  []
[]

[Executioner]
  type = Transient
  num_steps = 50
  dt = 10
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
[]
