# RigidBodyModes

!syntax description /UserObjects/RigidBodyModes

## Description

On an unconstrained or under-constrained ("floating") solid mechanics problem the
stiffness matrix is singular in the rigid body modes, which stalls an algebraic
multigrid (AMG) preconditioner. `RigidBodyModes` supplies those modes as the framework
near-null-space vectors, so AMG builds a coarse space that represents them and converges
in fewer linear iterations.

The object sizes the near-null-space itself from the number of modes it produces.
Do not set `near_null_space_dimension` in the `[Problem]` block; setting it to a
conflicting value is an error.

[!param](/UserObjects/RigidBodyModes/displacements) is required. List two displacement
variables in 2D or three in 3D. Any other count is an error.

### Modes Produced id=modes

The object writes one near-null-space vector per mode from the reference nodal
coordinates. A two-dimensional displacement field produces three modes: two translations
and one in-plane rotation. Evaluated at a node with reference coordinates $(x, y)$,

\begin{equation}
\mathbf{t}_x = (1,\ 0), \qquad
\mathbf{t}_y = (0,\ 1), \qquad
\mathbf{r}_z = (-y,\ x).
\end{equation}

A three-dimensional field produces six modes: three translations and three rotations
about the coordinate axes through the origin,

\begin{equation}
\begin{aligned}
\mathbf{t}_x &= (1,\ 0,\ 0), & \mathbf{t}_y &= (0,\ 1,\ 0), & \mathbf{t}_z &= (0,\ 0,\ 1), \\
\mathbf{r}_x &= (0,\ -z,\ y), & \mathbf{r}_y &= (z,\ 0,\ -x), & \mathbf{r}_z &= (-y,\ x,\ 0).
\end{aligned}
\end{equation}

Each rigid body mode is nonzero only in the displacement degrees of freedom; every other
degree of freedom stays zero.

## Constant Modes for Coupled Fields id=constant-modes

Multiphysics solves often couple the displacements to a diffusion-like field such as
temperature. The lowest-energy near-null-space mode of a Laplacian (heat-conduction)
block is the constant function. If the near-null-space left that field's degrees of
freedom at zero, AMG would have no coarse representation of the block, and its
convergence on that field would degrade.

List each such field in [!param](/UserObjects/RigidBodyModes/constant_mode_variables).
Each listed variable receives one additional near-null-space mode, equal to 1 on that
variable's degrees of freedom and 0 elsewhere. The modes stay block-structured: rigid
body modes occupy only the displacement degrees of freedom, and each constant mode
occupies only its own variable's, so no mode mixes fields. A variable cannot appear in
both [!param](/UserObjects/RigidBodyModes/displacements) and
[!param](/UserObjects/RigidBodyModes/constant_mode_variables).

## Enabling the Near-Null-Space id=enabling

Two setups produce the same near-null-space, and neither needs a `[Problem]` parameter:

- Add `RigidBodyModes` in the `[UserObjects]` block and couple the displacements (and any
  constant-mode variables).
- Set `rigid_body_near_null_space = true` on the
  [QuasiStaticSolidMechanicsPhysics.md] action. The action builds the object from its own
  displacements and forwards its `temperature`, when present, as a constant mode.

## Solver Support id=solver

`RigidBodyModes` only fills the near-null-space vectors. It does not select or configure a
preconditioner, so which solver uses the vectors depends on how the preconditioner is set
up:

- PETSc GAMG (`-pc_type gamg`) consumes the near-null-space automatically.
- hypre BoomerAMG uses it only when you also set `-pc_hypre_boomeramg_nodal_coarsen` (1-6)
  and `-pc_hypre_boomeramg_vec_interp_variant` (1-3), typically together with
  `-pc_hypre_boomeramg_interp_type ext+i`. Without those options BoomerAMG ignores the
  vectors silently. See [Hypre/BoomerAMG](hypre.md) for the option details.

## Example Input File Syntax

Add the object directly and couple the displacements. This example also gives the coupled
temperature field its own constant mode:

!listing modules/solid_mechanics/test/tests/userobjects/rigid_body_modes/free_thermal_block.i block=UserObjects/rigid_body_modes

The [QuasiStaticSolidMechanicsPhysics.md] action produces the same near-null-space from a
single parameter:

!listing modules/solid_mechanics/test/tests/userobjects/rigid_body_modes/physics_action.i block=Physics/SolidMechanics/QuasiStatic

!syntax parameters /UserObjects/RigidBodyModes

!syntax inputs /UserObjects/RigidBodyModes

!syntax children /UserObjects/RigidBodyModes
