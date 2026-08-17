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
    num_rows = 40
    distributions = 'S_dist D_dist L_dist'
    execute_on = PRE_MULTIAPP_SETUP
    seed = 0
  []
[]

[ROM]
  sampler = sample
  input = sub.i
  parameters = 'S D L'
  auto_tag_residual = true
  num_modes = '30 10 10'
[]
