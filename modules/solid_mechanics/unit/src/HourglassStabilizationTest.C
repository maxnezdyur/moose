//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "gtest/gtest.h"

#include <array>

#include "HourglassStabilization.h"

#include "libmesh/enum_elem_type.h"
#include "libmesh/point.h"
#include "libmesh/int_range.h"

#include <cmath>
#include <vector>

// Unit tests for the two static member functions HourglassStabilization exposes so its geometry
// math -- the classical Flanagan-Belytschko mode vectors and their affine-space projection -- can
// be exercised with no Kernel instance, no material, and no assembly.

namespace
{

/// Dot product of two equal-length vectors.
Real
dot(const std::vector<Real> & a, const std::vector<Real> & b)
{
  Real s = 0.0;
  for (const auto i : index_range(a))
    s += a[i] * b[i];
  return s;
}

/// A genuinely irregular quadrilateral in libMesh QUAD4 node order: no two sides are parallel and
/// no two side lengths match, so a projection bug (a skipped affine removal, or a sign error in the
/// 2x2 adjugate) that a parallelogram or rectangle would hide cannot hide here.
std::vector<Point>
distortedQuad4()
{
  return {Point(0.0, 0.0, 0.0), Point(1.0, -0.2, 0.0), Point(1.3, 1.1, 0.0), Point(-0.3, 0.9, 0.0)};
}

/// A genuinely irregular hexahedron in libMesh HEX8 node order: every node is displaced off the
/// unit cube by a different amount in every direction, so no face stays planar and no edge stays
/// parallel to another.
std::vector<Point>
distortedHex8()
{
  return {Point(0.0, 0.0, 0.0),
          Point(1.1, -0.1, 0.05),
          Point(1.05, 1.15, -0.05),
          Point(-0.05, 0.95, 0.1),
          Point(0.1, -0.05, 1.05),
          Point(1.0, 0.05, 0.95),
          Point(0.95, 1.05, 1.1),
          Point(0.0, 1.0, 1.0)};
}

/// Asserts every ghat[m] is orthogonal, summed over the element's nodes, to the constant field 1
/// and to each reference coordinate field. This is exactly the property computeProjectedModes()
/// exists to guarantee, and it is the first thing to break if the affine projection is skipped or
/// the A^-1 adjugate has a sign error.
void
expectOrthogonalToAffineFields(const std::vector<std::vector<Real>> & ghat,
                               const std::vector<Point> & coords,
                               const unsigned int n_dim,
                               const Real tol)
{
  for (const auto m : index_range(ghat))
  {
    Real sum_const = 0.0;
    std::vector<Real> sum_coord(n_dim, 0.0);
    for (const auto i : index_range(coords))
    {
      sum_const += ghat[m][i];
      for (const auto d : make_range(n_dim))
        sum_coord[d] += ghat[m][i] * coords[i](d);
    }
    EXPECT_NEAR(sum_const, 0.0, tol) << "mode " << m << " not orthogonal to the constant field";
    for (const auto d : make_range(n_dim))
      EXPECT_NEAR(sum_coord[d], 0.0, tol)
          << "mode " << m << " not orthogonal to coordinate field " << d;
  }
}

/// H^m(u) = sum_j ghat[m][j] * u[j], the amplitude of mode m in nodal displacement u.
Real
modeAmplitude(const std::vector<std::vector<Real>> & ghat,
              const unsigned int m,
              const std::vector<Real> & u)
{
  Real h = 0.0;
  for (const auto j : index_range(u))
    h += ghat[m][j] * u[j];
  return h;
}

/// R_i(u) = c * sum_m ghat[m][i] * H^m(u), the stabilization residual the kernel assembles.
Real
residual(const std::vector<std::vector<Real>> & ghat,
         const std::vector<Real> & u,
         const Real c,
         const unsigned int i)
{
  Real r = 0.0;
  for (const auto m : index_range(ghat))
    r += ghat[m][i] * modeAmplitude(ghat, m, u);
  return c * r;
}

/// Central-differences dR_i/du_j against the analytic c * sum_m ghat[m][i] * ghat[m][j] for every
/// (i, j) pair. R is exactly linear in u (H^m is linear, and R is linear in H^m), so the finite
/// difference carries no truncation error -- only floating-point round-off, which for O(1) ghat, u
/// and c is of order eps / h ~ 1e-16 / 1e-5 = 1e-11 with the step below, comfortably under the
/// 1e-9 tolerance.
void
checkTangentAgainstCentralDifference(const std::vector<std::vector<Real>> & ghat,
                                     const std::vector<Real> & u,
                                     const Real c)
{
  const Real h = 1.0e-5;
  for (const auto j : index_range(u))
  {
    std::vector<Real> u_plus = u;
    std::vector<Real> u_minus = u;
    u_plus[j] += h;
    u_minus[j] -= h;
    for (const auto i : index_range(u))
    {
      const Real fd = (residual(ghat, u_plus, c, i) - residual(ghat, u_minus, c, i)) / (2.0 * h);

      Real analytic = 0.0;
      for (const auto m : index_range(ghat))
        analytic += ghat[m][i] * ghat[m][j];
      analytic *= c;

      EXPECT_NEAR(fd, analytic, 1.0e-9) << "i=" << i << ", j=" << j;
    }
  }
}

}

