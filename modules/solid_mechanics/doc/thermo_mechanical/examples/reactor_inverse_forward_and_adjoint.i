# Forward + adjoint sub-app for the inverse heat-source problem.
# Forward: steady fuel-rod conduction with an unknown uniform heat source q.
# Adjoint: solved automatically (SteadyAndAdjoint) to give the objective
# gradient dObjective/dq in one extra solve. Driven by reactor_inverse_source.i.

[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 40
    ny = 10
    xmax = 0.30
    ymax = 0.05
  []
  coord_type = RZ
  rz_coord_axis = X
[]

[Problem]
  nl_sys_names = 'nl0 adjoint'
  kernel_coverage_check = false
[]

[Variables]
  [T][]
  [T_adjoint]
    solver_sys = adjoint
    outputs = none
  []
[]

[Kernels]
  [conduction]
    type = HeatConduction
    variable = T
  []
  [source]
    type = BodyForce
    variable = T
    function = source_func # q''' supplied by the optimizer
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
  boundary = top
  value = 350
[]

[Functions/source_func]
  # Uniform volumetric source of magnitude q (the single design parameter)
  type = ParsedOptimizationFunction
  expression = 'q'
  param_symbol_names = 'q'
  param_vector_name = 'params/q'
[]

[Reporters]
  [params]
    type = ConstantReporter
    real_vector_names = 'q'
    real_vector_values = '0' # overwritten each iteration by the optimizer
    outputs = none
  []
  [data]
    type = OptimizationData
    variable = T
    objective_name = objective_value
    measurement_file = reactor_thermocouples.csv
    file_xcoord = x
    file_ycoord = y
    file_zcoord = z
    file_value = value
    outputs = none
  []
[]

# Misfit (T_sim - T_measured) injected at each thermocouple as the adjoint load
[DiracKernels/misfit]
  type = ReporterPointSource
  variable = T_adjoint
  x_coord_name = data/measurement_xcoord
  y_coord_name = data/measurement_ycoord
  z_coord_name = data/measurement_zcoord
  value_name = data/misfit_values
[]

# Gradient of the objective w.r.t. q = integral of the adjoint against the source
[VectorPostprocessors/gradient]
  type = ElementOptimizationSourceFunctionInnerProduct
  variable = T_adjoint
  function = source_func
  execute_on = ADJOINT_TIMESTEP_END
  outputs = none
[]

[Preconditioning]
  [nl0]
    type = SMP
    nl_sys = 'nl0'
    petsc_options_iname = '-pc_type'
    petsc_options_value = 'lu'
  []
  [adjoint]
    type = SMP
    nl_sys = 'adjoint'
    petsc_options_iname = '-pc_type'
    petsc_options_value = 'lu'
  []
[]

[Executioner]
  type = SteadyAndAdjoint
  forward_system = nl0
  adjoint_system = adjoint
  nl_rel_tol = 1e-10
  l_tol = 1e-10
[]

[Outputs]
  console = false
[]
