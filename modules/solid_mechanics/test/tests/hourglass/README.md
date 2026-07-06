# Hourglass control test suite

Two families live here:

1. The explicit-dynamics QUAD4/HEX8 hourglass-correction kernels
   (HourglassCorrectionQuad4 ported from dschwen/hourglass_correction_29852,
   HourglassCorrectionHex8) with mode/affine/beam tests.

2. The IMPLICIT scalar (diffusion) reduced-integration ladder for
   HourglassCorrectionDiffusion, built phase-by-phase with a gate per phase
   (all numbers measured on this fork, distorted-quad meshes):

   - Phase 0 (baseline): full 2x2 integration; single distorted element has
     exactly ONE zero eigenvalue {0, 1.404, 2.400, 3.281}; MMS L2 rate 2.0.
     Driver: thermal_single_elem.i + -ksp_view_mat.
   - Phase 1 (measured failure): one-point integration has TWO zero
     eigenvalues {0, 0, 1.954, 3.199} - the hourglass mode, quantified.
     At mesh scale with Dirichlet anchoring the global operator stays
     nonsingular (lowest eig 0.058) - the pathology appears as conditioning
     (162 vs 38 stabilized) and, decisively, in the transient test below.
   - Phase 2 (gamma): orthogonality gamma.1 = gamma.x = gamma.y = 0 at
     1.6e-15 on distorted quads (thermal_gamma_ortho.i, HourglassGamma.h
     closed-form Flanagan-Belytschko construction).
   - Phase 3 (stabilization): eps gamma gamma^T with eps = penalty*k*A/h^2
     (penalty default 1/6 = exact hourglass stiffness of the full element on
     rectangles); single-element rank restored: {0, 1.292, 2.281, 3.257}.
   - Phase 4 (consistency): linear patch test on distorted quads recovered to
     2.7e-16 with stabilization energy 2.7e-31 (thermal_patch.i).
   - Phase 5 (cure + accuracy): transient checkerboard IC
     (thermal_transient_hg.i): WITHOUT stabilization the mode has zero
     stiffness and persists forever (Tmax = 1, gamma-energy 426.67 frozen);
     with stabilization it is annihilated (1e-14 after one step); full
     integration decays physically. MMS (thermal_mms.i, h-scaled distortion):
     L2 rates full 2.01 / stabilized 2.02 / H1 2.07 - optimal rates preserved.
   - Phase 6 (k(T) consistent tangent): thermal_kT_newton.i with
     DerivativeParsedMaterial k = 2 + 0.5T: FD-vs-analytic Jacobian
     ||J-Jfd||/||J|| = 3e-9 (FD truncation); Newton quadratic
     (6.98 -> 1.02 -> 1.3e-2 -> 2.1e-6 -> 8.5e-12). Negative control
     (derivative_order=0): FD mismatch 2.1e-2, Newton degrades to linear.
   - Phase 5b (STEADY checkerboard, thermal_flux_pin.i): on a regular mesh
     the one-point operator couples only diagonal neighbors, splitting the
     node graph into two independent sublattices; flux loading with a single
     pinned node anchors only one of them. Unstabilized the assembled matrix
     is EXACTLY singular (LU: FACTOR_NUMERIC_ZEROPIVOT; Krylov returns
     arbitrary checkerboard, solution gamma-energy 4.14 and unphysical
     T_max = +0.197 on a 0..-0.4 field); stabilized it matches full
     integration to 12 digits (gamma-energy 3.6e-25). Note a full Dirichlet
     EDGE contains both node parities and anchors both sublattices - which is
     why Dirichlet-dominated problems hide this pathology.
   AD NOTE: the implicit kernel is an ADKernel - the consistent tangent,
   including every diffusivity dependence (k(T) diagonal, mu(T) off-diagonal
   in the mechanical blocks) comes from automatic differentiation. No
   derivative declarations (DerivativeParsedMaterial derivative_order, args
   coupling lists, eigenstrain/temperature Jacobian params on stress kernels)
   exist anywhere in the ladder inputs; the hand-tangent negative-control
   failure mode (missing declaration -> silent linear Newton) is impossible
   by construction. Gates re-verified on AD: kT FD 4.0e-9, coupled FD
   3.4-7.5e-11, Newton quadratic and trajectory-identical (same residual).

   - Phase 7 (thermomechanics, thermomech_coupled.i): the SAME kernel
     stabilizes the mechanical components (coefficient = shear modulus);
     off-diagonal d(coefficient)/d(coupled var) tangent added for mu(T).
     Gates: coupled FD-vs-analytic Jacobian 4.8e-11 at the initial state and
     3.7-5.7e-11 across all Newton states (negative control with dmu/dT
     dropped: 6.8e-8 - three orders); coupled Newton quadratic
     (0.97 -> 1.2e-2 -> 1.7e-6 -> 6.2e-12); stabilized 1-pt matches the
     full-integration reference to 0.02% (T_avg) / 0.5% (uy_max) on the
     coarse benchmark. PetscJacobianTester pins the tangent in regression.
   - Phase 8 (HEX8 scalar, hex_thermal_*.i): four FB gamma vectors built from
     the center-Jacobian gradient vectors (HourglassGamma::gammaHex8);
     orthogonality to {1,x,y,z} at 1.6e-16 on a heavily distorted hex.
     Single-element eigenvalues: full 1 zero mode; one-point FIVE zero modes
     (constant + 4 hourglass); stabilized back to 1. Mode weights
     {1/2,1/2,1/2,1/4} with the same penalty=1/6 default reproduce the full
     element's ENTIRE spectrum exactly on rectangular parallelepipeds
     ({0, k/6, k/3 x3, 1.25 x3} for k=2.5). 3D MMS: L2 rate 2.04 (optimal).
     DOCUMENTED LIMITATION: on distorted hexes the one-point BASE operator
     (center-gradient B(0)) carries an O(distortion) consistency error
     (patch err 1e-4 at 6% distortion, present WITHOUT stabilization at
     8e-4; the stabilization itself is exactly consistent - parallelepiped
     patch at 7.5e-16, energy 1.4e-30). Exact distorted-hex consistency
     requires the Flanagan-Belytschko MEAN-gradient element (B-bar =
     (1/V) integral of B), a base-operator upgrade beyond stabilization
     scope; pinned by the hex_patch_distorted regression.

   Dev drivers without spec entries (single-element eigenvalue dumps): run
   thermal_single_elem[_stab].i with
   -ksp_view_mat ascii:jac.m:ascii_matlab and eigendecompose; see the
   session worklog for the full procedure.
