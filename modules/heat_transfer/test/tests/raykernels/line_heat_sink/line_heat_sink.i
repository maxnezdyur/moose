[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 8
    ny = 8
    nz = 8
  []
[]

[Variables]
  [T]
    order = FIRST
    family = LAGRANGE
  []
[]

[Kernels]
  [diffusion]
    type = ADHeatConduction
    variable = T
  []
[]

[Materials]
  [k]
    type = ADGenericConstantMaterial
    prop_names = 'thermal_conductivity'
    prop_values = '1.0'
  []
[]

[BCs]
  [left]
    type = DirichletBC
    variable = T
    boundary = left
    value = 0.0
  []
  [right]
    type = DirichletBC
    variable = T
    boundary = right
    value = 1.0
  []
[]

[Functions]
  [linear_x]
    type = ParsedFunction
    expression = 'x'
  []
  [h_const]
    type = ConstantFunction
    value = 1.0
  []
  [T_sink_const]
    type = ConstantFunction
    value = 0.0
  []
  # Non-constant variants (selected via cli_args in the function_coefficients sub-test)
  [h_var]
    type = ParsedFunction
    expression = '2 + x'
  []
  [T_sink_var]
    type = ParsedFunction
    expression = '0.1 * x'
  []
[]

[UserObjects]
  [sink_study]
    type = RepeatableRayStudy
    names = 'heat_sink_ray'
    start_points = '0.0 0.371 0.371'
    end_points = '1.0 0.371 0.371'
    execute_on = PRE_KERNELS
  []
  [integral_study]
    type = RepeatableRayStudy
    names = 'measure_ray'
    start_points = '0.0 0.371 0.371'
    end_points = '1.0 0.371 0.371'
    execute_on = TIMESTEP_END
  []
[]

[RayKernels]
  [sink]
    type = ADLineHeatSinkRayKernel
    variable = T
    h = h_const
    T_sink = T_sink_const
    study = sink_study
  []
  [function_integral]
    type = FunctionIntegralRayKernel
    function = linear_x
    study = integral_study
  []
  [T_integral]
    type = VariableIntegralRayKernel
    variable = T
    study = integral_study
  []
[]

[Postprocessors]
  [total_ray_length]
    type = RayTracingStudyResult
    study = sink_study
    result = total_distance
    execute_on = TIMESTEP_END
  []
  [function_integral_pp]
    type = RayIntegralValue
    ray_kernel = function_integral
    ray = measure_ray
    execute_on = TIMESTEP_END
  []
  [T_integral_pp]
    type = RayIntegralValue
    ray_kernel = T_integral
    ray = measure_ray
    execute_on = TIMESTEP_END
  []
[]

[Preconditioning]
  [smp]
    type = SMP
    full = true
  []
[]

[Executioner]
  type = Steady
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
  nl_abs_tol = 1e-12
[]

[Outputs]
  csv = true
  exodus=true
[]
