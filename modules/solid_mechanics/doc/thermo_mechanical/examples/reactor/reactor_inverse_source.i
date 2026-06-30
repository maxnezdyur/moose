# Inverse problem: recover the unknown fuel-rod heat-source magnitude q'''
# from four thermocouple temperature readings.
#
# This MAIN input runs the optimizer (TAO). Each iteration it picks a q,
# launches the forward+adjoint sub-app to get the data-misfit objective and
# its gradient, and steps q downhill until the simulated temperatures match
# the measurements. Run with:  combined-opt -i reactor_inverse_source.i

[Optimization][]

[OptimizationReporter]
  type = GeneralOptimization
  objective_name = objective_value
  parameter_names = 'q'
  num_values = '1'
  initial_condition = '1e6' # guess; true = 8e6
  lower_bounds = '0'
[]

[MultiApps/forward]
  type = FullSolveMultiApp
  input_files = reactor_inverse_forward_and_adjoint.i
  execute_on = FORWARD
[]

[Transfers]
  [to_forward]
    type = MultiAppReporterTransfer
    to_multi_app = forward
    from_reporters = 'OptimizationReporter/q'
    to_reporters = 'params/q'
  []
  [from_forward]
    type = MultiAppReporterTransfer
    from_multi_app = forward
    from_reporters = 'data/objective_value gradient/inner_product'
    to_reporters = 'OptimizationReporter/objective_value OptimizationReporter/grad_q'
  []
[]

[Executioner]
  type = Optimize
  tao_solver = taobncg # bounded nonlinear CG, gradient-based
  petsc_options_iname = '-tao_gatol'
  petsc_options_value = '1e-4'
  verbose = true
[]

[Outputs]
  console = true
[]
