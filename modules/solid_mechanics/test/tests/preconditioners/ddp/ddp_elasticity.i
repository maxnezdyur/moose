[Mesh]
  [cube]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 4
    ny = 4
    nz = 4
  []
[]

[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
[]

[Physics/SolidMechanics/QuasiStatic]
  [all]
    strain = SMALL
    add_variables = true
    generate_output = 'stress_xx stress_yy stress_zz'
  []
[]

[BCs]
  [symmetry_x]
    type = DirichletBC
    variable = disp_x
    boundary = left
    value = 0
  []
  [symmetry_y]
    type = DirichletBC
    variable = disp_y
    boundary = bottom
    value = 0
  []
  [symmetry_z]
    type = DirichletBC
    variable = disp_z
    boundary = back
    value = 0
  []
  [stretch_x]
    type = DirichletBC
    variable = disp_x
    boundary = right
    value = 0.01
  []
[]

[Materials]
  [elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 2.1e5
    poissons_ratio = 0.3
  []
  [stress]
    type = ComputeLinearElasticStress
  []
[]

[Preconditioning]
  [ddp]
    type = DDP
    full = true
  []
[]

[Executioner]
  type = Steady
  solve_type = NEWTON
  l_tol = 1e-8
  nl_rel_tol = 1e-10
  petsc_options = '-ksp_view'
[]

[Outputs]
  exodus = true
[]
