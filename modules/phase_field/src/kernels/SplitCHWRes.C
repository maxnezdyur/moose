<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "SplitCHWRes.h"

registerMooseObject("PhaseFieldApp", SplitCHWRes);

InputParameters
SplitCHWRes::validParams()
{
  InputParameters params = SplitCHWResBase<Real>::validParams();
  params.addClassDescription("Split formulation Cahn-Hilliard Kernel for the chemical potential "
                             "variable with a scalar (isotropic) mobility");
  return params;
}

SplitCHWRes::SplitCHWRes(const InputParameters & parameters) : SplitCHWResBase<Real>(parameters) {}
=======
#include "SplitCHWRes.h"
template<>
InputParameters validParams<SplitCHWRes>()
{
  InputParameters params = validParams<Kernel>();

    params.addRequiredCoupledVar("c","intermediate parameter--concentration");
    params.addParam<std::string>("mob_name","mobtemp","The mobility used with the kernel");

  return params;
}

SplitCHWRes::SplitCHWRes(const std::string & name, InputParameters parameters)
  :Kernel(name, parameters),
   _mob_name(getParam<std::string>("mob_name")),
   _mob(getMaterialProperty<Real>(_mob_name)),
//  This _c_var is needed to compute off-diagonal Jacobian.
   _c_var(coupled("c")),
   _c(coupledValue("c"))
{}

Real
SplitCHWRes::computeQpResidual()
{
  return  _mob[_qp]*_grad_u[_qp] * _grad_test[_i][_qp];
}

Real
SplitCHWRes::computeQpJacobian()
{
  return _mob[_qp]*_grad_phi[_j][_qp] * _grad_test[_i][_qp];
}

Real
SplitCHWRes::computeQpOffDiagJacobian(unsigned int jvar)
{
  if(jvar == _c_var)
  {

   return 0.0;

  }

  return 0.0;
}


>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
