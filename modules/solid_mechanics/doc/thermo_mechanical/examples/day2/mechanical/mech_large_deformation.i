# Day 2 - Mechanics: LARGE DEFORMATION (geometric nonlinearity)
#
# A slender cantilever beam, clamped at its left end, is pushed down by a
# large pressure on its top surface until it bends through a big arc.
#
# THE KEY IDEA - geometric nonlinearity:
#   In the earlier *linear* mechanics examples we used strain = SMALL +
#   ComputeLinearElasticStress.  There, strain is a linear function of the
#   displacement gradient and stress is a linear function of strain, so the
#   problem is LINEAR: Newton converges in a single iteration and doubling the
#   load exactly doubles the deflection.
#
#   Here the deflection is comparable to the beam length, so we MUST account
#   for the change in geometry as the beam deforms.  We switch to the
#   finite-strain path (strain = FINITE + ComputeFiniteStrainElasticStress).
#   Now the strain measure is a *nonlinear* function of the displacements, the
#   residual is nonlinear, and Newton needs MANY iterations to converge.  This
#   is geometric (large-deformation) nonlinearity - the material law itself is
#   still linear elastic.

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

# 2D beam: 10 long, 1 tall.  Default 2D Cartesian = plane strain.
[Mesh]
  [beam]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 40
    ny = 4
    xmax = 10
    ymax = 1
  []
[]

# Finite-strain solid mechanics action.
#   strain = FINITE             -> nonlinear (large-rotation) strain measure
#   ADComputeFiniteStrainElasticStress (added by the action) pairs with it
#   use_automatic_differentiation -> EXACT Jacobian, which a hand-coded
#     finite-strain Jacobian cannot easily match; this is what lets NEWTON
#     drive the strongly nonlinear residual all the way down.
# Contrast: the linear examples used the default strain = SMALL.
[Physics/SolidMechanics/QuasiStatic]
  [all]
    add_variables = true
    strain = FINITE
    use_automatic_differentiation = true
    # stress_xx / strain_xx -> the axial BENDING fields: tension on one face,
    # compression on the other, growing toward the clamped end.  The action
    # auto-creates these aux vars and handles the AD stress path.
    generate_output = 'vonmises_stress stress_xx strain_xx'
  []
[]

# Clamp the left end: both displacement components pinned -> a true cantilever.
[BCs]
  [fix_x]
    type = DirichletBC
    variable = disp_x
    boundary = left
    value = 0
  []
  [fix_y]
    type = DirichletBC
    variable = disp_y
    boundary = left
    value = 0
  []
  # Downward pressure on the top face.  Ramped in over pseudo-time steps so the
  # huge final load is reached gradually, which keeps Newton in its basin of
  # convergence.  A positive Pressure pushes inward (-y on the top face).
  [Pressure]
    [top_load]
      boundary = top
      function = '3300*t' # ramped: 3300 Pa added per pseudo-time step
      use_automatic_differentiation = true # match the AD stress path
    []
  []
[]

# Linear-elastic material (the constitutive law is still linear) on the
# finite-strain stress path.
[Materials]
  [elasticity]
    type = ADComputeIsotropicElasticityTensor
    youngs_modulus = 1e7
    poissons_ratio = 0.3
  []
  [stress]
    # Finite-strain elastic stress: REQUIRED partner of strain = FINITE.
    type = ADComputeFiniteStrainElasticStress
  []
[]

# Full coupling between disp_x and disp_y in the preconditioner -> good Newton
# steps for the strongly coupled large-deformation problem.
[Preconditioning]
  [smp]
    type = SMP
    full = true
  []
[]

# Extra field for visualization only (does NOT affect the solve).
#   disp_mag = sqrt(disp_x^2 + disp_y^2) -> the total displacement magnitude.
#   Color the deformed beam by this in ParaView to SEE the large deflection:
#   it grows from 0 at the clamped left end to its maximum at the free tip.
[AuxVariables]
  [disp_mag]
    family = LAGRANGE
    order = FIRST
  []
[]
[AuxKernels]
  [disp_mag]
    type = ParsedAux
    variable = disp_mag
    coupled_variables = 'disp_x disp_y'
    expression = 'sqrt(disp_x^2 + disp_y^2)'
    execute_on = 'initial timestep_end'
  []
[]

# Track the response: tip deflection and peak stress.
[Postprocessors]
  [tip_disp_y]
    type = NodalExtremeValue
    variable = disp_y
    value_type = min # largest downward (most negative) deflection
  []
  [max_vonmises]
    type = ElementExtremeValue
    variable = vonmises_stress
  []
[]

# Pseudo-transient load stepping. Each pseudo-time step adds 3300 Pa and is a
# full nonlinear solve; reaching the load gradually keeps Newton converging.
#   NEWTON + lu : robust direct solve of the nonlinear system
#   line_search = none : with the EXACT AD Jacobian and gradual load steps,
#     plain Newton converges quadratically. A backtracking line search
#     (line_search = bt) is the fallback if a step ever diverges, but here it
#     would only slow things down, so we leave it off.
# Each load step here takes ~8 nonlinear iterations to converge -- contrast the
# LINEAR examples, where every solve finishes in a single Newton iteration.
[Executioner]
  type = Transient
  solve_type = NEWTON
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  line_search = none
  nl_max_its = 25
  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-9
  automatic_scaling = true
  num_steps = 4
  dt = 1
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
[]
