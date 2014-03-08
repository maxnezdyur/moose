<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PrimaryConvection.h"

registerMooseObject("ChemicalReactionsApp", PrimaryConvection);

InputParameters
PrimaryConvection::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addRequiredCoupledVar("p", "Pressure");
  RealVectorValue g(0, 0, 0);
  params.addParam<RealVectorValue>("gravity", g, "Gravity vector (default is (0, 0, 0))");
  params.addClassDescription("Convection of primary species");
  return params;
}

PrimaryConvection::PrimaryConvection(const InputParameters & parameters)
  : DerivativeMaterialInterface<Kernel>(parameters),
    _cond(getMaterialProperty<Real>("conductivity")),
    _gravity(getParam<RealVectorValue>("gravity")),
    _density(getDefaultMaterialProperty<Real>("density")),
    _grad_p(coupledGradient("p")),
    _pvar(coupled("p"))

{
}

Real
PrimaryConvection::computeQpResidual()
{
  RealVectorValue darcy_vel = -_cond[_qp] * (_grad_p[_qp] - _density[_qp] * _gravity);

  return _test[_i][_qp] * (darcy_vel * _grad_u[_qp]);
}

Real
PrimaryConvection::computeQpJacobian()
{
  RealVectorValue darcy_vel = -_cond[_qp] * (_grad_p[_qp] - _density[_qp] * _gravity);

  return _test[_i][_qp] * (darcy_vel * _grad_phi[_j][_qp]);
}

Real
PrimaryConvection::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _pvar)
  {
    RealVectorValue ddarcy_vel_dp = -_cond[_qp] * _grad_phi[_j][_qp];
    return _test[_i][_qp] * (ddarcy_vel_dp * _grad_u[_qp]);
  }
  else
    return 0.0;
=======
#include "PrimaryConvection.h"
#include "Material.h"

template<>
InputParameters validParams<PrimaryConvection>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar("p", "Pressure");
  return params;
}

PrimaryConvection::PrimaryConvection(const std::string & name, InputParameters parameters)

  // You must call the constructor of the base class first
  // The "true" here specifies that this Kernel is to be integrated
  // over the domain.
  :Kernel(name,parameters),
   _cond(getMaterialProperty<Real>("conductivity")),
   // coupledGradient will give us a reference to the gradient of another
   // variable in the computation.  We are going to use that gradient
   // as our velocity vector.
   //
   // Note that "some_var" is the name this Kernel expects... ie
   // what should be in "coupled_as"
   //
   // You can also use coupledValue() and coupledValueOld() if you want
   // values
   _grad_p(coupledGradient("p"))
{
}

Real PrimaryConvection::computeQpResidual()
{
  // _grad_p[_qp] * _grad_u[_qp] is actually doing a dot product
  RealGradient _Darcy_vel=-_grad_p[_qp]*_cond[_qp];

//  Moose::out << "Pore velocity " << _Darcy_vel(0) << std::endl;

  return _test[_i][_qp]*(_Darcy_vel*_grad_u[_qp]);
}

Real PrimaryConvection::computeQpJacobian()
{
  // the partial derivative of _grad_u is just _dphi[_j]
  RealGradient _Darcy_vel=-_grad_p[_qp]*_cond[_qp];

  return _test[_i][_qp]*(_Darcy_vel*_grad_phi[_j][_qp]);
}

Real PrimaryConvection::computeQpOffDiagJacobian(unsigned int /*jvar*/)
{
  return 0.0;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
