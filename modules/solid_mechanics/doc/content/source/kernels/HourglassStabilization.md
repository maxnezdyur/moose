# HourglassStabilization

!syntax description /Kernels/HourglassStabilization

## Description

At a single quadrature point the discrete gradient operator of a second-order operator is
$n_d \times n$, where $n_d$ is the spatial dimension and $n$ the number of element nodes, so the
element operator reaches rank $n_d$ at most while rank $n-1$ is required. The constant field is the
one physical direction of the nullspace; every further direction the element loses is a spurious
zero-energy hourglass mode.

!table id=rank_table caption=Rank supplied and rank required under single-point quadrature.
| Element | Nodes $n$ | Dimension $n_d$ | Rank at one point | Rank required | Hourglass modes $n_m$ |
|---------|-----------|-----------------|-------------------|---------------|-----------------------|
| QUAD4   | 4         | 2               | 2                 | 3             | 1                     |
| HEX8    | 8         | 3               | 3                 | 7             | 4                     |

That nullspace is a property of the element and the quadrature rule alone, never of the physics, so
the same construction stabilizes a displacement component, a temperature, or any other field
carrying a second-order operator. The kernel stabilizes the one field named by
[!param](/Kernels/HourglassStabilization/variable), which must be first order Lagrange so that it
carries one nodal value per element node. A block with several such fields needs one kernel per
field.

QUAD4 and HEX8 are the only element types the Flanagan-Belytschko construction covers
[!cite](flanagan1981), and the kernel errors on any other. TRI3 and TET4 have a constant gradient
over the element and so are not rank-deficient at one point, and higher-order elements have a
different zero-energy mode structure. Restrict the kernel to the QUAD4 and HEX8 blocks with
[!param](/Kernels/HourglassStabilization/block). Note that leaving the remaining blocks at full
quadrature is a separate step: the rule itself is scoped with
[!param](/Executioner/Quadrature/custom_blocks), not by this parameter.

A two-dimensional element must also lie in a plane of constant $z$. The reference geometry matrix is
built from the leading `dim()` global coordinate components, so a QUAD4 rotated out of that plane is
rejected rather than silently mis-projected.

## Formulation id=formulation

Write $\mathbf{X}_i$ for the reference coordinates of node $i$, $\bar{\mathbf{X}}$ for their
average, and $\boldsymbol{\gamma}^m$, $m = 1 \ldots n_m$, for the classical Flanagan-Belytschko mode
vectors. The element geometry enters through

\begin{equation}
\mathbf{d}_i = \mathbf{X}_i - \bar{\mathbf{X}}, \qquad
\mathbf{A} = \sum_{i=1}^{n} \mathbf{d}_i \mathbf{d}_i^{T}, \qquad
h^2 = \frac{1}{n_d} \operatorname{tr} \mathbf{A} ,
\end{equation}

where $\mathbf{A}$ is the symmetric $n_d \times n_d$ reference geometry matrix and $h^2$ is a
rotation-invariant squared length scale. Each classical mode vector has zero mean, so removing its
linear part is all that is needed to make it orthogonal to the affine field space:

\begin{equation}
\mathbf{p}^m = \sum_{j=1}^{n} \gamma^m_j \mathbf{d}_j , \qquad
\hat{\gamma}^m_i = \gamma^m_i - \left(\mathbf{p}^m\right)^{T} \mathbf{A}^{-1} \mathbf{d}_i .
\end{equation}

That projection is what leaves every affine field, and therefore the patch test and rigid-body
motion, exactly unstabilized. It is load-bearing on distorted elements: an unprojected mode vector
is not orthogonal to the affine space on a non-parallelogram quadrilateral or a
non-parallelepiped hexahedron, so without it a linear field would generate a spurious stabilization
force.

With $u_j$ the nodal values of the stabilized field, $S$ the stiffness scale of
[stiffness_table], and $H^m$ the amplitude of mode $m$ in the current elemental solution,

\begin{equation}
H^m = \sum_{j=1}^{n} \hat{\gamma}^m_j u_j , \qquad
c = \frac{\texttt{penalty} \, S}{h^2} ,
\end{equation}

and the kernel adds to the residual of its own field alone,

\begin{equation}
R_i = c \sum_{m=1}^{n_m} \hat{\gamma}^m_i H^m , \qquad
\frac{\partial R_i}{\partial u_j} = c \sum_{m=1}^{n_m} \hat{\gamma}^m_i \hat{\gamma}^m_j .
\end{equation}

These are the pointwise factors. Kernel assembly multiplies by the quadrature weight and the
coordinate transformation once, and with one quadrature point that product is the element measure
$V_e$, so the integrated coefficient is $\texttt{penalty} \, S \, V_e / h^2$ -- including the
$2 \pi r$ factor in RZ coordinates.

