# Day 2 - Mechanics "anatomy": uniaxial tension of a single elastic block.
#
# This is the simplest complete solid-mechanics problem and shows every piece a
# MOOSE mechanics input needs: a mesh, the SolidMechanics action (variables +
# equilibrium kernels + strain), an elasticity tensor, a stress model, boundary
# conditions, and postprocessed outputs.
#
# Setup: a 2-D rectangular block is held on its left and bottom faces by rollers
# (each is free to slide along the face but cannot move through it) and pulled in
# +x by prescribing a fixed displacement on the right face. The result is a
# uniform uniaxial stress state we can check by hand.

# Displacement variables shared by the action, kernels, and BCs. Naming them here
# once lets every block below refer to the same unknowns.
[GlobalParams]
  displacements = 'disp_x disp_y'
[]

# Geometry: a 1 m x 0.2 m rectangle. The mesh generator also creates the named
# sidesets (left/right/bottom/top) we apply boundary conditions to.
[Mesh]
  [block]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 20
    ny = 4
    xmax = 1.0 # length pulled in the x-direction (m)
    ymax = 0.2 # height (m)
  []
[]

# SolidMechanics QuasiStatic action: the workhorse that assembles the mechanics.
# It (1) adds the disp_x/disp_y variables, (2) adds the stress-divergence
# equilibrium kernels, and (3) adds the strain calculator.
[Physics/SolidMechanics/QuasiStatic]
  [all]
    add_variables = true
    # SMALL strain: linearized (infinitesimal) kinematics. Valid here because the
    # imposed strain is tiny (0.1%); pair it with ComputeLinearElasticStress.
    strain = SMALL
    # Ask the action to also output these stress/strain components as
    # AuxVariables so we can visualize and postprocess them:
    #   vonmises_stress   - scalar measure used for yield checks (see below)
    #   stress_xx         - axial (loading-direction) Cauchy stress
    #   strain_xx         - axial strain
    #   strain_yy         - transverse strain; the Poisson contraction
    #                       (~ -nu*strain_xx = -0.3e-3) is visible as a field
    #   stress_yy         - transverse stress; ~0 here, confirming a true
    #                       uniaxial *stress* state (not uniaxial strain)
    #   elastic_strain_xx - axial elastic strain; equals strain_xx because there
    #                       is no eigenstrain yet (sets up thermal coupling later)
    generate_output = 'vonmises_stress stress_xx strain_xx strain_yy stress_yy elastic_strain_xx'
  []
[]

[BCs]
  # Roller on the left face: fixes only the normal (x) component, so the face
  # cannot move in x but is free to contract in y (Poisson effect).
  [left_roller_x]
    type = DirichletBC
    variable = disp_x
    boundary = left
    value = 0
  []
  # Roller on the bottom face: fixes only the y component. Together with the left
  # roller this removes all rigid-body motion without over-constraining the block.
  [bottom_roller_y]
    type = DirichletBC
    variable = disp_y
    boundary = bottom
    value = 0
  []
  # Load: prescribe a fixed x-displacement on the right face (displacement
  # control). 1e-3 m over a 1 m length is a uniform axial strain of 0.001.
  [pull_right_x]
    type = DirichletBC
    variable = disp_x
    boundary = right
    value = 1e-3
  []
[]

[Materials]
  # Hooke's law, part 1 - the elasticity tensor. For an isotropic material it is
  # built from just two constants, Young's modulus E and Poisson's ratio nu.
  [elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 200e9 # E (Pa), ~ steel
    poissons_ratio = 0.3   # nu
  []
  # Hooke's law, part 2 - linear elastic stress: stress = C : (elastic strain).
  # Expected here: stress_xx ~ E * strain_xx = 200e9 * 1e-3 = 2.0e8 Pa.
  [stress]
    type = ComputeLinearElasticStress
  []
[]

[Postprocessors]
  # Volume-averaged von Mises stress. For uniaxial tension the von Mises stress
  # equals the axial stress, so this should be ~2.0e8 Pa.
  [avg_vonmises]
    type = ElementAverageValue
    variable = vonmises_stress
  []
  # Sample the axial stress at the block center to confirm the uniform field.
  [stress_xx_center]
    type = PointValue
    variable = stress_xx
    point = '0.5 0.1 0'
  []
[]

# Steady problem (no time dependence) solved with a full Newton iteration. The LU
# direct solver is robust and fast for a problem this small.
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
