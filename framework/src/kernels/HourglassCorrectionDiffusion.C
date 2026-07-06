//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "HourglassCorrectionDiffusion.h"
#include "HourglassGamma.h"

#include "libmesh/enum_to_string.h"

registerMooseObject("MooseApp", HourglassCorrectionDiffusion);

InputParameters
HourglassCorrectionDiffusion::validParams()
{
  InputParameters params = ADKernel::validParams();
  params.addClassDescription(
      "Implicit hourglass stabilization for a scalar diffusion-type field on underintegrated "
      "QUAD4/HEX8 elements: eps * gamma * gamma^T with the Flanagan-Belytschko gamma (annihilates "
      "affine fields by construction) and eps = penalty * diffusivity * volume / h^2. The exact "
      "consistent tangent, including all diffusivity dependencies, comes from automatic "
      "differentiation.");
  params.addParam<Real>("penalty",
                        0.16666666666666666,
                        "Base penalty; the default 1/6 reproduces the fully-integrated hourglass "
                        "stiffness exactly on rectangles (QUAD4) and rectangular parallelepipeds "
                        "(HEX8).");
  params.addRequiredParam<MaterialPropertyName>(
      "diffusivity", "Diffusivity (e.g. thermal conductivity) material property.");
  return params;
}

HourglassCorrectionDiffusion::HourglassCorrectionDiffusion(const InputParameters & parameters)
  : ADKernel(parameters),
    _penalty(getParam<Real>("penalty")),
    _diff(getADMaterialProperty<Real>("diffusivity")),
    _v_nodal(_var.adDofValues())
{
}

ADReal
HourglassCorrectionDiffusion::computeQpResidual()
{
  mooseAssert(_qp == 0, "This kernel must only be used with single quadrature point integration.");

  ADReal r = 0;
  if (_current_elem->type() == libMesh::QUAD4)
  {
    const auto g = HourglassGamma::gammaQuad4(*_current_elem);
    ADReal gT = 0;
    for (unsigned int j = 0; j < 4; ++j)
      gT += g.gamma[j] * _v_nodal[j];
    r = g.gamma[_i] * gT / g.h2;
  }
  else if (_current_elem->type() == libMesh::HEX8)
  {
    const auto g = HourglassGamma::gammaHex8(*_current_elem);
    // with penalty = 1/6 these weights reproduce the fully-integrated
    // hourglass eigenvalues exactly on rectangular parallelepipeds:
    // k/3 for the three face modes, k/6 for the xi*eta*zeta mode
    static const Real w[4] = {0.5, 0.5, 0.5, 0.25};
    for (unsigned int m = 0; m < 4; ++m)
    {
      ADReal gT = 0;
      for (unsigned int j = 0; j < 8; ++j)
        gT += g.gamma[m][j] * _v_nodal[j];
      r += w[m] * g.gamma[m][_i] * gT / g.h2;
    }
  }
  else
    mooseError("HourglassCorrectionDiffusion supports QUAD4 and HEX8 elements, got ",
               libMesh::Utility::enum_to_string(_current_elem->type()));

  // MOOSE multiplies by JxW (= area/volume at one-point quadrature),
  // completing eps = penalty * D * V / h^2
  return _penalty * _diff[_qp] * r;
}