// (a) Affine orthogonality on deliberately distorted elements -- the property the projection
// exists to guarantee. A regular grid would pass even with a broken projection, so this must run
// on a genuinely irregular element to mean anything.

TEST(HourglassStabilizationTest, AffineOrthogonalityDistortedQuad4)
{
  const auto coords = distortedQuad4();
  const auto * const gamma = HourglassStabilization::modeVectors(libMesh::QUAD4);
  ASSERT_NE(gamma, nullptr);

  std::vector<std::vector<Real>> ghat;
  Real h2 = 0.0;
  HourglassStabilization::computeProjectedModes(*gamma, coords, 2u, ghat, h2);

  // Tight tolerance: double-precision round-off through this O(1), well-conditioned 2x2 inverse is
  // of order 1e-13; 1e-10 leaves a comfortable margin while still catching a broken projection,
  // whose residual affine component would be O(1).
  expectOrthogonalToAffineFields(ghat, coords, 2u, 1.0e-10);
}

TEST(HourglassStabilizationTest, AffineOrthogonalityDistortedHex8)
{
  const auto coords = distortedHex8();
  const auto * const gamma = HourglassStabilization::modeVectors(libMesh::HEX8);
  ASSERT_NE(gamma, nullptr);

  std::vector<std::vector<Real>> ghat;
  Real h2 = 0.0;
  HourglassStabilization::computeProjectedModes(*gamma, coords, 3u, ghat, h2);

  expectOrthogonalToAffineFields(ghat, coords, 3u, 1.0e-10);
}

// (b) Mode-count and independence contract.

TEST(HourglassStabilizationTest, ModeVectorsQuad4)
{
  const auto * const gamma = HourglassStabilization::modeVectors(libMesh::QUAD4);
  ASSERT_NE(gamma, nullptr);
  ASSERT_EQ(gamma->size(), 1u);
  ASSERT_EQ((*gamma)[0].size(), 4u);

  const std::vector<Real> expected = {1.0, -1.0, 1.0, -1.0};
  for (const auto i : make_range(4u))
    EXPECT_DOUBLE_EQ((*gamma)[0][i], expected[i]);
}

TEST(HourglassStabilizationTest, ModeVectorsHex8)
{
  const auto * const gamma = HourglassStabilization::modeVectors(libMesh::HEX8);
  ASSERT_NE(gamma, nullptr);
  ASSERT_EQ(gamma->size(), 4u);

  const std::vector<std::vector<Real>> expected = {{1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0},
                                                   {1.0, 1.0, -1.0, -1.0, -1.0, -1.0, 1.0, 1.0},
                                                   {1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0},
                                                   {-1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0}};

  for (const auto m : make_range(4u))
  {
    ASSERT_EQ((*gamma)[m].size(), 8u);
    for (const auto i : make_range(8u))
      EXPECT_DOUBLE_EQ((*gamma)[m][i], expected[m][i]);
  }
}

