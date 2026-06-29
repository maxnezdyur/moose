# Day 2 - Mechanics: applied load + rigid-body pinning + tensor visualization
# =============================================================================
# A 2-D elastic plate is loaded by a pressure on its top edge. The ONLY essential
# (Dirichlet) constraints are the minimal "pins" required to remove the three 2-D
# rigid-body modes (x-translation, y-translation, in-plane rotation). The pressure
# load is self-equilibrated by the pin reactions, so the plate develops a real
# stress field that we visualize with RankTwoAux / RankTwoScalarAux.
#
# Concepts reinforced:
#   * Pressure load via the [BCs]/[Pressure] action
#   * Removing rigid-body modes with the FEWEST possible Dirichlet DOFs
#   * ExtraNodesetGenerator to tag individual nodes for point pins
#   * RankTwoAux (one tensor component) and RankTwoScalarAux (von Mises) for
#     turning the stress tensor into viewable aux fields
#   * Postprocessors reducing an aux field to a single number written to CSV

# 'displacements' is consumed by the SolidMechanics action, the Pressure action,
# and the strain calculators - set it once here.
[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  # A simple rectangular plate, 2 x 1. GeneratedMeshGenerator auto-creates the
  # 'left', 'right', 'top', 'bottom' sidesets. Node spacing is 0.1 in x and y,
  # so the corner coordinates below land exactly on mesh nodes.
  [plate]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 20
    ny = 10
    xmax = 2.0
    ymax = 1.0
  []

  # Pin node #1: the bottom-left corner. We will fix BOTH disp_x and disp_y here.
  [pin_corner]
    type = ExtraNodesetGenerator
    input = plate
    new_boundary = pin_corner
    coord = '0 0 0'
  []

  # Pin node #2: the bottom-right corner. We will fix ONLY disp_y here. The
  # horizontal offset from pin node #1 is what kills the rotational mode.
  [pin_roller]
    type = ExtraNodesetGenerator
    input = pin_corner
    new_boundary = pin_roller
    coord = '2 0 0'
  []
[]

# SolidMechanics QuasiStatic action: adds disp_x/disp_y, the stress-divergence
# kernels, and the SMALL-strain calculator in one shot. ComputeLinearElasticStress
# (below) consumes the small strain.
[Physics/SolidMechanics/QuasiStatic]
  [all]
    add_variables = true
    strain = SMALL
  []
[]

# =============================================================================
# RIGID-BODY PINNING  --  READ THIS BEFORE RUNNING
# -----------------------------------------------------------------------------
# A 2-D body has THREE rigid-body modes: translation in x, translation in y, and
# in-plane rotation. A pure pressure (traction) load constrains NONE of them, so
# the stiffness matrix is SINGULAR: infinitely many displacement solutions differ
# only by a rigid-body motion, and the linear solver hits a ZERO PIVOT and fails
# (typical PETSc error: "Zero pivot ... factorization failed" or a singular-matrix
# / non-convergence error).
#
# We remove exactly those 3 modes with exactly 3 pinned DOFs:
#     pin_corner : disp_x = 0  AND  disp_y = 0   -> kills both translations
#     pin_roller : disp_y = 0                    -> kills rotation about the corner
# That is the MINIMUM restraint - the plate is otherwise free to deform under the
# load, so the computed stresses are the true response to the pressure alone.
#
# TRY IT: comment out the three pin BCs below and re-run. The solve will FAIL with
# a zero-pivot / singular-matrix error. Restore them and it converges.
# =============================================================================
[BCs]
  # --- Minimal pins (do NOT remove - see big comment above) ---
  [pin_corner_x]
    type = DirichletBC
    variable = disp_x
    boundary = pin_corner
    value = 0
  []
  [pin_corner_y]
    type = DirichletBC
    variable = disp_y
    boundary = pin_corner
    value = 0
  []
  [pin_roller_y]
    type = DirichletBC
    variable = disp_y
    boundary = pin_roller
    value = 0
  []

  # --- Applied load: uniform pressure pushing down on the top edge ---
  # The Pressure action applies a traction normal to the named boundary. A
  # constant 'factor' (in Pa) makes this a steady load (no time function needed).
  [Pressure]
    [top_load]
      boundary = top
      factor = 5e6
    []
  []
[]

[Materials]
  # Linear isotropic elasticity: build C_ijkl from E and nu...
  [elasticity]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 1e9
    poissons_ratio = 0.3
  []
  # ...then compute stress = C : (small strain).
  [stress]
    type = ComputeLinearElasticStress
  []
[]

# Tensor visualization: copy quantities out of the 'stress' RankTwoTensor into
# CONSTANT MONOMIAL aux fields (one value per element) so they can be plotted.
[AuxVariables]
  [vonmises]
    order = CONSTANT
    family = MONOMIAL
  []
  [stress_yy]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[AuxKernels]
  # RankTwoScalarAux: reduce the full stress tensor to a scalar invariant.
  # scalar_type = VonMisesStress gives the equivalent (von Mises) stress.
  [vonmises]
    type = RankTwoScalarAux
    rank_two_tensor = stress
    variable = vonmises
    scalar_type = VonMisesStress
  []
  # RankTwoAux: pull out a single component. index_i=1, index_j=1 -> stress_yy
  # (0-based indices: 0=x, 1=y), the component aligned with the applied pressure.
  [stress_yy]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_yy
    index_i = 1
    index_j = 1
  []
[]

# Postprocessors reduce an aux field to a single number, written to CSV.
[Postprocessors]
  # Peak von Mises stress anywhere in the plate.
  [max_vonmises]
    type = ElementExtremeValue
    variable = vonmises
  []
  # Volume-averaged von Mises stress.
  [avg_vonmises]
    type = ElementAverageValue
    variable = vonmises
  []
[]

# consider all off-diagonal Jacobian blocks for preconditioning
[Preconditioning]
  [SMP]
    type = SMP
    full = true
  []
[]

# Steady linear-elastic solve with a direct (LU) solver.
[Executioner]
  type = Steady
  solve_type = NEWTON
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
[]

[Outputs]
  exodus = true
  csv = true
[]
