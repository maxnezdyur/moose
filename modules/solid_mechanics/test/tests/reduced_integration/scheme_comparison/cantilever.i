[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 6
    ny = 4
    xmax = 10.0
    ymax = 1.0
  []
[]

[Physics]
  [SolidMechanics]
    [QuasiStatic]
      [all]
        add_variables = true
        strain = SMALL
      []
    []
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
  [elasticity]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 100000.0
    poissons_ratio = 0.3
  []
  [stress]
    type = ComputeLinearElasticStress
  []
[]

[Postprocessors]
  [tip_disp_y]
    type = PointValue
    variable = disp_y
    point = '10.0 0.5 0.0'
  []
  # Mesh-converged tip deflection: Richardson extrapolation of a volumetric-locking-corrected
  # refinement sweep (6x4 through 240x48), cross-checked by reduced integration converging to the
  # same limit from the opposite side. Not a full-quadrature value -- full quadrature is still 26%
  # off at 80x16 for nu = 0.499. The nu = 0.499 reference comes in by cli_args.
  [reference_disp]
    type = ConstantPostprocessor
    value = -0.36587
  []
  [normalized_disp]
    type = ParsedPostprocessor
    expression = 'tip_disp_y / reference_disp'
    pp_names = 'tip_disp_y reference_disp'
  []
[]

[Executioner]
  type = Steady
  solve_type = NEWTON
  line_search = none
  petsc_options_iname = '-ksp_type -pc_type'
  petsc_options_value = 'preonly   lu'
  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-8
  [Quadrature]
    type = GAUSS
    element_order = AUTO
  []
[]

[Outputs]
  csv = true
[]
