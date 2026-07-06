# Phase 7: coupled thermomechanics at one-point quadrature, all fields
# stabilized with the same gamma. Conduction with k(T); mechanics with
# thermal-expansion eigenstrain and a temperature-dependent shear modulus in
# the mechanical hourglass coefficient (exercising the off-diagonal
# d(mu)/dT tangent). Gates: FD-vs-analytic Jacobian and quadratic Newton.
[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 6
    ny = 6
    elem_type = QUAD4
  []
  [distort]
    type = ParsedNodeTransformGenerator
    input = gen
    x_function = 'x + 0.05*sin(3.1*y+0.7)*if(x*(1-x)>0.001,1,0)'
    y_function = 'y + 0.05*sin(2.7*x+0.3)*if(y*(1-y)>0.001,1,0)'
  []
[]

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Variables]
  [disp_x]
  []
  [disp_y]
  []
  [T]
  []
[]

[Kernels]
  [sdx]
    type = ADStressDivergenceTensors
    variable = disp_x
    component = 0
  []
  [sdy]
    type = ADStressDivergenceTensors
    variable = disp_y
    component = 1
  []
  [hg_x]
    type = HourglassCorrectionDiffusion
    variable = disp_x
    diffusivity = mu
  []
  [hg_y]
    type = HourglassCorrectionDiffusion
    variable = disp_y
    diffusivity = mu
  []
  [cond]
    type = ADMatDiffusion
    variable = T
    diffusivity = k
  []
  [heat_src]
    type = ADBodyForce
    variable = T
    value = 3
  []
  [hg_T]
    type = HourglassCorrectionDiffusion
    variable = T
    diffusivity = k
  []
[]

[Materials]
  [k]
    type = ADParsedMaterial
    property_name = k
    expression = '2.0 + 0.5*T'
    coupled_variables = 'T'
  []
  [mu]
    type = ADParsedMaterial
    property_name = mu
    expression = '40.0 - 8.0*T'
    coupled_variables = 'T'
  []
  [C]
    type = ADComputeIsotropicElasticityTensor
    youngs_modulus = 100
    poissons_ratio = 0.3
  []
  [strain]
    type = ADComputeSmallStrain
    eigenstrain_names = 'thermal_eig'
  []
  [thermal_eig]
    type = ADComputeThermalExpansionEigenstrain
    temperature = T
    thermal_expansion_coeff = 1e-3
    stress_free_temperature = 0
    eigenstrain_name = thermal_eig
  []
  [stress]
    type = ADComputeLinearElasticStress
  []
[]

[BCs]
  [T_left]
    type = DirichletBC
    variable = T
    boundary = left
    value = 0
  []
  [T_right]
    type = DirichletBC
    variable = T
    boundary = right
    value = 1
  []
  [fix_x]
    type = DirichletBC
    variable = disp_x
    boundary = left
    value = 0
  []
  [fix_y]
    type = DirichletBC
    variable = disp_y
    boundary = bottom
    value = 0
  []
[]

[Postprocessors]
  [ux_max]
    type = NodalExtremeValue
    variable = disp_x
  []
  [uy_max]
    type = NodalExtremeValue
    variable = disp_y
  []
  [T_avg]
    type = ElementAverageValue
    variable = T
  []
  [hg_energy_T]
    type = HourglassEnergy
    variable = T
    diffusivity = k
  []
  [hg_energy_ux]
    type = HourglassEnergy
    variable = disp_x
    diffusivity = mu
  []
[]

[Preconditioning]
  [smp]
    type = SMP
    full = true
  []
[]

[Executioner]
  type = Steady
  solve_type = NEWTON
  nl_rel_tol = 1e-13
  [Quadrature]
    type = GAUSS
    order = CONSTANT
  []
[]

[Outputs]
  csv = true
[]
