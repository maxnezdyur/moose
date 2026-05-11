//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ADLineHeatSinkRayKernel.h"
#include "Function.h"

registerMooseObject("HeatTransferApp", ADLineHeatSinkRayKernel);

InputParameters
ADLineHeatSinkRayKernel::validParams()
{
  InputParameters params = ADRayKernel::validParams();
  params.addRequiredParam<FunctionName>("h",
                                        "Heat transfer coefficient h(x, t) for the line sink.");
  params.addRequiredParam<FunctionName>("T_sink",
                                        "Temperature of the line sink fluid T_sink(x, t).");
  params.addClassDescription(
      "Newton-cooling line heat sink h(x,t) * (T - T_sink(x,t)) integrated along a ray traced "
      "through the mesh by a RepeatableRayStudy; mesh-independent by construction.");
  return params;
}

ADLineHeatSinkRayKernel::ADLineHeatSinkRayKernel(const InputParameters & parameters)
  : ADRayKernel(parameters), _h(getFunction("h")), _T_sink(getFunction("T_sink"))
{
}

ADReal
ADLineHeatSinkRayKernel::computeQpResidual()
{
  const Real h = _h.value(_t, _q_point[_qp]);
  const Real T_sink = _T_sink.value(_t, _q_point[_qp]);
  return _test[_i][_qp] * h * (_u[_qp] - T_sink);
}
