[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 2
    ny = 2
    nz = 2
  []
[]

[Functions]
  [heat_source]
    type = ParsedFunction
    expression = '2.0e4 * (1.0 + 2.0 * x * y * z)'
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
  [temp_hourglass_flux]
    order = FIRST
    family = LAGRANGE
  []
[]

[Physics]
  [SolidMechanics]
    [QuasiStatic]
      displacements = 'disp_x disp_y disp_z'
      [all]
        displacements = 'disp_x disp_y disp_z'
        temperature = temp
        strain = SMALL
        add_variables = true
        reduced_integration = true
        hourglass_penalty = 0.1
        eigenstrain_names = 'thermal_eigenstrain'
        generate_output = 'vonmises_stress'
      []
    []
  []
[]

[Kernels]
  [conduction]
    type = MatDiffusion
    variable = temp
    diffusivity = thermal_conductivity
  []
  [source]
    type = BodyForce
    variable = temp
    function = heat_source
  []
  [hourglass_temp]
    type = HourglassStabilization
    variable = temp
    stiffness_source = scalar_property
    scalar_stiffness = thermal_conductivity
    penalty = 0.1
    save_in = 'temp_hourglass_flux'
  []
[]

[BCs]
  [sym_x]
    type = DirichletBC
    variable = disp_x
    boundary = left
    value = 0.0
  []
  [sym_y]
    type = DirichletBC
    variable = disp_y
    boundary = bottom
    value = 0.0
  []
  [sym_z]
    type = DirichletBC
    variable = disp_z
    boundary = back
    value = 0.0
  []
  [cold]
    type = DirichletBC
    variable = temp
    boundary = left
    value = 300.0
  []
  [hot]
    type = DirichletBC
    variable = temp
    boundary = right
    value = 340.0
  []
[]

[Materials]
  [elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 2.0e5
    poissons_ratio = 0.3
  []
  [thermal_eigenstrain]
    type = ComputeThermalExpansionEigenstrain
    eigenstrain_name = thermal_eigenstrain
    temperature = temp
    stress_free_temperature = 300.0
    thermal_expansion_coeff = 1.3e-5
  []
  [stress]
    type = ComputeLinearElasticStress
  []
  [k]
    type = GenericConstantMaterial
    prop_names = 'thermal_conductivity'
    prop_values = '12.0'
  []
[]

[Postprocessors]
  [temp_hourglass_flux_norm]
    type = NodalL2Norm
    variable = temp_hourglass_flux
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
  [disp_x_max]
    type = NodalExtremeValue
    variable = disp_x
    value_type = max
  []
  [disp_y_max]
    type = NodalExtremeValue
    variable = disp_y
    value_type = max
  []
  [disp_z_max]
    type = NodalExtremeValue
    variable = disp_z
    value_type = max
  []
  [vonmises_avg]
    type = ElementAverageValue
    variable = vonmises_stress
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