TEST(HourglassStabilizationTest, Hex8ModesLinearlyIndependent)
{
  const auto * const gamma = HourglassStabilization::modeVectors(libMesh::HEX8);
  ASSERT_NE(gamma, nullptr);
  ASSERT_EQ(gamma->size(), 4u);

  std::array<std::array<Real, 4>, 4> gram;
  for (const auto a : make_range(4u))
    for (const auto b : make_range(4u))
      gram[a][b] = dot((*gamma)[a], (*gamma)[b]);

  // By hand from the table above, the four modes are mutually orthogonal with norm^2 = 8 each, so
  // the Gram matrix is exactly 8*I -- which is linear independence, checked numerically against the
  // actual returned vectors rather than assuming the hand computation still holds.
  for (const auto a : make_range(4u))
  {
    EXPECT_DOUBLE_EQ(gram[a][a], 8.0);
    for (const auto b : make_range(4u))
      if (a != b)
        EXPECT_DOUBLE_EQ(gram[a][b], 0.0);
  }
}

TEST(HourglassStabilizationTest, ModeVectorsNullForUnsupportedElementTypes)
{
  // TRI3 and TET4 have a constant gradient over the element and so are not rank-deficient under
  // one-point quadrature; they are deliberately out of scope for this construction.
  EXPECT_EQ(HourglassStabilization::modeVectors(libMesh::TRI3), nullptr);
  EXPECT_EQ(HourglassStabilization::modeVectors(libMesh::TET4), nullptr);
  // A higher-order element has a different zero-energy mode structure and is likewise unsupported.
  EXPECT_EQ(HourglassStabilization::modeVectors(libMesh::HEX20), nullptr);
}

// (c) Analytic tangent equals a central difference.

TEST(HourglassStabilizationTest, TangentMatchesCentralDifferenceQuad4)
{
  const auto coords = distortedQuad4();
  const auto * const gamma = HourglassStabilization::modeVectors(libMesh::QUAD4);
  ASSERT_NE(gamma, nullptr);

  std::vector<std::vector<Real>> ghat;
  Real h2 = 0.0;
  HourglassStabilization::computeProjectedModes(*gamma, coords, 2u, ghat, h2);

  const std::vector<Real> u = {0.3, -0.7, 1.1, -0.2};
  checkTangentAgainstCentralDifference(ghat, u, 2.5);
}

TEST(HourglassStabilizationTest, TangentMatchesCentralDifferenceHex8)
{
  const auto coords = distortedHex8();
  const auto * const gamma = HourglassStabilization::modeVectors(libMesh::HEX8);
  ASSERT_NE(gamma, nullptr);

  std::vector<std::vector<Real>> ghat;
  Real h2 = 0.0;
  HourglassStabilization::computeProjectedModes(*gamma, coords, 3u, ghat, h2);

  const std::vector<Real> u = {0.3, -0.7, 1.1, -0.2, 0.5, -0.4, 0.9, -0.6};
  checkTangentAgainstCentralDifference(ghat, u, 1.75);
}

// (d) Singular-A regularization returns finite values. The regularization threshold and fallback
// value are explicitly not part of the interface, so only finiteness is asserted here.

TEST(HourglassStabilizationTest, ProjectedModesFiniteOnCollinearQuad4)
{
  // Four distinct, unevenly spaced nodes on a line: every d_i has y = 0, so A is exactly diag(*, 0)
  // -- singular -- while h2 = trace(A)/2 > 0. This exercises the isotropic-inverse fallback on a
  // partially degenerate element that still has a length scale.
  const std::vector<Point> coords = {
      Point(0.0, 0.0, 0.0), Point(1.0, 0.0, 0.0), Point(2.5, 0.0, 0.0), Point(4.0, 0.0, 0.0)};
  const auto * const gamma = HourglassStabilization::modeVectors(libMesh::QUAD4);
  ASSERT_NE(gamma, nullptr);

  std::vector<std::vector<Real>> ghat;
  Real h2 = 0.0;
  HourglassStabilization::computeProjectedModes(*gamma, coords, 2u, ghat, h2);

  EXPECT_TRUE(std::isfinite(h2));
  EXPECT_GT(h2, 0.0);
  ASSERT_EQ(ghat.size(), 1u);
  for (const auto val : ghat[0])
    EXPECT_TRUE(std::isfinite(val));
}

