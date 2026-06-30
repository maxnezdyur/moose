# ==============================================================================
# Day 2 - Mechanics: SMALL vs FINITE strain
# ------------------------------------------------------------------------------
# A block is stretched 25% in x by a prescribed end displacement. Its top and
# bottom faces are held in y (a confined "channel" stretch), so the deformation
# is a clean, homogeneous, axial stretch. That isolates the one thing this
# example is about: how the choice of STRAIN MEASURE (small vs finite) changes
# the predicted stress once the stretch is large.
#
# This shipped file uses the FINITE-strain formulation and MUST run:
#     Physics/SolidMechanics/QuasiStatic/all/strain = FINITE
#     Materials/stress  type = ComputeFiniteStrainElasticStress
# At full load it reports (averaged over the block):
#     strain_xx ~ 0.223  (= ln(1.25), the logarithmic/true stretch)
#     stress_xx ~ 6.0e10 Pa
#
#   *** TRY IT: re-run with small strain to watch the answer change ***
#   Change the two lines flagged below and re-run, then compare stress_xx:
#     1) [Physics/SolidMechanics/QuasiStatic][all]  strain = SMALL
#     2) [Materials][stress]  type = ComputeLinearElasticStress
#   The SMALL-strain case ALSO runs (and converges in ~1 iteration/step because
#   it is linear), but it uses engineering strain (strain_xx = 0.25) and
#   predicts stress_xx ~ 6.7e10 Pa -- about 12% higher. That gap is the
#   linearization error: at 25% stretch the small-strain assumption visibly
#   diverges from the finite-strain answer.
#
# The load is applied through a Transient executioner used purely as PSEUDO-TIME
# load stepping: the end displacement is ramped 0 -> 0.25 in 5 steps so each
# NEWTON solve starts near its answer (~5 iterations/step). It also keeps the
# incremental finite-strain update accurate (small strain increments per step);
# the final step is the steady stretched state of interest.
# ==============================================================================

[GlobalParams]
  # Names the displacement variables for the whole input (used by the action,
  # the strain calculator, and the stress-divergence kernels).
  displacements = 'disp_x disp_y'
[]

# 2D block, 1.0 long (x) by 0.1 tall (y). Coarse in y: the deformation is a
# homogeneous uniaxial stretch, so few elements through the thickness suffice.
[Mesh]
  [block]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 20
    ny = 2
    xmax = 1.0
    ymax = 0.1
  []
[]

# QuasiStatic SolidMechanics action: adds disp_x/disp_y, the stress-divergence
# kernels, and the strain calculator. strain = FINITE selects the large-
# deformation (incremental) strain measure that pairs with the finite-strain
# stress material below. generate_output writes the stress/strain AuxVariables.
[Physics/SolidMechanics/QuasiStatic]
  [all]
    add_variables = true
    strain = FINITE # *** change to SMALL for the linearized comparison ***
    # stress_xx/strain_xx are the axial pair the SMALL-vs-FINITE comparison is about.
    # The next three are added purely for visualization -- open them in ParaView to
    # see the rest of the confined-stretch story this example sets up:
    #   strain_yy       ~ 0    -- the y-confinement holds the channel (no lateral motion)
    #   stress_yy       != 0   -- yet that confinement carries a transverse reaction stress
    #   vonmises_stress        -- one scalar summarizing the (now biaxial) stress state
    generate_output = 'stress_xx strain_xx strain_yy stress_yy vonmises_stress'
  []
[]

# Ramp the end displacement linearly in pseudo-time: reaches 0.25 at t = 1.
[Functions]
  [pull]
    type = ParsedFunction
    expression = '0.25 * t'
  []
[]

# Prescribed-displacement loading. Grip the left end in x, pull the right end
# in x to +0.25 over the 1.0-long block (stretch ratio = 1.25). Holding disp_y
# on top and bottom keeps the stretch homogeneous (no lateral contraction), so
# the small-vs-finite comparison is about the strain measure, not geometry.
[BCs]
  # Left end held in x.
  [left_x]
    type = DirichletBC
    variable = disp_x
    boundary = left
    value = 0
  []
  # Right end pulled out to 0.25 (ramped through pseudo-time)  ->  stretch 1.25.
  [right_x]
    type = FunctionDirichletBC
    variable = disp_x
    boundary = right
    function = pull
  []
  # Confine the top and bottom faces in y -> homogeneous axial stretch.
  [top_y]
    type = DirichletBC
    variable = disp_y
    boundary = top
    value = 0
  []
  [bottom_y]
    type = DirichletBC
    variable = disp_y
    boundary = bottom
    value = 0
  []
[]

[Materials]
  # Linear-elastic constitutive constants (steel-like).
  [elasticity]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 2e11 # Pa
    poissons_ratio = 0.3
  []
  # Finite-strain elastic stress (pairs with strain = FINITE above).
  # *** For the SMALL-strain comparison, swap this to ComputeLinearElasticStress. ***
  [stress]
    type = ComputeFiniteStrainElasticStress
  []
[]

# Consider all off-diagonal Jacobian blocks: needed for a clean NEWTON solve.
[Preconditioning]
  [smp]
    type = SMP
    full = true
  []
[]

# Quantities to compare against a SMALL-strain re-run.
[Postprocessors]
  # Axial stress -- this is what visibly changes between SMALL and FINITE strain.
  [stress_xx_avg]
    type = ElementAverageValue
    variable = stress_xx
  []
  # Axial strain measure: ~0.223 = ln(1.25) for FINITE, exactly 0.25 for SMALL.
  [strain_xx_avg]
    type = ElementAverageValue
    variable = strain_xx
  []
  # End displacement at the pulled face; axial stretch = 1 + end_disp_x / L,
  # with L = 1.0, so 0.25 of displacement is a stretch ratio of 1.25.
  [end_disp_x]
    type = PointValue
    variable = disp_x
    point = '1.0 0.0 0.0'
  []
[]

# Full NEWTON. Pseudo-time (5 steps to t = 1) only ramps the stretch; the state
# at the final step (stretch = 1.25) is the steady result of interest.
[Executioner]
  type = Transient
  solve_type = NEWTON
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  end_time = 1.0
  dt = 0.2
  nl_rel_tol = 1e-10
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
[]
