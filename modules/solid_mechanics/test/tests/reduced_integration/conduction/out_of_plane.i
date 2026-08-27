# A QUAD4 mesh rotated out of the x-y plane. HourglassStabilization builds its reference geometry
# matrix from the leading dim() global coordinate components, so this orientation is rejected
# rather than silently mis-projected.
[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 2
    ny = 2
  []
  [rotate]
    type = TransformGenerator
    input = gen
    transform = ROTATE
    vector_value = '0 90 0'
  []
[]

[Variables]
  [temp]
    order = FIRST
    family = LAGRANGE
    initial_condition = 300.0
  []
[]

[Kernels]
  [conduction]
    type = MatDiffusion
    variable = temp
    diffusivity = thermal_conductivity
  []
  [hourglass_temp]
    type = HourglassStabilization
    variable = temp
    stiffness_source = scalar_property
    scalar_stiffness = thermal_conductivity
  []
[]

[BCs]
  [fixed]
    type = DirichletBC
    variable = temp
    boundary = 'left right'
    value = 300.0
  []
[]

[Materials]
  [k]
    type = GenericConstantMaterial
    prop_names = 'thermal_conductivity'
    prop_values = '12.0'
  []
[]

[Executioner]
  type = Steady
  solve_type = NEWTON
  [Quadrature]
    type = GAUSS
    element_order = CONSTANT
  []
[]

[Outputs]
[]
