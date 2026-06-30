# ==============================================================================
# CHT coupling example - THE FLUID SUB-APP ("coolant" stand-in)
# ------------------------------------------------------------------------------
# This is the SUB-APP in a conjugate-heat-transfer (CHT) MultiApp coupling. It
# represents the COOLANT/fluid channel that sits next to a heated solid wall and
# carries heat away. The parent app (cht_solid_parent.i) owns the coupling and
# launches this file once per fixed-point (Picard) iteration.
#
# *** IMPORTANT: this is a TEACHING STAND-IN, not a CFD solve. ***
#   We are demonstrating the [MultiApps]/[Transfers]/fixed-point machinery, NOT
#   Navier-Stokes. So the "fluid" here is modelled with PLAIN HEAT CONDUCTION on
#   a stationary block. In a real problem you would SWAP THIS ENTIRE FILE for an
#   incompressible Navier-Stokes (INS) / finite-volume CFD input that solves the
#   momentum + energy equations on a flowing coolant (see, e.g.,
#   modules/navier_stokes/test/tests/finite_volume/ins/cht/conjugate_heat_transfer).
#   The COUPLING INTERFACE this file exposes to the parent would stay the same:
#     - it RECEIVES the solid wall interface temperature  -> T_solid_if (aux)
#     - it applies a convective (Robin) BC at the shared 'interface' sideset
#     - the parent PULLS this block's interface temperature back out of T.
#
# Geometry: a 2-D channel block occupying x in [1, 2], y in [0, 1]. Its LEFT face
# (x = 1) is the shared 'interface' with the solid (the solid lives in x in [0,1],
# so the two faces are COINCIDENT - that is what lets the field transfers map
# point-to-point). Its RIGHT face (x = 2) is the cold coolant inlet/outlet that
# actually drains heat out of the system.
#
# Standalone-runnable: T_solid_if has an initial_condition, so this file solves
# on its own (check-input and a real solve both work) without the parent.
# ==============================================================================

# Coupling / physics constants (kept at top, day2 style).
#
# NOTE on the choice of htc relative to k_fluid: this segregated (Robin-Robin)
# coupling is solved by fixed-point/Picard iteration, whose convergence rate is
# roughly htc / (htc + k/L). A very large htc (near-perfect contact) makes the
# two interface temperatures nearly equal but makes Picard converge SLOWLY; a
# small htc converges fast but leaves a large contact-resistance jump. The values
# below are tuned so the loop converges comfortably inside 10 iterations while
# the interface temperatures still come out close.
k_fluid = 20.0 # fluid (coolant) effective thermal conductivity  [W/(m.K)]
htc = 12.0 # interface heat-transfer coefficient (gap conductance) [W/(m^2.K)]
T_coolant = 500.0 # coolant sink temperature at the far (x=2) face       [K]
T_solid_if_init = 560.0 # initial guess for the solid wall interface temperature [K]

[Mesh]
  # The fluid channel block. Note xmin = 1: it is placed to the RIGHT of the
  # solid so the two meshes share the plane x = 1 with no overlap.
  [channel]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 20
    ny = 20
    xmin = 1.0
    xmax = 2.0
    ymin = 0.0
    ymax = 1.0
  []
  # Name the left face (x = 1) 'interface' - this is the CHT coupling sideset and
  # must match the sideset name the parent transfers to/from.
  [name_interface]
    type = RenameBoundaryGenerator
    input = channel
    old_boundary = 'left'
    new_boundary = 'interface'
  []
[]

[Variables]
  # Fluid temperature - the field this sub-app actually solves for.
  [T]
    initial_condition = ${T_coolant}
  []
[]

[AuxVariables]
  # The SOLID wall's interface temperature, RECEIVED from the parent app each
  # fixed-point iteration via a MultiAppGeneralField...Transfer. The Robin BC
  # below uses it as the far-field temperature T_infinity. The initial_condition
  # is what makes this file runnable on its own (before any transfer happens).
  [T_solid_if]
    initial_condition = ${T_solid_if_init}
  []
[]

[Kernels]
  # Steady heat conduction in the fluid block: -div(k grad T) = 0.
  [conduction]
    type = HeatConduction
    variable = T
  []
[]

[BCs]
  # Shared interface (x = 1): convective/Robin coupling to the solid. The fluid
  # surface exchanges heat with the OTHER side's interface temperature
  # (T_solid_if) through the gap conductance htc. This is one half of the
  # Robin-Robin pair; the solid parent applies the mirror-image BC.
  [interface_convection]
    type = CoupledConvectiveHeatFluxBC
    variable = T
    boundary = 'interface'
    T_infinity = T_solid_if # far-field = solid wall interface temp (from parent)
    htc = ${htc}
  []
  # Cold coolant inlet/outlet (x = 2): a fixed cold temperature that lets the
  # fluid actually CARRY HEAT AWAY (otherwise the block would just equilibrate to
  # the solid temperature and nothing would be transported).
  [coolant_sink]
    type = DirichletBC
    variable = T
    boundary = 'right'
    value = ${T_coolant}
  []
  # top/bottom (y = 0, y = 1) are left as natural (insulated) BCs.
[]

[Materials]
  # Constant fluid conductivity. HeatConductionMaterial also declares the
  # thermal_conductivity_dT property (= 0 here) the HeatConduction kernel expects.
  [fluid_props]
    type = HeatConductionMaterial
    thermal_conductivity = ${k_fluid}
    specific_heat = 1.0
  []
[]

[Postprocessors]
  # Fluid-side interface average temperature. The parent reads the equivalent of
  # this (via transfer) to compare the two sides and confirm a continuous
  # interface temperature at convergence.
  [T_fluid_interface_avg]
    type = SideAverageValue
    variable = T
    boundary = 'interface'
  []
[]

[Executioner]
  type = Steady
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
  nl_rel_tol = 1e-9
  nl_abs_tol = 1e-8
[]

[Outputs]
  exodus = true
[]