## Stiffness scale id=stiffness

[!param](/Kernels/HourglassStabilization/stiffness_source) selects where $S$ comes from.
Dimensional analysis fixes the $1/h^2$ scaling for any second-order operator: a stiffness entry
scales as $S L^{n_d - 2}$ while $c V_e$ scales as $c L^{n_d}$, so $c = \texttt{penalty} \, S / h^2$
in every case.

!table id=stiffness_table caption=Stiffness scale by source.
| [!param](/Kernels/HourglassStabilization/stiffness_source) | $S$ | Property read |
|------------------------------------------------------------|-----|---------------|
| `elasticity_tensor` | the isotropic shear modulus $\mu$ | [!param](/Kernels/HourglassStabilization/elasticity_tensor), a rank-four property, through `ElasticityTensorTools::getIsotropicShearModulus` |
| `scalar_property`   | the named scalar itself, for instance a thermal conductivity | [!param](/Kernels/HourglassStabilization/scalar_stiffness), a scalar property |

The two properties are looked up asymmetrically on purpose. The elasticity tensor is a member of the
solid mechanics property family and is read with the
[!param](/Kernels/HourglassStabilization/base_name) prefix applied, exactly as the stress divergence
kernels read it. [!param](/Kernels/HourglassStabilization/scalar_stiffness) names a property from
outside that family and is read unprefixed, because a thermal conductivity has no reason to carry a
mechanics base name.

## Quadrature rule id=quadrature

The kernel validates the rule it is given but does not set it: MOOSE's object-level quadrature API
can only raise the order of a rule, never lower it, so one-point quadrature is requested through
the executioner and applies to the whole block.

!listing modules/solid_mechanics/test/tests/reduced_integration/hourglass_suppression/suppression.i
         block=Executioner/Quadrature

If the rule the kernel sees is not single-point it errors with that block as pasteable remediation
text. Because the rule belongs to the block rather than to a kernel, enabling it changes the
integration of every object there; [reduced integration](solid_mechanics/reduced_integration.md)
covers those consequences.

## Reference configuration id=reference

All geometry is measured in the reference configuration, and `use_displaced_mesh` is fixed at
`false` rather than offered as a choice. That makes $\hat{\boldsymbol{\gamma}}^m$, $h^2$ and $c$
solution-independent. When the stiffness property is solution-independent too, two consequences
follow: the tangent above is the exact derivative of the
residual, measured with `-snes_test_jacobian` at a Frobenius norm ratio of 2.63e-10 on QUAD4 and
4.99e-10 on HEX8 against a 1e-7 tolerance; and the residual depends on no variable other than its own, so
the off-diagonal tangent is identically zero.

The cost is that the stabilization does not track large deformation. On a heavily distorted or
largely rotated element it remains scaled by the original geometry and so under- or
over-stabilizes. For the small-strain implicit quasi-static problems this kernel targets that is the
right trade; a displaced-geometry variant carrying the full geometric tangent is a follow-up.

$S$ is treated as solution-independent in the tangent as well, which holds for ordinary linear
elasticity and for a constant conductivity. Supply a temperature- or damage-dependent stiffness and
the $\partial S / \partial u$ term is missing: Newton still converges as an inexact method, but the
tangent is no longer exact.

## Choosing the penalty id=penalty

[!param](/Kernels/HourglassStabilization/penalty) is dimensionless because $S$ carries the physical
stiffness, and its default sits at the top of the classical 0.03 to 0.1 window. The response is
monotonic and sensitive rather than flat over a plateau: too small under-stabilizes and overshoots
the converged answer, too large stiffens the element back toward the locked one. How much that
matters depends on the mesh. A 200-fold range of the penalty moves a resolved bending answer by half
a percentage point and a coarse one by 23 points, so an answer that shifts with this parameter is a
signal that the mesh is too coarse for the bending it resolves. The measured sweeps are on the
[reduced integration](solid_mechanics/reduced_integration.md#penalty) page.

## Example Input File Syntax id=example

Two kernels stabilize the two displacement components of a coarse QUAD4 cantilever alongside the
stress divergence kernels. [!param](/Kernels/HourglassStabilization/save_in) collects the
stabilization force into an aux variable, where it is nonzero on this bending problem and exactly
zero for an affine field.

!listing modules/solid_mechanics/test/tests/reduced_integration/hourglass_suppression/suppression.i
         block=Kernels

!syntax parameters /Kernels/HourglassStabilization

!syntax inputs /Kernels/HourglassStabilization

!syntax children /Kernels/HourglassStabilization

!bibtex bibliography
