//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "INSADMassPSPG.h"
#include "MooseTypes.h"

registerMooseObject("NavierStokesApp", INSADMassPSPG);

InputParameters
INSADMassPSPG::validParams()
{
  InputParameters params = ADKernelGrad::validParams();
  params.addClassDescription(
      "This class adds PSPG stabilization to the mass equation, enabling use of "
      "equal order shape functions for pressure and velocity variables");
  params.addParam<MaterialPropertyName>("rho_name", "rho", "The name of the density");
  params.addCoupledVar("volume_fraction", 1, "volume_frac");
  return params;
}

INSADMassPSPG::INSADMassPSPG(const InputParameters & parameters)
  : ADKernelGrad(parameters),
    _rho(getADMaterialProperty<Real>("rho_name")),
    _tau(getADMaterialProperty<Real>("tau")),
    _momentum_strong_residual(getADMaterialProperty<RealVectorValue>("momentum_strong_residual")),
    _vol_frac(coupledValue("volume_fraction"))
{
}

ADRealVectorValue
INSADMassPSPG::precomputeQpResidual()
{
  return -_tau[_qp] / _rho[_qp] * _momentum_strong_residual[_qp] * _vol_frac[_qp];
}
