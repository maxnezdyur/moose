# Generator for the inverse-problem "thermocouple" data.
# Runs the forward fuel-rod conduction problem with the TRUE heat-source
# magnitude and samples the temperature at four interior thermocouple
# locations. Those samples become the synthetic measurements the optimization
# tries to match (reactor_thermocouples.csv).

[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 40
    ny = 10
    xmax = 0.30 # rod length (m), axial = X
    ymax = 0.05 # rod radius (m)
  []
  coord_type = RZ
  rz_coord_axis = X
[]

[Variables/T][]

[Kernels]
  [conduction]
    type = HeatConduction
    variable = T
  []
  [source]
    type = BodyForce
    variable = T
    value = 8e6 # TRUE volumetric heat generation (W/m^3) we will try to recover
  []
[]

[Materials/k]
  type = GenericConstantMaterial
  prop_names = thermal_conductivity
  prop_values = 18
[]

[BCs/cooled_surface]
  type = DirichletBC
  variable = T
  boundary = top # outer radius (coolant)
  value = 350
[]

[Postprocessors]
  [tc1]
    type = PointValue
    variable = T
    point = '0.08 0.005 0'
  []
  [tc2]
    type = PointValue
    variable = T
    point = '0.15 0.020 0'
  []
  [tc3]
    type = PointValue
    variable = T
    point = '0.22 0.035 0'
  []
  [tc4]
    type = PointValue
    variable = T
    point = '0.15 0.045 0'
  []
[]

[Executioner]
  type = Steady
  solve_type = NEWTON
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
[]

[Outputs]
  csv = true
[]
