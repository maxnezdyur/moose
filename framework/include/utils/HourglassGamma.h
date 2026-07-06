//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "Moose.h"
#include "libmesh/elem.h"
#include <array>

namespace HourglassGamma
{

/// Result of the QUAD4 hourglass projection
struct GammaQuad4
{
  /// gamma vector: hourglass base orthogonalized against {1, x, y}
  std::array<Real, 4> gamma;
  /// element area
  Real area;
  /// rotation-invariant length-squared scale h^2 = tr(sum d d^T)/2
  Real h2;
};

/**
 * Flanagan-Belytschko gamma vector for a QUAD4 from nodal coordinates only:
 * gamma = h - (h.x) b_x - (h.y) b_y with h = (1,-1,1,-1) and b the one-point
 * gradient vectors b_x = (y24, y31, y42, y13)/(2A), b_y = (x42, x13, x24, x31)/(2A).
 * By construction gamma.1 = gamma.x = gamma.y = 0 to machine precision on any
 * (non-degenerate) quad, so the stabilization eps*gamma*gamma^T annihilates
 * affine fields exactly.
 */
inline GammaQuad4
gammaQuad4(const libMesh::Elem & elem)
{
  mooseAssert(elem.type() == libMesh::QUAD4, "gammaQuad4 requires a QUAD4 element");

  std::array<Real, 4> x, y;
  for (unsigned int i = 0; i < 4; ++i)
  {
    x[i] = elem.point(i)(0);
    y[i] = elem.point(i)(1);
  }

  // twice the (signed) area via the shoelace formula
  const Real A2 = (x[2] - x[0]) * (y[3] - y[1]) + (x[1] - x[3]) * (y[2] - y[0]);

  static const std::array<Real, 4> h = {{1, -1, 1, -1}};

  // one-point gradient vectors
  const std::array<Real, 4> bx = {
      {(y[1] - y[3]) / A2, (y[2] - y[0]) / A2, (y[3] - y[1]) / A2, (y[0] - y[2]) / A2}};
  const std::array<Real, 4> by = {
      {(x[3] - x[1]) / A2, (x[0] - x[2]) / A2, (x[1] - x[3]) / A2, (x[2] - x[0]) / A2}};

  Real hx = 0, hy = 0;
  for (unsigned int i = 0; i < 4; ++i)
  {
    hx += h[i] * x[i];
    hy += h[i] * y[i];
  }

  GammaQuad4 out;
  for (unsigned int i = 0; i < 4; ++i)
    out.gamma[i] = h[i] - hx * bx[i] - hy * by[i];
  out.area = 0.5 * std::abs(A2);

  // rotation-invariant scale about the vertex average
  const Real cx = 0.25 * (x[0] + x[1] + x[2] + x[3]);
  const Real cy = 0.25 * (y[0] + y[1] + y[2] + y[3]);
  Real tr = 0;
  for (unsigned int i = 0; i < 4; ++i)
    tr += (x[i] - cx) * (x[i] - cx) + (y[i] - cy) * (y[i] - cy);
  out.h2 = 0.5 * tr;

  return out;
}

/// Result of the HEX8 hourglass projection
struct GammaHex8
{
  /// four gamma vectors (FB modes xi*eta, eta*zeta, xi*zeta, xi*eta*zeta),
  /// each orthogonalized against {1, x, y, z}
  std::array<std::array<Real, 8>, 4> gamma;
  /// effective volume 8*det(J(0)) (the one-point quadrature weight)
  Real volume;
  /// rotation-invariant length-squared scale h^2 = tr(sum d d^T)/3
  Real h2;
};

/**
 * Flanagan-Belytschko gamma vectors for a HEX8: gamma_m = h_m - sum_a
 * (h_m . x_a) b_a with the four hourglass base patterns h_m and the one-point
 * gradient vectors b_a = rows of J(0)^-T S/8 (S = trilinear sign patterns).
 * Each gamma_m is orthogonal to {1, x, y, z} by construction on any
 * non-degenerate hex.
 */
inline GammaHex8
gammaHex8(const libMesh::Elem & elem)
{
  mooseAssert(elem.type() == libMesh::HEX8, "gammaHex8 requires a HEX8 element");

  // trilinear sign patterns (libMesh HEX8 node ordering)
  static const Real S[3][8] = {
      {-1, 1, 1, -1, -1, 1, 1, -1}, {-1, -1, 1, 1, -1, -1, 1, 1}, {-1, -1, -1, -1, 1, 1, 1, 1}};
  static const Real H[4][8] = {{1, -1, 1, -1, 1, -1, 1, -1},
                               {1, 1, -1, -1, -1, -1, 1, 1},
                               {1, -1, -1, 1, -1, 1, 1, -1},
                               {-1, 1, -1, 1, 1, -1, 1, -1}};

  std::array<std::array<Real, 8>, 3> x;
  for (unsigned int i = 0; i < 8; ++i)
    for (unsigned int a = 0; a < 3; ++a)
      x[a][i] = elem.point(i)(a);

  // Jacobian of the trilinear map at the center: J_ab = sum_i S[a][i]/8 x_b,i
  Real J[3][3];
  for (unsigned int a = 0; a < 3; ++a)
    for (unsigned int b = 0; b < 3; ++b)
    {
      J[a][b] = 0;
      for (unsigned int i = 0; i < 8; ++i)
        J[a][b] += S[a][i] * x[b][i] / 8.0;
    }
  const Real detJ = J[0][0] * (J[1][1] * J[2][2] - J[1][2] * J[2][1]) -
                    J[0][1] * (J[1][0] * J[2][2] - J[1][2] * J[2][0]) +
                    J[0][2] * (J[1][0] * J[2][1] - J[1][1] * J[2][0]);
  const Real inv = 1.0 / detJ;
  Real Ji[3][3];
  Ji[0][0] = (J[1][1] * J[2][2] - J[1][2] * J[2][1]) * inv;
  Ji[0][1] = (J[0][2] * J[2][1] - J[0][1] * J[2][2]) * inv;
  Ji[0][2] = (J[0][1] * J[1][2] - J[0][2] * J[1][1]) * inv;
  Ji[1][0] = (J[1][2] * J[2][0] - J[1][0] * J[2][2]) * inv;
  Ji[1][1] = (J[0][0] * J[2][2] - J[0][2] * J[2][0]) * inv;
  Ji[1][2] = (J[0][2] * J[1][0] - J[0][0] * J[1][2]) * inv;
  Ji[2][0] = (J[1][0] * J[2][1] - J[1][1] * J[2][0]) * inv;
  Ji[2][1] = (J[0][1] * J[2][0] - J[0][0] * J[2][1]) * inv;
  Ji[2][2] = (J[0][0] * J[1][1] - J[0][1] * J[1][0]) * inv;

  // one-point gradient vectors b_a,i = sum_c Ji[a][c] S[c][i]/8
  std::array<std::array<Real, 8>, 3> b;
  for (unsigned int a = 0; a < 3; ++a)
    for (unsigned int i = 0; i < 8; ++i)
    {
      b[a][i] = 0;
      for (unsigned int c = 0; c < 3; ++c)
        b[a][i] += Ji[a][c] * S[c][i] / 8.0;
    }

  GammaHex8 out;
  for (unsigned int m = 0; m < 4; ++m)
  {
    Real hx[3] = {0, 0, 0};
    for (unsigned int a = 0; a < 3; ++a)
      for (unsigned int i = 0; i < 8; ++i)
        hx[a] += H[m][i] * x[a][i];
    for (unsigned int i = 0; i < 8; ++i)
    {
      Real g = H[m][i];
      for (unsigned int a = 0; a < 3; ++a)
        g -= hx[a] * b[a][i];
      out.gamma[m][i] = g;
    }
  }
  out.volume = 8.0 * std::abs(detJ);

  Real cx[3] = {0, 0, 0};
  for (unsigned int a = 0; a < 3; ++a)
    for (unsigned int i = 0; i < 8; ++i)
      cx[a] += x[a][i] / 8.0;
  Real tr = 0;
  for (unsigned int a = 0; a < 3; ++a)
    for (unsigned int i = 0; i < 8; ++i)
      tr += (x[a][i] - cx[a]) * (x[a][i] - cx[a]);
  out.h2 = tr / 3.0;

  return out;
}

} // namespace HourglassGamma
