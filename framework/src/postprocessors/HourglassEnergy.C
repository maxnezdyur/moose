//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "HourglassEnergy.h"
#include "HourglassGamma.h"
#include "metaphysicl/raw_type.h"

registerMooseObject("MooseApp", HourglassEnergy);

InputParameters
HourglassEnergy::validParams()
{
  InputParameters params = ElementIntegralVariablePostprocessor::validParams();
  params.addClassDescription("Total hourglass stabilization energy sum_e eps (gamma . u)^2 with "
                             "eps = penalty * diffusivity * area / h^2 (QUAD4).");
  params.addParam<Real>("penalty", 0.16666666666666666, "Base penalty (match the kernel).");
  params.addRequiredParam<MaterialPropertyName>(
      "diffusivity", "Diffusivity material property (match the kernel).");
  return params;
}

HourglassEnergy::HourglassEnergy(const InputParameters & parameters)
  : ElementIntegralVariablePostprocessor(parameters),
    _penalty(getParam<Real>("penalty")),
    _diff(getADMaterialProperty<Real>("diffusivity")),
    _v_nodal(coupledDofValues("variable"))
{
}

void
HourglassEnergy::execute()
{
  if (_current_elem->type() == libMesh::QUAD4)
  {
    const auto g = HourglassGamma::gammaQuad4(*_current_elem);
    Real gT = 0;
    for (unsigned int j = 0; j < 4 && j < _v_nodal.size(); ++j)
      gT += g.gamma[j] * _v_nodal[j];
    _integral_value += _penalty * MetaPhysicL::raw_value(_diff[0]) * g.area / g.h2 * gT * gT;
  }
  else if (_current_elem->type() == libMesh::HEX8)
  {
    const auto g = HourglassGamma::gammaHex8(*_current_elem);
    static const Real w[4] = {0.5, 0.5, 0.5, 0.25}; // match the kernel's mode weights
    for (unsigned int m = 0; m < 4; ++m)
    {
      Real gT = 0;
      for (unsigned int j = 0; j < 8 && j < _v_nodal.size(); ++j)
        gT += g.gamma[m][j] * _v_nodal[j];
      _integral_value +=
          _penalty * w[m] * MetaPhysicL::raw_value(_diff[0]) * g.volume / g.h2 * gT * gT;
    }
  }
}
