[StochasticTools]
[]

[Distributions]
  [S_dist]
    type = Uniform
    lower_bound = 0
    upper_bound = 20
  []
  [D_dist]
    type = Uniform
    lower_bound = 0
    upper_bound = 20
  []
  [L_dist]
    type = Uniform
    lower_bound = 1
    upper_bound = 10
  []
[]

[Samplers]
  [sample]
    type = MonteCarlo
    num_rows = 3
    distributions = 'S_dist D_dist L_dist'
    execute_on = PRE_MULTIAPP_SETUP
    seed = 0
  []
[]

[MultiApps]
  [worker]
    type = SamplerFullSolveMultiApp
    input_files = sub.i
    sampler = sample
    mode = batch-reset
  []
[]

[Transfers]
  [snapshot_transfer]
    type = SerializedSnapshotTransfer
    parallel_storage = parallel_storage
    from_multi_app = worker
    sampler = sample
    solution_container = solution_storage
    residual_container = residual_storage
    jacobian_container = jacobian_storage
    serialize_on_root = true
  []
[]

[Controls]
  [cmd_line]
    type = MultiAppSamplerControl
    multi_app = worker
    sampler = sample
    param_names = 'S D L'
  []
[]

[Reporters]
  [parallel_storage]
    type = ParallelSolutionStorage
    variables = 'solution jacobian residual::residual_tag_diffusion_u residual::residual_tag_source_u residual::residual_tag_left_u residual::residual_tag_right_u'
    outputs = out
  []
[]

[Outputs]
  [out]
    type = JSON
    execute_on = FINAL
    execute_system_information_on = none
  []
[]
