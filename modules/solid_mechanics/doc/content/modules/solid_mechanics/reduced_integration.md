# Reduced Integration with Hourglass Stabilization

Reduced integration evaluates a QUAD4 or a HEX8 at one quadrature point instead of four or eight.
That removes both the shear locking and the volumetric locking of a fully integrated linear element,
at the price of a rank-deficient element operator that has to be stabilized. This page covers when
to reach for the scheme, how to set it up, and what enabling it changes for the rest of the block.
[HourglassStabilization.md] carries the stabilization construction itself.

## Quadrature belongs to the block, not to a kernel id=block-wide

The quadrature rule is a property of the assembly and the subdomain. MOOSE offers no way to
integrate one variable differently from another on the same block, so one-point quadrature is never
a property of the mechanics -- it is a property of the block. Enabling it changes the integration
rule for every material, aux kernel and postprocessor there, and leaves every other
second-order-operator field on that block rank-deficient in exactly the same way the displacements
are.

The numbers users watch move as a result. On the coupled thermomechanical test problem the element
average von Mises stress reads 34.3 at one quadrature point against 214.9 at full quadrature, a
factor of six. Stress recovery and any element-average postprocessor now sample one point per
element. That is the correct answer for the rule in force, but it is not the number the same input
reported before.

Scoping takes two separate steps, and only one of them is the kernel's. The quadrature rule is
scoped with [!param](/Executioner/Quadrature/custom_blocks) and
[!param](/Executioner/Quadrature/custom_orders), which is what actually leaves the rest of the mesh
at full quadrature. [!param](/Kernels/HourglassStabilization/block) then restricts the stabilization
to those same blocks. Setting only the kernel's `block` gives one-point integration everywhere with
stabilization on a subset -- the worst of both. Treat every field on a reduced block as needing its
own stabilization; see [#coupled] below.

## Restoring rank id=rank

At one quadrature point a QUAD4 supplies rank 2 of the 3 its operator needs and admits one spurious
zero-energy mode; a HEX8 supplies rank 3 of 7 and admits four. [HourglassStabilization.md]
penalizes the amplitude of the classical Flanagan-Belytschko mode vectors
[!cite](flanagan1981) after projecting the affine field space out of them, which restores rank while
leaving affine fields, the patch test and rigid-body motion exactly unstabilized. One kernel
stabilizes one field, so a 3D mechanics block needs three.

The [QuasiStatic Physics](QuasiStaticSolidMechanicsPhysics.md) generates those kernels from a single
[!param](/Physics/SolidMechanics/QuasiStatic/QuasiStaticSolidMechanicsPhysics/reduced_integration)
flag. It generates kernels only; it does not set the quadrature rule.

## Requesting one-point quadrature id=quadrature

Nothing in the C++ lowers the rule, because MOOSE's object-level quadrature API can only raise the
order of a rule and never lower it. The only route down to one point is the executioner, which the
user writes:

!listing modules/solid_mechanics/test/tests/reduced_integration/conduction/thermomechanical.i
         block=Executioner/Quadrature

`element_order = CONSTANT` produces exactly one Gauss point on both element types. So does
`FIRST`: one-point Gauss is exact through degree one, so libMesh returns the same rule for both. The stabilization kernel counts the points of the rule it is actually handed, once
per element, and errors with this block as pasteable remediation text if the count is not one, so a
rule that quietly stayed at full quadrature fails loudly rather than returning a wrong answer.

For a mesh where only some blocks should be under-integrated, set
[!param](/Executioner/Quadrature/custom_blocks) and
[!param](/Executioner/Quadrature/custom_orders) in that same
[Quadrature](/Executioner/Quadrature/index.md) block rather than writing new code.

## Choosing the hourglass penalty id=penalty

[!param](/Kernels/HourglassStabilization/penalty) defaults to 0.1. The guidance is two-part, because
the parameter's influence is a coarse-mesh phenomenon: on a coarse mesh it materially moves the
answer, and on a resolved mesh any reasonable value works.

### On a coarse mesh id=penalty-coarse

[penalty_table] reports the tip displacement of the bending cantilever as a percentage of its
mesh-converged value, so 100% is the target, above 100% is under-stabilized and below it is
over-stabilized.

!table id=penalty_table caption=Cantilever tip displacement as a percentage of the mesh-converged value, by penalty and coarse mesh.
| [!param](/Kernels/HourglassStabilization/penalty) | 6x4   | 10x2  | 20x2  | 5x2   | 10x1  |
|---------------------------------------------------|-------|-------|-------|-------|-------|
| 0.05                                              | 103.1 | 119.2 | 124.2 | 116.3 | 473.3 |
| 0.1                                               | 100.4 | 108.2 | 116.5 | 104.1 | 237.0 |
| 0.2                                               | 95.3  | 91.5  | 103.7 | 86.1  | 118.9 |

There is no plateau: the response is monotonic and sensitive over the whole range. A three-decade
sweep on the 6x4 mesh gives 105.8% at a penalty of 0.005, 103.1% at 0.05 and 82.9% at 0.5, so 23
percentage points across the range. The sensitivity has a stiff side and not only a soft one: at 0.2
three of the five meshes have crossed from too soft to too stiff, with 5x2 landing 14% under, while
the two pathological aspect ratios improve. No single value wins on every mesh.

0.1 is the default for three reasons rather than one. It is the top of the classical 0.03 to 0.1
window [!cite](flanagan1981), which makes it a literature value rather than a fitted one. It is the best of the three on
the well-shaped coarse meshes. And its error there is one-sided -- always too soft -- and bounded,
which is easier to reason about than a 0.2 error that changes sign with element shape; 0.2 also sits
outside the classical window with no literature behind it.

The lower bound is an accuracy bound, not a singularity bound. Nothing failed to converge at 0.005;
that value was simply too soft. Conditioning nevertheless improves monotonically with the penalty,
and near-linearly, as $c = \texttt{penalty} \, \mu / h^2$ predicts: on the 10x1 cantilever the
minimum singular value of the assembled operator rises from 4.50e-12 at a penalty of zero, a
condition number of 1.08e17 and numerically singular, to 36.9 at 0.05, a condition number of 7.17e3.

Where the only pathology is volumetric the penalty barely matters even on a coarse mesh: the 8x8
confined block stays within 98.0% to 101.1% of its converged value across penalties from 0 to 1.0.
It is the bending-dominated, high-aspect-ratio cases that make the choice matter.

### On a resolved mesh id=penalty-fine

[penalty_fine_table] repeats the sweep on a 60x40 cantilever at $\nu = 0.3$ -- 2400 elements at the
same 6.67 element aspect ratio as the 6x4 case, so a clean tenfold refinement.

!table id=penalty_fine_table caption=Cantilever tip displacement as a percentage of the mesh-converged value on a 60x40 mesh.
| Scheme                     | % converged |
|----------------------------|-------------|
| full                       | 98.89       |
| B-bar                      | 99.00       |
| reduced, penalty = 0.005   | 100.06      |
| reduced, penalty = 0.05    | 100.03      |
| reduced, penalty = 0.1     | 100.00      |
| reduced, penalty = 0.2     | 99.94       |
| reduced, penalty = 0.5     | 99.78       |
| reduced, penalty = 1.0     | 99.51       |

A 200-fold range of the penalty spans 0.55 percentage points here, against 23 points on 6x4.
Refinement, not tuning, is the reliable fix: a user who sees the answer move with the penalty is
being told that the mesh is too coarse for the bending it is asked to resolve. 0.1 lands on 100.00%
exactly on this mesh, which completes the basis for the default.

## Comparison against full and B-bar quadrature id=comparison

[scheme_table] measures three schemes on two geometries at two Poisson's ratios: full quadrature,
full quadrature with the [volumetric locking correction](VolumetricLocking.md) (B-bar), and one-point
quadrature with hourglass stabilization. The block is 8x8 and laterally confined under a graded
surface pressure, where the only pathology is volumetric; the cantilever is 6x4 with a high element
aspect ratio, where shear locking is present as well.

!table id=scheme_table caption=Characteristic displacement as a percentage of the mesh-converged value, on an 8x8 block and a 6x4 cantilever.
| Geometry   | $\nu$ | Full | B-bar | Reduced |
|------------|-------|------|-------|---------|
| block      | 0.3   | 99.8 | 100.1 | 100.3   |
| block      | 0.499 | 50.2 | 100.7 | 101.0   |
| cantilever | 0.3   | 49.8 | 51.2  | 100.4   |
| cantilever | 0.499 | 7.5  | 57.6  | 101.8   |

B-bar cures volumetric locking only. On the cantilever at $\nu = 0.3$ it moves the shear-locked
answer by 1.4 percentage points while reduced integration moves it by 51. On the block at
$\nu = 0.499$ both cure the one pathology present and land 0.3 percentage points apart, each
within 1% of the reference. That is the whole
selection rule: if the problem locks only volumetrically, keep full quadrature and use
[B-bar or F-bar stabilization](Stabilization.md), which leave the rest of the block untouched. If
the problem is bending-dominated on linear elements, reduced integration is the only one of the three
that helps. The [QuasiStatic Physics](QuasiStaticSolidMechanicsPhysics.md) refuses to combine the two
corrections, since stacking B-bar on a one-point rule is not a defined scheme.

Three qualifications belong with every number quoted from that table.

It is a coarse-mesh measurement, and that is the point: locking is a coarse-mesh pathology.
Refinement removes it for every scheme. On the resolved 60x40 cantilever at $\nu = 0.3$ the same
three schemes read 98.89, 99.00 and 100.00, about a percentage point apart. The coarse-mesh spreads
of 49.8, 51.2 and 100.4 are the comparison to lead with; the fine-mesh figures are not.

The ordering is mesh-dependent and can invert. On a 10x2 cantilever the same three schemes come out
at -29% for full quadrature, -18% for B-bar and +8.2% for reduced integration at the default
penalty of 0.1: the last two are roughly equally accurate, on opposite sides of the answer. At a
penalty of 0.05 reduced integration reads +19% on that mesh, which is why the figure moves with the
penalty as well as with the mesh. Always state which mesh a quoted figure
came from.

The reference values are B-bar-extrapolated rather than full-quadrature, because full quadrature is
still 26% short at 80x16 on the nearly incompressible cantilever and so cannot serve as its own
reference. Each was obtained by Richardson extrapolation of a locking-corrected refinement sweep --
the block from 8x8 to 128x128 with an observed ratio of 4.00, exactly second order, and the
cantilever from 6x4 to 240x48 with ratios of 3.76 and 3.02. Those cantilever ratios are short of
4.00, so that sweep was not fully asymptotic and its own uncertainty is plausibly a few tenths of a
percent. Differences below roughly 0.5% on a fine mesh are therefore inside the reference's noise
and should not be read as real differences between schemes. Each extrapolated limit was
cross-checked against reduced integration, which converges to it from the opposite side.

## Coupled fields id=coupled

A conduction kernel on a reduced-integration block needs its own hourglass stabilization on the
temperature. This is a correctness trap, not a nicety. The global checkerboard temperature
field restricts to the hourglass pattern on every element, so it is an exact zero-energy mode of the
assembled one-point operator. Dirichlet conditions pin the boundary; the interior checkerboard
survives, and it survives silently.

The failure is severe in both directions. Left entirely unstabilized, the coupled test problem fails
to solve at all -- a zero pivot, then NaN. Given a starved penalty of 1e-4 it converges instead to a
temperature peaking at 59,270 K on a problem whose physical answer is near 676 K.

A `HeatConduction` kernel therefore wants a companion kernel on the same variable, with the stiffness
scale taken from the conductivity rather than from an elasticity tensor:

```
[Kernels]
  [conduction]
    type = HeatConduction
    variable = temp
  []
  [hourglass_temp]
    type = HourglassStabilization
    variable = temp
    stiffness_source = scalar_property
    scalar_stiffness = thermal_conductivity
  []
[]
```

The in-repo test uses the framework's `MatDiffusion` kernel in place of `HeatConduction`, because
`solid_mechanics` does not link the heat transfer module. The `HourglassStabilization` block is
identical either way; only the operator kernel differs.

!listing modules/solid_mechanics/test/tests/reduced_integration/conduction/thermomechanical.i
         block=Kernels

Two alternatives are legitimate. Put the conduction problem in a sub-app with its own mesh and
assembly at full quadrature and transfer the temperature in, which leaves the thermal field
integrated normally. Or simply do not enable reduced integration on a coupled block: if the
mechanics there does not lock, the scheme buys nothing worth the extra stabilization.

## Dynamics id=dynamics

[DynamicSolidMechanicsPhysics.md] derives from the QuasiStatic Physics and calls its `act()`, so
[!param](/Physics/SolidMechanics/QuasiStatic/QuasiStaticSolidMechanicsPhysics/reduced_integration)
does generate stabilization kernels there. Implicit dynamics -- Newmark, HHT -- should be sound on
the merits, because reference-configuration geometry keeps the tangent exact whatever factor the
time integrator applies to it, but no test in this module covers it.

Explicit dynamics is a different problem and a stiffness penalty is the wrong tool for it. With a
positive-definite mass matrix the hourglass modes carry mass and oscillate rather than sitting at
zero energy, which is why classical Flanagan-Belytschko explicit codes apply viscous hourglass
damping instead [!cite](flanagan1981). Adding hourglass stiffness also raises the maximum eigenvalue
and shrinks the stable timestep.

## Out of scope id=scope

TRI3 and TET4 have a constant gradient over the element, so they are not rank-deficient at one point
and need no hourglass control; the kernel errors on them rather than silently doing nothing.
Higher-order elements have a different zero-energy mode structure. There is no AD variant, and no
support for explicit dynamics.

The QuasiStatic Physics generates stabilization only on the `StressDivergenceTensors` path. It
refuses `new_system = true` and `compatibility_mode = true` because the Lagrangian path is untested
here, and the NEML2 and PoroMechanics action paths are likewise not wired. The kernel itself is
independent of the stress divergence kernels, so it can be added by hand in a [Kernels] block
alongside Lagrangian kernels.

!bibtex bibliography