TEST(HourglassStabilizationTest, ProjectedModesFiniteOnCollapsedHex8)
{
  // All eight nodes collapsed onto a single point: every d_i = 0, so A = 0 and h2 = 0 -- the fully
  // degenerate case where even the isotropic fallback has no length scale to divide by. The
  // implementation must not produce NaN or inf here.
  const std::vector<Point> coords(8, Point(1.0, 2.0, 3.0));
  const auto * const gamma = HourglassStabilization::modeVectors(libMesh::HEX8);
  ASSERT_NE(gamma, nullptr);

  std::vector<std::vector<Real>> ghat;
  Real h2 = 0.0;
  HourglassStabilization::computeProjectedModes(*gamma, coords, 3u, ghat, h2);

  EXPECT_TRUE(std::isfinite(h2));
  EXPECT_DOUBLE_EQ(h2, 0.0);
  ASSERT_EQ(ghat.size(), 4u);
  for (const auto & mode : ghat)
    for (const auto val : mode)
      EXPECT_TRUE(std::isfinite(val));
}

// (e) h2 is exactly trace(A) / n_dim, pinned by hand on elements whose A is easy to compute.

TEST(HourglassStabilizationTest, H2EqualsTraceOverDimUnitSquare)
{
  // By hand: centroid = (0.5, 0.5); d_i = (+/-0.5, +/-0.5) for the four corners. A(0,0) = A(1,1) =
  // 4 * 0.25 = 1.0, and A(0,1) = 0 because the signs of x and y vary independently across
  // the four corners, so the cross terms cancel in pairs. trace(A) = 2.0, n_dim = 2, so
  // h2 = 2.0 / 2 = 1.0.
  const std::vector<Point> coords = {
      Point(0.0, 0.0, 0.0), Point(1.0, 0.0, 0.0), Point(1.0, 1.0, 0.0), Point(0.0, 1.0, 0.0)};
  const auto * const gamma = HourglassStabilization::modeVectors(libMesh::QUAD4);
  ASSERT_NE(gamma, nullptr);

  std::vector<std::vector<Real>> ghat;
  Real h2 = 0.0;
  HourglassStabilization::computeProjectedModes(*gamma, coords, 2u, ghat, h2);

  // Every input is an exact half-integer, so the arithmetic above has no rounding at all; NEAR with
  // a tight tolerance guards against floating-point reassociation without weakening the check.
  EXPECT_NEAR(h2, 1.0, 1.0e-12);
}

TEST(HourglassStabilizationTest, H2EqualsTraceOverDimUnitCube)
{
  // By hand: centroid = (0.5, 0.5, 0.5); d_i = (+/-0.5, +/-0.5, +/-0.5) for the eight corners.
  // A(a,a) = 8 * 0.25 = 2.0 for each of the three diagonal entries (twice as many corners as the
  // square contribute to each axis), and every off-diagonal cancels for the same independent-sign
  // reason as the square. trace(A) = 3 * 2.0 = 6.0, n_dim = 3, so h2 = 6.0 / 3 = 2.0.
  const std::vector<Point> coords = {Point(0.0, 0.0, 0.0),
                                     Point(1.0, 0.0, 0.0),
                                     Point(1.0, 1.0, 0.0),
                                     Point(0.0, 1.0, 0.0),
                                     Point(0.0, 0.0, 1.0),
                                     Point(1.0, 0.0, 1.0),
                                     Point(1.0, 1.0, 1.0),
                                     Point(0.0, 1.0, 1.0)};
  const auto * const gamma = HourglassStabilization::modeVectors(libMesh::HEX8);
  ASSERT_NE(gamma, nullptr);

  std::vector<std::vector<Real>> ghat;
  Real h2 = 0.0;
  HourglassStabilization::computeProjectedModes(*gamma, coords, 3u, ghat, h2);

  EXPECT_NEAR(h2, 2.0, 1.0e-12);
}
