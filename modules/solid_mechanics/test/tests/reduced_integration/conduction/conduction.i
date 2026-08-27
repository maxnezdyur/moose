[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 4
    ny = 4
  []
  [corners]
    type = ExtraNodesetGenerator
    input = gen
    new_boundary = 'corners'
    coord = '0 0 0; 1 0 0; 0 1 0; 1 1 0'
  []
  [distort]
    type = ParsedNodeTransformGenerator
    input = corners
    constant_names = 'amp'
    constant_expressions = '0' # cli_args raises amp to bend the grid into general quadrilaterals
    x_function = 'x + amp * (0.30 * x * y + 0.20 * y * y)'
    y_function = 'y + amp * (0.20 * x * y - 0.25 * x * x)'
  []
[]

[Functions]
  [affine_temp]
    type = ParsedFunction
    expression = '300.0 + 25.0 * x + 40.0 * y'
  []
  [source]
    type = ParsedFunction
    expression = '0'
  []
[]

[Variables]
  [temp]
    order = FIRST
    family = LAGRANGE
    initial_condition = 300.0
  []
[]

[AuxVariables]
  [hourglass_flux]
    order = FIRST
    family = LAGRANGE
  []
[]

[Kernels]
  [conduction]
    type = MatDiffusion
    variable = temp
    diffusivity = thermal_conductivity
  []
  [heat_source]
    type = BodyForce
    variable = temp
    function = source
  []
  [hourglass_temp]
    type = HourglassStabilization
    variable = temp
    stiffness_source = scalar_property
    scalar_stiffness = thermal_conductivity
    penalty = 0.1
    save_in = 'hourglass_flux'
  []
[]

[BCs]
  [prescribed_temp]
    type = FunctionDirichletBC
    variable = temp
    boundary = 'left right bottom top'
    function = affine_temp
  []
[]

[Materials]
  [k]
    type = GenericConstantMaterial
    prop_names = 'thermal_conductivity'
    prop_values = '12.0'
  []
[]

[Postprocessors]
  [temp_affine_nodal_error]
    type = NodalL2Error
    variable = temp
    function = affine_temp
  []
  [hourglass_flux_norm]
    type = NodalL2Norm
    variable = hourglass_flux
  []
  [temp_max]
    type = NodalExtremeValue
    variable = temp
    value_type = max
  []
  [temp_min]
    type = NodalExtremeValue
    variable = temp
    value_type = min
  []
  [temp_center]
    type = PointValue
    variable = temp
    point = '0.5 0.5 0'
  []
  [center_rel_err_vs_reference]
    type = ParsedPostprocessor
    expression = 'abs(temp_center - reference) / abs(reference)'
    pp_names = 'temp_center'
    constant_names = 'reference'
    constant_expressions = '300.0+25.0*0.5+40.0*0.5'
  []
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  petsc_options_iname = '-ksp_type -pc_type -pc_factor_shift_type'
  petsc_options_value = 'preonly   lu       NONE'
  nl_rel_tol = 1.0e-11
  nl_abs_tol = 1.0e-9
  nl_max_its = 10
  dt = 1.0
  dtmin = 1.0
  num_steps = 2
  [Quadrature]
    type = GAUSS
    element_order = CONSTANT
  []
[]

[Outputs]
  csv = true
[]
