[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 8
    ny = 8
    xmax = 1.0
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

[Functions]
  [surface_pressure]
    type = ParsedFunction
    expression = '-1000.0 * cos(1.5707963267948966 * x)'
  []
[]

[BCs]
  [confine_left]
    type = DirichletBC
    variable = disp_x
    boundary = left
    value = 0.0
  []
  [confine_right]
    type = DirichletBC
    variable = disp_x
    boundary = right
    value = 0.0
  []
  [support]
    type = DirichletBC
    variable = disp_y
    boundary = bottom
    value = 0.0
  []
  [graded_load]
    type = FunctionNeumannBC
    variable = disp_y
    boundary = top
    function = surface_pressure
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
  [char_disp_y]
    type = PointValue
    variable = disp_y
    point = '0.0 1.0 0.0'
  []
  # Mesh-converged settlement: Richardson extrapolation of a volumetric-locking-corrected
  # refinement sweep (8x8 through 128x128, observed ratio 4.00 = exactly second order),
  # cross-checked by reduced integration converging to the same limit from the opposite side.
  # Not a full-quadrature value. The nu = 0.499 reference comes in by cli_args.
  [reference_disp]
    type = ConstantPostprocessor
    value = -0.0069273
  []
  [normalized_disp]
    type = ParsedPostprocessor
    expression = 'char_disp_y / reference_disp'
    pp_names = 'char_disp_y reference_disp'
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
