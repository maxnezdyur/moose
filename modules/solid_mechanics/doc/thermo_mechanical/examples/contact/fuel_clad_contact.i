# =============================================================================
# THERMO-MECHANICAL GAP CONTACT EXEMPLAR  (3-D)
#   A heated fuel cylinder swells, closes a small radial gap onto its cladding,
#   presses on it (radial contact), AND -- because it is much hotter -- grows
#   TALLER than the cladding and SLIDES UP along the contact interface.
# =============================================================================
#
# This is a TEMPLATE for setting up mechanical gap contact in 3-D and for
# MEASURING the relative sliding between the two bodies. Change it for your case.
#
# Geometry: a 90-degree WEDGE of two concentric annular cylinders (fuel inside
# cladding) separated by a 0.1 mm radial gap. Each 2-D quarter-ring is extruded
# in z to height H; the two radial cut faces use symmetry, so the wedge behaves
# like the full cylinder at a quarter of the cost. (To model the full cylinder,
# set dmax = 360 on both rings and replace the two symmetry BCs with rigid-body
# pins -- a wedge is simpler because symmetry removes the rigid-body modes.)
#
#   build:  fuel quarter-ring  +  clad quarter-ring  ->  combine  ->  extrude(z)
#
#   surfaces (after extrusion):
#     fuel_rmin = fuel bore (inner)      clad_rmin = clad inner  (CONTACT)
#     fuel_rmax = fuel outer (CONTACT)   clad_rmax = clad outer
#     *_dmin    = theta=0 cut face       *_dmax    = theta=90 cut face
#     bottom    = z=0 (fixed in z)       top       = z=H (free)
#
# Physics (all thermal):
#   * constant volumetric heat source in the fuel,
#   * convective BC on the fuel bore and the cladding OD -- the only heat sinks;
#     the gap is left thermally open, so the fuel runs hot, the cladding cool.
#   * the hot fuel expands (thermal-expansion eigenstrain) far more than the cool
#     cladding: radially it closes the gap and contacts; axially it grows taller.
#
# Contact: frictionless PENALTY between fuel_rmax (secondary) and clad_rmin
#   (primary). Frictionless => radial pressure only, free to slide in z, so the
#   fuel "sweeps up" past the cladding.
#
# WHAT TO LOOK AT (console / CSV):
#   max_contact_pressure  > 0   -> the gap closed, they are touching
#   fuel_surface_rise           -> how far the fuel contact surface swept up (z)
#   clad_surface_rise           -> how far the cladding contact surface rose
#   axial_slip = fuel - clad    -> the relative SLIDING along the interface
#   the LineValueSampler CSVs give disp_z vs height up each surface (plot them:
#   the vertical gap between the two curves IS the slip at each height).
#
# Run:  combined-opt -i fuel_clad_contact.i
#
# NOTE: the material model is linear elastic, so the thermal stress in the very
# hot, constrained fuel is illustrative and unrealistically large (a real fuel
# relieves it by cracking/creep). The point is the CONTACT setup, the gap
# closing, and measuring the sliding -- not a validated stress number.
# =============================================================================

