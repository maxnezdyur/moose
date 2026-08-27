[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 4
    ny = 1
    xmax = 4.0
    ymax = 1.0
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
[]

[AuxVariables]
  [hourglass_force_x]
    order = FIRST
    family = LAGRANGE
  []
  [hourglass_force_y]
    order = FIRST
    family = LAGRANGE
  []
[]

[Kernels]
  [sdx]
    type = StressDivergenceTensors
    variable = disp_x
    displacements = 'disp_x disp_y'
    component = 0
  []
  [sdy]
    type = StressDivergenceTensors
    variable = disp_y
    displacements = 'disp_x disp_y'
    component = 1
  []
  [hourglass_x]
    type = HourglassStabilization
    variable = disp_x
    penalty = 0.1
    save_in = 'hourglass_force_x'
  []
  [hourglass_y]
    type = HourglassStabilization
    variable = disp_y
    penalty = 0.1
    save_in = 'hourglass_force_y'
  []
[]

[BCs]
  [clamp_x]
    type = DirichletBC
    variable = disp_x
    boundary = left
    value = 0.0
  []
  [clamp_y]
    type = DirichletBC
    variable = disp_y
    boundary = left
    value = 0.0
  []
  [tip_shear]
    type = NeumannBC
    variable = disp_y
    boundary = right
    value = -10.0
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
    displacements = 'disp_x disp_y'
  []
  [stress]
    type = ComputeLinearElasticStress
  []
[]

[Postprocessors]
  [hourglass_force_x_norm]
    type = NodalL2Norm
    variable = hourglass_force_x
  []
  [hourglass_force_y_norm]
    type = NodalL2Norm
    variable = hourglass_force_y
  []
  [tip_deflection]
    type = NodalExtremeValue
    variable = disp_y
    value_type = min
  []
  [disp_x_max]
    type = NodalExtremeValue
    variable = disp_x
    value_type = max
  []
  [disp_x_min]
    type = NodalExtremeValue
    variable = disp_x
    value_type = min
  []
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  line_search = none
  petsc_options_iname = '-ksp_type -pc_type'
  petsc_options_value = 'preonly   lu'
  nl_rel_tol = 1e-10
  nl_abs_tol = 1e-10
  nl_max_its = 10
  dt = 0.5
  dtmin = 0.5
  num_steps = 2
  [Quadrature]
    type = GAUSS
    element_order = CONSTANT
  []
[]

[Outputs]
  csv = true
[]
