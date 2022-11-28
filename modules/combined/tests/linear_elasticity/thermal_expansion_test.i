# This input file is designed to test the RankTwoAux and RankFourAux
# auxkernels, which report values out of the Tensors used in materials
# properties.
# Materials properties into AuxVariables - these are elemental variables, not nodal variables.

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 2
  ny = 2
  nz = 0
  xmin = 0
  xmax = 2
  ymin = 0
  ymax = 2
  zmin = 0
  zmax = 0
  elem_type = QUAD4
[]

[Variables]
  [./disp_x]
    order = FIRST
    family = LAGRANGE
  [../]
  [./disp_y]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[AuxVariables]
  # stresses
  [./s11_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./s12_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./s22_aux]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[TensorMechanics]
  [./solid]
    disp_x = disp_x
    disp_y = disp_y
  [../]
[]

[AuxKernels]
  [./matl_s11]
    type = RankTwoAux
    rank_two_tensor = stress
    index_i = 1
    index_j = 1
    variable = s11_aux
  [../]
  [./matl_s12]
    type = RankTwoAux
    rank_two_tensor = stress
    index_i = 1
    index_j = 2
    variable = s12_aux
  [../]
  [./matl_s22]
    type = RankTwoAux
    rank_two_tensor = stress
    index_i = 2
    index_j = 2
    variable = s22_aux
  [../]
[]

[Materials]
  [./Anisotropic]
    type = LinearElasticMaterial
    block = 0
    disp_x = disp_x
    disp_y = disp_y
    all_21 = false
    C_ijkl = '1e6 0 0 1e6 0 1e6 .5e6 .5e6 .5e6'
    thermal_expansion_coeff = 1.0e-6
    Temp = 400
  [../]
[]

[BCs]
  [./bottom_y]
    type = PresetBC
    variable = disp_y
    boundary = 'bottom'
    value = 0
  [../]
  [./left_x]
    type = PresetBC
    variable = disp_x
    boundary = 'left'
    value = 0
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Steady

  solve_type = 'NEWTON'


  nl_rel_tol = 1e-14
[]

[Output]
  linear_residuals = true
  file_base = thermal_expansion
  output_initial = true
  interval = 1
  exodus = true
  perf_log = true
[]

