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
#   * convective BC on the fuel bore and the cladding OD -- the two heat sinks;
#   * MODULAR (mortar) GAP CONDUCTANCE across the gap, so heat conducts from the
#     hot fuel into the cladding (see the THERMAL GAP CONDUCTANCE block below);
#     the cladding now runs well above its coolant temperature.
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

  # --- MORTAR THERMAL GAP: lower-dimensional interface blocks --------------
  # The modular (mortar) gap-conductance constraint lives on a pair of
  # lower-dimensional element blocks, one "painted" onto each side of the gap.
  # These EDGE/QUAD shells carry only the Lagrange multiplier and the gap
  # flux; they hold NO bulk kernels (see [Problem] kernel_coverage_check).
  # Chain them onto the last existing generator so the rest of the mesh is
  # untouched. NOTE: these share the SAME sidesets (fuel_rmax / clad_rmin) as
  # the node-face penalty mechanical contact -- the two constraints coexist.
  [fuel_lower] # secondary side: a shell on the fuel outer (contact) face
    type = LowerDBlockFromSidesetGenerator
    input = name_ends
    sidesets = 'fuel_rmax'
    new_block_id = 10001
    new_block_name = 'fuel_lower'
  []
  [clad_lower] # primary side: a shell on the cladding inner face
    type = LowerDBlockFromSidesetGenerator
    input = fuel_lower
    sidesets = 'clad_rmin'
    new_block_id = 10000
    new_block_name = 'clad_lower'
  []
[]

# The lower-dimensional gap blocks carry no bulk kernels, so relax the check
# that every subdomain is covered by a kernel.
[Problem]
  kernel_coverage_check = false
[]

[Variables]
  [temperature]
    initial_condition = 300 # K
    block = 'fuel cladding' # solid blocks only -- NOT the lower-d gap shells
  []
  # Lagrange multiplier for the modular gap-conductance constraint. It is the
  # interfacial heat flux unknown; it lives ONLY on the secondary gap shell.
  [lm]
    order = FIRST
    family = LAGRANGE
    block = 'fuel_lower'
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
    block = 'fuel cladding' # keep displacement vars/kernels off the lower-d gap shells
  []
[]

[Kernels]
  [conduction]
    type = HeatConduction
    variable = temperature
    block = 'fuel cladding' # solid blocks only -- the gap shells carry no bulk kernels
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

# =============================================================================
# THERMAL GAP CONDUCTANCE (modular / mortar)  -- THE ADDITION
# -----------------------------------------------------------------------------
# Previously the gap was thermally OPEN: no heat crossed it, so the cladding
# floated at its coolant temperature (~400 K) while the fuel cooked. Here we
# let heat conduct across the gap from the hot fuel into the cladding using the
# MORTAR modular gap-conductance pattern (independent of the mechanical contact
# above, which stays node-face penalty):
#
#   gap gas (UserObject)  ->  gap flux model: q = k * (T_clad - T_fuel) / gap
#   constraint            ->  ties that flux between the two gap shells via lm
#
# Heat now flows  fuel interior -> fuel_rmax -> [gap] -> clad_rmin -> clad_rmax
# -> coolant, so the CLADDING heats up well above the old ~400 K baseline.
# -----------------------------------------------------------------------------
[UserObjects]
  # Gap "gas" conduction model. GapFluxModelSimple returns a heat flux
  #   q = k * (T_primary - T_secondary) / gap_width
  # so k is the GAP-GAS CONDUCTIVITY (W/m-K) and the effective conductance is
  # k / gap. With the 0.1 mm (1e-4 m) reference gap this gives k/1e-4 = 300
  # W/m^2-K. k is deliberately modest: a stronger gap short would bleed so much
  # heat out of the fuel that it would no longer swell enough to close the gap
  # mechanically -- this value heats the cladding visibly (~+55 K) while the
  # fuel stays hot enough to keep the node-face contact engaged. (Switch to
  # GapFluxModelConduction for an explicit gas-conductivity-vs-temperature law.)
  [gap_gas]
    type = GapFluxModelSimple
    k = 0.03 # gap-gas thermal conductivity (W/m-K) -> ~300 W/m^2-K across the gap
    temperature = temperature
    boundary = clad_rmin # the primary side of the gap
    use_displaced_mesh = false # match the constraint -> use the reference gap width
  []
[]

[Constraints]
  # Modular gap-conductance constraint: enforces the gap heat flux (built from
  # the gap_gas model) between the two lower-d shells, with the Lagrange
  # multiplier `lm` as the interfacial flux unknown. Evaluated on the
  # UNDISPLACED mesh (use_displaced_mesh = false) so the gap width is the clean
  # 1e-4 m reference gap -- robust, and the flux magnitude is set by k above.
  [thermal_gap]
    type = ModularGapConductanceConstraint
    variable = lm
    secondary_variable = temperature
    primary_boundary = clad_rmin
    primary_subdomain = clad_lower
    secondary_boundary = fuel_rmax
    secondary_subdomain = fuel_lower
    gap_flux_models = 'gap_gas'
    use_displaced_mesh = false
    displacements = '' # override GlobalParams: gap width is taken on the reference mesh
    correct_edge_dropping = true # 3-D mortar: tolerate non-matching surface edges
  []
[]

[BCs]
  # --- thermal: the two heat sinks (the gap now conducts -- see [Constraints]) ---
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
    block = 'fuel cladding' # solid blocks only -- no stress on the lower-d gap shells
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
  # --- gap-conductance proof: peak temperature in each whole block. With the
  # gap thermally open clad_max_temp sat at ~400 K; once the gap conducts it
  # rises clearly above that.
  [clad_max_temp]
    type = ElementExtremeValue
    variable = temperature
    block = cladding
  []
  [fuel_max_temp]
    type = ElementExtremeValue
    variable = temperature
    block = fuel
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
  # Direct solve: the mortar gap constraint adds a Lagrange-multiplier
  # (saddle-point) block with zeros on the diagonal, which the previous gamg
  # multigrid cannot handle -- a sparse LU factorization is the robust choice
  # for a mortar problem of this size.
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_type'
  petsc_options_value = ' lu       superlu_dist'
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
