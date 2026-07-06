//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "HourglassGammaOrthogonality.h"
#include "HourglassGamma.h"

registerMooseObject("MooseApp", HourglassGammaOrthogonality);

InputParameters
HourglassGammaOrthogonality::validParams()
{
  InputParameters params = ElementPostprocessor::validParams();
  params.addClassDescription(
      "Worst violation over all QUAD4 elements of the hourglass-gamma orthogonality conditions "
      "gamma.1 = gamma.x = gamma.y = 0, normalized by |gamma|.");
  return params;
}

HourglassGammaOrthogonality::HourglassGammaOrthogonality(const InputParameters & parameters)
  : ElementPostprocessor(parameters), _max_violation(0)
{
}

void
HourglassGammaOrthogonality::execute()
{
  std::vector<std::array<Real, 8>> gammas;
  unsigned int nnode;
  Real h2;
  if (_current_elem->type() == libMesh::QUAD4)
  {
    const auto g = HourglassGamma::gammaQuad4(*_current_elem);
    gammas.resize(1);
    for (unsigned int i = 0; i < 4; ++i)
      gammas[0][i] = g.gamma[i];
    nnode = 4;
    h2 = g.h2;
  }
  else if (_current_elem->type() == libMesh::HEX8)
  {
    const auto g = HourglassGamma::gammaHex8(*_current_elem);
    gammas.assign(g.gamma.begin(), g.gamma.end());
    nnode = 8;
    h2 = g.h2;
  }
  else
    return;

  for (const auto & gam : gammas)
  {
    Real d1 = 0, norm2 = 0;
    Real dx[3] = {0, 0, 0};
    for (unsigned int i = 0; i < nnode; ++i)
    {
      d1 += gam[i];
      for (unsigned int a = 0; a < 3; ++a)
        dx[a] += gam[i] * _current_elem->point(i)(a);
      norm2 += gam[i] * gam[i];
    }
    const Real scale = std::sqrt(norm2) * std::sqrt(h2);
    const Real viol =
        std::max(
            {std::abs(d1) * std::sqrt(h2), std::abs(dx[0]), std::abs(dx[1]), std::abs(dx[2])}) /
        scale;
    _max_violation = std::max(_max_violation, viol);
  }
}

void
HourglassGammaOrthogonality::finalize()
{
  gatherMax(_max_violation);
}

void
HourglassGammaOrthogonality::threadJoin(const UserObject & y)
{
  const auto & o = static_cast<const HourglassGammaOrthogonality &>(y);
  _max_violation = std::max(_max_violation, o._max_violation);
}
