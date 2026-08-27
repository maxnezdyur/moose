[Mesh]
  [msh]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 4
    ny = 4
    nz = 4
  []
[]

[Variables]
  [disp_x]
    order = FIRST
    family = LAGRANGE
  []
  [disp_y]
    order = FIRST
    family = LAGRANGE
  []
  [disp_z]
    order = FIRST
    family = LAGRANGE
  []
[]

[ICs]
  [disp_x]
    type = RandomIC
    variable = disp_x
    min = -0.02
    max = 0.02
  []
  [disp_y]
    type = RandomIC
    variable = disp_y
    min = -0.02
    max = 0.02
  []
  [disp_z]
    type = RandomIC
    variable = disp_z
    min = -0.02
    max = 0.02
  []
[]

[Kernels]
  [sdx]
    type = StressDivergenceTensors
    variable = disp_x
    displacements = 'disp_x disp_y disp_z'
    component = 0
  []
  [sdy]
    type = StressDivergenceTensors
    variable = disp_y
    displacements = 'disp_x disp_y disp_z'
    component = 1
  []
  [sdz]
    type = StressDivergenceTensors
    variable = disp_z
    displacements = 'disp_x disp_y disp_z'
    component = 2
  []
  [hourglass_x]
    type = HourglassStabilization
    variable = disp_x
    penalty = 0.1
  []
  [hourglass_y]
    type = HourglassStabilization
    variable = disp_y
    penalty = 0.1
  []
  [hourglass_z]
    type = HourglassStabilization
    variable = disp_z
    penalty = 0.1
  []
[]

[BCs]
  [leftx]
    type = DirichletBC
    preset = true
    boundary = left
    variable = disp_x
    value = 0.0
  []
  [lefty]
    type = DirichletBC
    preset = true
    boundary = left
    variable = disp_y
    value = 0.0
  []
  [leftz]
    type = DirichletBC
    preset = true
    boundary = left
    variable = disp_z
    value = 0.0
  []
[]

[Materials]
  [elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 100000.0
    poissons_ratio = 0.3
  []
  [strain]
    type = ComputeSmallStrain
    displacements = 'disp_x disp_y disp_z'
  []
  [stress]
    type = ComputeLinearElasticStress
  []
[]

[Preconditioning]
  [smp]
    type = SMP
    full = true
  []
[]

[Executioner]
  type = Transient

  solve_type = 'newton'
  line_search = none

  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  automatic_scaling = true

  l_max_its = 2
  l_tol = 1e-14
  nl_max_its = 15
  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-10

  start_time = 0.0
  dt = 1.0
  dtmin = 1.0
  end_time = 1.0

  [Quadrature]
    type = GAUSS
    element_order = CONSTANT
  []
[]
