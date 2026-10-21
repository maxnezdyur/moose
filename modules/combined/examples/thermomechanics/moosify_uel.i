# Coupled thermomechanical two-block model of the UEL problem in main.pdf.
# The sidesets are renamed to the main.pdf faces (Table 1 and Table 3):
#   main.pdf face  plane       generated name  role
#   Bottom         z = -0.04   back            disp_z = 0
#   Top            z =  0.04   front           pressure and convection
#   Left           x = -0.09   left            disp_x = 0
#   Right          x =  0.09   right           plane remains plane, tie on disp_x
#   Front          y = -0.06   bottom          plane remains plane, tie on disp_y
#   Back           y =  0.06   top             disp_y = 0
# elastic_incremental is a placeholder UMAT until the NNL viscoplastic UMAT is available.
# It writes no STATEV entry, so the Z aux (state_var[0]) stays 0 with the placeholder.

[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
  temperature = temperature
[]

[Mesh]
  [generated]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 9
    ny = 6
    nz = 4
    xmin = -0.09
    xmax = 0.09
    ymin = -0.06
    ymax = 0.06
    zmin = -0.04
    zmax = 0.04
  []
  [block_a]
    type = SubdomainBoundingBoxGenerator
    input = generated
    block_id = 1
    block_name = block_a
    bottom_left = '-0.09 -0.06 -0.04'
    top_right = '0.09 0.06 0'
  []
  [block_b]
    type = RenameBlockGenerator
    input = block_a
    old_block = 0
    new_block = block_b
  []
  [pdf_faces]
    type = RenameBoundaryGenerator
    input = block_b
    old_boundary = 'back bottom top front'
    new_boundary = 'pdf_bottom pdf_front pdf_back pdf_top'
  []
  [faces]
    type = RenameBoundaryGenerator
    input = pdf_faces
    old_boundary = 'pdf_bottom pdf_front pdf_back pdf_top'
    new_boundary = 'bottom front back top'
  []
[]

[Variables]
  [temperature]
    initial_condition = 20
  []
[]

[AuxVariables]
  [phi]
    initial_condition = 0
  []
  [Z]
    family = MONOMIAL
    order = CONSTANT
  []
  [heat_flux_x]
    family = MONOMIAL
    order = CONSTANT
  []
  [heat_flux_y]
    family = MONOMIAL
    order = CONSTANT
  []
  [heat_flux_z]
    family = MONOMIAL
    order = CONSTANT
  []
  [reaction_x]
  []
  [reaction_y]
  []
  [reaction_z]
  []
[]

[AuxKernels]
  [phi]
    type = FunctionAux
    variable = phi
    function = phi_function
    execute_on = 'INITIAL TIMESTEP_BEGIN'
  []
  [Z]
    type = MaterialStdVectorAux
    variable = Z
    property = state_var
    index = 0
    execute_on = timestep_end
  []
  [heat_flux_x]
    type = DiffusionFluxAux
    variable = heat_flux_x
    diffusion_variable = temperature
    diffusivity = thermal_conductivity
    component = x
  []
  [heat_flux_y]
    type = DiffusionFluxAux
    variable = heat_flux_y
    diffusion_variable = temperature
    diffusivity = thermal_conductivity
    component = y
  []
  [heat_flux_z]
    type = DiffusionFluxAux
    variable = heat_flux_z
    diffusion_variable = temperature
    diffusivity = thermal_conductivity
    component = z
  []
[]

[Functions]
  [phi_function]
    type = ParsedFunction
    # x is in m, so the main.pdf factor 1 - x^2/81 (x in cm) is 1 - x^2/0.0081.
    # The source shuts off at t_off = 5e6 s, the main.pdf section 6 formula.
    expression = 'if(t <= 1000, (3000 - 2.9 * t) * (1 - x^2 / 0.0081), if(t < 5e6, 100 * (1 - x^2 / 0.0081), 0))'
  []
  [pressure_function]
    type = ParsedFunction
    expression = 'if(t < 100000, -1e5, 0)'
  []
  [ambient_temperature]
    type = ParsedFunction
    expression = 'if(t < 100000, 500, 20)'
  []
  [heat_transfer_coefficient]
    type = ParsedFunction
    expression = '200 * sin(pi * t / 1e6)'
  []
[]

[Physics/SolidMechanics/QuasiStatic]
  [all]
    add_variables = true
    strain = FINITE
    incremental = true
    eigenstrain_names = thermal_expansion
    save_in = 'reaction_x reaction_y reaction_z'
    generate_output = 'stress_xx stress_yy stress_zz stress_xy stress_xz stress_yz strain_xx strain_yy strain_zz'
  []
[]

[Kernels]
  [heat_time]
    type = ADHeatConductionTimeDerivative
    variable = temperature
    density_name = density
    specific_heat = specific_heat
  []
  [heat_conduction]
    type = ADHeatConduction
    variable = temperature
    thermal_conductivity = thermal_conductivity
  []
  [heat_source]
    type = ADMatHeatSource
    variable = temperature
    material_property = volumetric_heat
    block = block_a
  []
[]

[BCs]
  [bottom_z]
    type = DirichletBC
    variable = disp_z
    boundary = bottom
    value = 0
  []
  [left_x]
    type = DirichletBC
    variable = disp_x
    boundary = left
    value = 0
  []
  [back_y]
    type = DirichletBC
    variable = disp_y
    boundary = back
    value = 0
  []
  [top_pressure]
    type = FunctionNeumannBC
    variable = disp_z
    boundary = top
    function = pressure_function
  []
  [top_convection]
    type = ADConvectiveHeatFluxBC
    variable = temperature
    boundary = top
    T_infinity = ambient_temperature
    heat_transfer_coefficient = heat_transfer_coefficient
  []
[]

[Constraints]
  [right_prp]
    type = EqualValueBoundaryConstraint
    variable = disp_x
    secondary = right
    formulation = rows
  []
  [front_prp]
    type = EqualValueBoundaryConstraint
    variable = disp_y
    secondary = front
    formulation = rows
  []
[]

[Materials]
  [block_a_thermal]
    type = ADGenericConstantMaterial
    block = block_a
    prop_names = 'density specific_heat thermal_conductivity'
    prop_values = '8000 500 80'
  []
  [block_b_thermal]
    type = ADGenericConstantMaterial
    block = block_b
    prop_names = 'density specific_heat thermal_conductivity volumetric_heat'
    prop_values = '3000 800 200 0'
  []
  [block_a_heat_source]
    type = ADParsedMaterial
    block = block_a
    property_name = volumetric_heat
    coupled_variables = phi
    expression = 'phi^2'
  []
  [top_convection_properties]
    type = ADGenericFunctionMaterial
    boundary = top
    prop_names = 'ambient_temperature heat_transfer_coefficient'
    prop_values = 'ambient_temperature heat_transfer_coefficient'
  []
  [block_a_thermal_expansion]
    type = ComputeThermalExpansionEigenstrain
    block = block_a
    temperature = temperature
    thermal_expansion_coeff = 1e-6
    stress_free_temperature = 20
    eigenstrain_name = thermal_expansion
  []
  [block_a_umat]
    type = AbaqusUMATStress
    block = block_a
    constant_properties = '2e11 0.3'
    plugin = '../../../solid_mechanics/test/plugins/elastic_incremental'
    num_state_vars = 1
    temperature = temperature
    external_fields = phi
    use_one_based_indexing = true
  []
  [block_b_thermal_expansion]
    type = ComputeThermalExpansionEigenstrain
    block = block_b
    temperature = temperature
    thermal_expansion_coeff = 5e-6
    stress_free_temperature = 20
    eigenstrain_name = thermal_expansion
  []
  [block_b_umat]
    type = AbaqusUMATStress
    block = block_b
    constant_properties = '4e11 0.15'
    plugin = '../../../solid_mechanics/test/plugins/elastic_incremental'
    num_state_vars = 1
    temperature = temperature
    external_fields = phi
    use_one_based_indexing = true
  []
[]

[UserObjects]
  [analysis_steps]
    type = AnalysisStepUserObject
    step_start_times = '0 500 100000'
  []
[]

[Postprocessors]
  [top_reaction]
    type = NodalSum
    variable = reaction_z
    boundary = top
  []
  [bottom_reaction]
    type = NodalSum
    variable = reaction_z
    boundary = bottom
  []
  [left_reaction]
    type = NodalSum
    variable = reaction_x
    boundary = left
  []
  [right_reaction]
    type = NodalSum
    variable = reaction_x
    boundary = right
  []
  [front_reaction]
    type = NodalSum
    variable = reaction_y
    boundary = front
  []
  [back_reaction]
    type = NodalSum
    variable = reaction_y
    boundary = back
  []
  [top_conductive_rate]
    type = ADSideDiffusiveFluxIntegral
    variable = temperature
    diffusivity = thermal_conductivity
    boundary = top
  []
  [top_convective_rate]
    type = ADConvectiveHeatTransferSideIntegral
    T_fluid = ambient_temperature
    htc = heat_transfer_coefficient
    T_solid = temperature
    boundary = top
  []
  [top_heat_loss]
    type = TimeIntegratedPostprocessor
    value = top_convective_rate
    time_integration_scheme = implicit-euler
  []
  [right_disp_x_max]
    type = NodalExtremeValue
    variable = disp_x
    boundary = right
    value_type = max
  []
  [right_disp_x_min]
    type = NodalExtremeValue
    variable = disp_x
    boundary = right
    value_type = min
  []
  [right_spread]
    type = DifferencePostprocessor
    value1 = right_disp_x_max
    value2 = right_disp_x_min
  []
  [front_disp_y_max]
    type = NodalExtremeValue
    variable = disp_y
    boundary = front
    value_type = max
  []
  [front_disp_y_min]
    type = NodalExtremeValue
    variable = disp_y
    boundary = front
    value_type = min
  []
  [front_spread]
    type = DifferencePostprocessor
    value1 = front_disp_y_max
    value2 = front_disp_y_min
  []
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  automatic_scaling = true
  petsc_options_iname = '-pc_type'
  petsc_options_value = lu
  l_max_its = 30
  nl_max_its = 20
  start_time = 0
  end_time = 105000
  dt = 100
  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-5
[]

[Preconditioning]
  [smp]
    type = SMP
    full = true
  []
[]

[Outputs]
  exodus = true
  csv = true
[]
