[Mesh]
  [cube]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 10
    ny = 10
    nz = 10
  []
[]

[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
[]

[Physics/SolidMechanics/QuasiStatic]
  [all]
    strain = FINITE
    add_variables = true
    use_automatic_differentiation = true
    # decomposition_method = eigensolution
    # fill the near-null-space with the displacement rigid body modes; PCBDDC consumes it
    # (-pc_bddc_use_nnsp defaults to true) to enrich its coarse space
    # rigid_body_near_null_space = true
    generate_output = 'stress_xx stress_xy stress_yy'

  []
[]

[BCs]
  [clamp_x]
    type = ADDirichletBC
    variable = disp_x
    boundary = left
    value = 0
  []
  [clamp_y]
    type = ADDirichletBC
    variable = disp_y
    boundary = left
    value = 0
  []
  [clamp_z]
    type = ADDirichletBC
    variable = disp_z
    boundary = left
    value = 0
  []
  [shear_y]
    # a traction load rather than a displacement: presetting a displacement jump against a zero
    # interior concentrates the whole shear in one element layer at the first iterate, which
    # defeats exact Newton on fine meshes regardless of the linear solver
    type = ADFunctionNeumannBC
    variable = disp_y
    boundary = right
    function = '4000*t'
  []
[]

[Materials]
  [elasticity_tensor]
    type = ADComputeIsotropicElasticityTensor
    youngs_modulus = 2.1e5
    poissons_ratio = 0.3
  []
  [stress]
    type = ADComputeFiniteStrainElasticStress
  []
[]

[Preconditioning]
  [ddp]
    type = DDP
    full = true
  []
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  line_search = none
  dt = 1
  num_steps = 1
  # a single load step; a solve that needs a timestep cut is a failure, not a retry
  abort_on_solve_fail = true
  # tight enough that the converged solution is partition independent at the Exodiff tolerances
  l_tol = 1e-8
  nl_rel_tol = 1e-10
  # petsc_options = '-ksp_view'
[]

[Outputs]
  exodus = true
[]