[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
[]

# -----------------------------------------------------------------------------
# Mesh: two 2-D quarter rings (distinct blocks + prefixed/offset boundary names
# so nothing collides), combined, then extruded once into 3-D. The single
# extrusion gives one shared bottom/top sideset used to anchor both bodies.
# -----------------------------------------------------------------------------
[Mesh]
  patch_update_strategy = iteration # refresh the contact search as the gap closes

  [fuel_ring]
    type = AnnularMeshGenerator
    rmin = 0.004 # fuel bore radius (m)
    rmax = 0.010 # fuel outer radius (m)
    dmin = 0
    dmax = 90
    nr = 6
    nt = 12
    quad_subdomain_id = 1
    tri_subdomain_id = 101 # unused (annulus has no center tris); avoids the quad==tri check
    boundary_name_prefix = fuel
  []
  [clad_ring]
    type = AnnularMeshGenerator
    rmin = 0.0101 # cladding inner radius (0.1 mm gap outside the fuel)
    rmax = 0.0116 # cladding outer radius (m)
    dmin = 0
    dmax = 90
    nr = 3
    nt = 12
    quad_subdomain_id = 2
    tri_subdomain_id = 102 # unused; avoids the quad==tri check
    boundary_name_prefix = clad
    boundary_id_offset = 10 # keep cladding boundary IDs from colliding with the fuel's
  []
  [combine_2d]
    type = CombinerGenerator
    inputs = 'fuel_ring clad_ring'
  []
  [extrude]
    type = AdvancedExtruderGenerator
    input = combine_2d
    direction = '0 0 1'
    heights = '0.10' # cylinder height (m)
    num_layers = '20'
    bottom_boundary = 1000
    top_boundary = 2000
  []
  [name_blocks]
    type = RenameBlockGenerator
    input = extrude
    old_block = '1 2'
    new_block = 'fuel cladding'
  []
  [name_ends]
    type = RenameBoundaryGenerator
    input = name_blocks
    old_boundary = '1000 2000'
    new_boundary = 'bottom top'
  []
[]

[Variables]
  [temperature]
    initial_condition = 300 # K
  []
[]

# -----------------------------------------------------------------------------
# Mechanics: the QuasiStatic action adds disp_x/y/z, the equilibrium kernels,
# and the small-strain calculator. `temperature` + `eigenstrain_names` wire the
# thermal-expansion coupling (and its off-diagonal Jacobian) into the solve.
# -----------------------------------------------------------------------------
[Physics/SolidMechanics/QuasiStatic]
  [all]
    add_variables = true
    strain = SMALL
    eigenstrain_names = eigenstrain
    temperature = temperature
    generate_output = 'vonmises_stress stress_zz'
  []
[]

[Kernels]
  [conduction]
    type = HeatConduction
    variable = temperature
  []
  [fuel_heat]
    type = HeatSource
    variable = temperature
    block = fuel
    value = 1.0e8 # W/m^3 fission-like volumetric heating (fuel only)
  []
[]

# -----------------------------------------------------------------------------
# Mechanical contact: this single block is the thing the students change.
# -----------------------------------------------------------------------------
[Contact]
  [fuel_clad]
    primary = clad_rmin # cladding inner surface
    secondary = fuel_rmax # fuel outer surface (the surface that swells into the clad)
    model = frictionless # transmits radial pressure only -> free to slide in z
    formulation = penalty
    penalty = 1e13 # contact stiffness: bigger = less penetration, harder to converge
    normalize_penalty = true # scale the penalty by nodal area (recommended)
    tangential_tolerance = 1e-4
  []
[]

[BCs]
  # --- thermal: the only two heat sinks (gap is left open / insulated) ---
  [bore_cooling]
    type = ConvectiveHeatFluxBC
    variable = temperature
    boundary = fuel_rmin # fuel inner bore
    T_infinity = 500 # inner coolant temperature (K)
    heat_transfer_coefficient = 6000 # W/m^2-K
  []
  [outer_cooling]
    type = ConvectiveHeatFluxBC
    variable = temperature
    boundary = clad_rmax # cladding outer surface
    T_infinity = 400 # outer coolant temperature (K)
    heat_transfer_coefficient = 3000 # W/m^2-K
  []

  # --- mechanical: quarter-symmetry on the cut faces + anchor the bottom in z ---
  [sym_y] # theta = 0 cut face lies in the x-z plane -> no y motion
    type = DirichletBC
    variable = disp_y
    boundary = 'fuel_dmin clad_dmin'
    value = 0
  []
  [sym_x] # theta = 90 cut face lies in the y-z plane -> no x motion
    type = DirichletBC
    variable = disp_x
    boundary = 'fuel_dmax clad_dmax'
    value = 0
  []
  [fix_bottom_z] # the "one side that does not move" -> they sweep upward from here
    type = DirichletBC
    variable = disp_z
    boundary = bottom
    value = 0
  []
[]

[Materials]
  # thermal conductivity per block (low-k fuel runs hot, higher-k cladding)
  [fuel_thermal]
    type = HeatConductionMaterial
    block = fuel
    thermal_conductivity = 3.0 # W/m-K
    specific_heat = 300
  []
  [clad_thermal]
    type = HeatConductionMaterial
    block = cladding
    thermal_conductivity = 16.0 # W/m-K
    specific_heat = 400
  []

  # elasticity per block
  [fuel_elasticity]
    type = ComputeIsotropicElasticityTensor
    block = fuel
    youngs_modulus = 200e9
    poissons_ratio = 0.3
  []
  [clad_elasticity]
    type = ComputeIsotropicElasticityTensor
    block = cladding
    youngs_modulus = 80e9
    poissons_ratio = 0.3
  []
  [stress]
    type = ComputeLinearElasticStress
  []

  # thermal-expansion eigenstrain ("swelling") per block: fuel >> cladding
  [fuel_thermal_strain]
    type = ComputeThermalExpansionEigenstrain
    block = fuel
    eigenstrain_name = eigenstrain
    temperature = temperature
    stress_free_temperature = 300
    thermal_expansion_coeff = 2.0e-5 # 1/K  (fuel swells a lot)
  []
  [clad_thermal_strain]
    type = ComputeThermalExpansionEigenstrain
    block = cladding
    eigenstrain_name = eigenstrain
    temperature = temperature
    stress_free_temperature = 300
    thermal_expansion_coeff = 5.0e-6 # 1/K  (cladding barely moves)
  []
[]

[Postprocessors]
  [fuel_surf_temp] # hottest part of the fuel is its insulated outer (contact) face
    type = NodalExtremeValue
    variable = temperature
    boundary = fuel_rmax
  []
  [clad_surf_temp]
    type = NodalExtremeValue
    variable = temperature
    boundary = clad_rmin
  []
  # --- proof of contact (radial): positive only where the faces press together
  [max_contact_pressure]
    type = NodalExtremeValue
    variable = contact_pressure
    boundary = fuel_rmax
  []
  [max_penetration] # how far the fuel surface overlaps the cladding (penalty is finite)
    type = NodalExtremeValue
    variable = penetration
    boundary = fuel_rmax
  []
  # --- THE SLIDING MEASUREMENT (axial) ---
  # disp_z at the TOP of each contact face (anchored at z=0, free at z=H). The
  # fuel face rides UP; the cladding face is squeezed by the contact and actually
  # creeps slightly DOWN, so the true relative slip is the difference of the two
  # (fuel positive minus cladding negative) -- they move apart in z.
  [fuel_surface_rise]
    type = PointValue
    variable = disp_z
    point = '0.007036 0.007036 0.0999' # top of fuel outer face (r=0.00995, 45 deg)
  []
  [clad_surface_rise]
    type = PointValue
    variable = disp_z
    point = '0.007177 0.007177 0.0999' # top of clad inner face (r=0.01015, 45 deg)
  []
  [axial_slip] # relative sliding = how far the fuel slid up past the cladding
    type = DifferencePostprocessor
    value1 = fuel_surface_rise
    value2 = clad_surface_rise
  []
  [max_vonmises]
    type = ElementExtremeValue
    variable = vonmises_stress
  []
[]

[VectorPostprocessors]
  # disp_z up each contact surface vs height (sampled at theta=45 deg, just inside
  # each body) -> plot both; the vertical distance between the curves at any
  # height is the local sliding there.
  [fuel_interface]
    type = LineValueSampler
    variable = disp_z
    start_point = '0.007036 0.007036 0.0' # r=0.00995, 45 deg
    end_point = '0.007036 0.007036 0.10'
    num_points = 41
    sort_by = z
  []
  [clad_interface]
    type = LineValueSampler
    variable = disp_z
    start_point = '0.007177 0.007177 0.0' # r=0.01015, 45 deg
    end_point = '0.007177 0.007177 0.10'
    num_points = 41
    sort_by = z
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
  petsc_options_iname = '-pc_type -pc_factor_shift_type'
  petsc_options_value = 'lu NONZERO'
  line_search = contact # damps contact set "chatter" (nodes flicking in/out of contact)
  automatic_scaling = true # balance the temperature, displacement, and contact residuals
  nl_rel_tol = 1e-7
  nl_abs_tol = 1e-6
  nl_max_its = 100
  l_max_its = 100
[]

[Outputs]
  exodus = true
  csv = true
[]
