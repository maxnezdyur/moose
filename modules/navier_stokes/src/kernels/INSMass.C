<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "INSMass.h"
#include "Function.h"

registerMooseObject("NavierStokesApp", INSMass);

InputParameters
INSMass::validParams()
{
  InputParameters params = INSBase::validParams();

  params.addClassDescription("This class computes the mass equation residual and Jacobian "
                             "contributions for the incompressible Navier-Stokes momentum "
                             "equation.");
  params.addParam<bool>(
      "pspg", false, "Whether to perform PSPG stabilization of the mass equation");
  params.addParam<FunctionName>("x_vel_forcing_func", 0, "The x-velocity mms forcing function.");
  params.addParam<FunctionName>("y_vel_forcing_func", 0, "The y-velocity mms forcing function.");
  params.addParam<FunctionName>("z_vel_forcing_func", 0, "The z-velocity mms forcing function.");
  return params;
}

INSMass::INSMass(const InputParameters & parameters)
  : INSBase(parameters),
    _pspg(getParam<bool>("pspg")),
    _x_ffn(getFunction("x_vel_forcing_func")),
    _y_ffn(getFunction("y_vel_forcing_func")),
    _z_ffn(getFunction("z_vel_forcing_func"))

{
}

Real
INSMass::computeQpResidual()
{
  // (div u) * q
  // Note: we (arbitrarily) multiply this term by -1 so that it matches the -p(div v)
  // term in the momentum equation.  Not sure if that is really important?
  Real r = -(_grad_u_vel[_qp](0) + _grad_v_vel[_qp](1) + _grad_w_vel[_qp](2)) * _test[_i][_qp];

  if (_pspg)
    r += computeQpPGResidual();

  return r;
}

Real
INSMass::computeQpPGResidual()
{
  RealVectorValue viscous_term =
      _laplace ? strongViscousTermLaplace() : strongViscousTermTraction();
  RealVectorValue transient_term =
      _transient_term ? timeDerivativeTerm() : RealVectorValue(0, 0, 0);
  RealVectorValue convective_term = _convective_term ? convectiveTerm() : RealVectorValue(0, 0, 0);
  Real r = -1. / _rho[_qp] * tau() * _grad_test[_i][_qp] *
           (strongPressureTerm() + gravityTerm() + viscous_term + convective_term + transient_term -
            RealVectorValue(_x_ffn.value(_t, _q_point[_qp]),
                            _y_ffn.value(_t, _q_point[_qp]),
                            _z_ffn.value(_t, _q_point[_qp])));

  return r;
}

Real
INSMass::computeQpJacobian()
{
  // Derivative wrt to p is zero
  Real r = 0;

  // Unless we are doing GLS stabilization
  if (_pspg)
    r += computeQpPGJacobian();

  return r;
}

Real
INSMass::computeQpPGJacobian()
{
  return -1. / _rho[_qp] * tau() * _grad_test[_i][_qp] * dStrongPressureDPressure();
}

Real
INSMass::computeQpOffDiagJacobian(unsigned jvar)
{
  if (jvar == _u_vel_var_number)
  {
    Real jac = -_grad_phi[_j][_qp](0) * _test[_i][_qp];
    if (_pspg)
      jac += computeQpPGOffDiagJacobian(0);
    return jac;
  }

  else if (jvar == _v_vel_var_number)
  {
    Real jac = -_grad_phi[_j][_qp](1) * _test[_i][_qp];
    if (_pspg)
      jac += computeQpPGOffDiagJacobian(1);
    return jac;
  }

  else if (jvar == _w_vel_var_number)
  {
    Real jac = -_grad_phi[_j][_qp](2) * _test[_i][_qp];
    if (_pspg)
      jac += computeQpPGOffDiagJacobian(2);
    return jac;
  }

  else
    return 0.0;
}

Real
INSMass::computeQpPGOffDiagJacobian(unsigned comp)
{
  RealVectorValue convective_term = _convective_term ? convectiveTerm() : RealVectorValue(0, 0, 0);
  RealVectorValue d_convective_term_d_u_comp =
      _convective_term ? dConvecDUComp(comp) : RealVectorValue(0, 0, 0);
  RealVectorValue viscous_term =
      _laplace ? strongViscousTermLaplace() : strongViscousTermTraction();
  RealVectorValue d_viscous_term_d_u_comp =
      _laplace ? dStrongViscDUCompLaplace(comp) : dStrongViscDUCompTraction(comp);
  RealVectorValue transient_term =
      _transient_term ? timeDerivativeTerm() : RealVectorValue(0, 0, 0);
  RealVectorValue d_transient_term_d_u_comp =
      _transient_term ? dTimeDerivativeDUComp(comp) : RealVectorValue(0, 0, 0);

  return -1. / _rho[_qp] * tau() * _grad_test[_i][_qp] *
             (d_convective_term_d_u_comp + d_viscous_term_d_u_comp + d_transient_term_d_u_comp) -
         1. / _rho[_qp] * dTauDUComp(comp) * _grad_test[_i][_qp] *
             (convective_term + viscous_term + transient_term + strongPressureTerm() +
              gravityTerm() -
              RealVectorValue(_x_ffn.value(_t, _q_point[_qp]),
                              _y_ffn.value(_t, _q_point[_qp]),
                              _z_ffn.value(_t, _q_point[_qp])));
=======
#include "INSMass.h"

template<>
InputParameters validParams<INSMass>()
{
  InputParameters params = validParams<Kernel>();

  // Coupled variables
  params.addRequiredCoupledVar("u", "x-velocity");
  params.addCoupledVar("v", "y-velocity"); // only required in 2D and 3D
  params.addCoupledVar("w", "z-velocity"); // only required in 3D
  params.addRequiredCoupledVar("p", "pressure");

  return params;
}



INSMass::INSMass(const std::string & name, InputParameters parameters) :
  Kernel(name, parameters),

  // Gradients
  _grad_u_vel(coupledGradient("u")),
  _grad_v_vel(_mesh.dimension() >= 2 ? coupledGradient("v") : _grad_zero),
  _grad_w_vel(_mesh.dimension() == 3 ? coupledGradient("w") : _grad_zero),

  // Variable numberings
  _u_vel_var_number(coupled("u")),
  _v_vel_var_number(_mesh.dimension() >= 2 ? coupled("v") : libMesh::invalid_uint),
  _w_vel_var_number(_mesh.dimension() == 3 ? coupled("w") : libMesh::invalid_uint),
  _p_var_number(coupled("p"))
{
}



Real INSMass::computeQpResidual()
{
  // (div u) * q
  // Note: we (arbitrarily) multilply this term by -1 so that it matches the -p(div v)
  // term in the momentum equation.  Not sure if that is really important?
  return -(_grad_u_vel[_qp](0) + _grad_v_vel[_qp](1) + _grad_w_vel[_qp](2)) * _test[_i][_qp];
}




Real INSMass::computeQpJacobian()
{
  // Derivative wrt to p is zero
  return 0;
}




Real INSMass::computeQpOffDiagJacobian(unsigned jvar)
{
  if (jvar == _u_vel_var_number)
    return -_grad_phi[_j][_qp](0) * _test[_i][_qp];

  else if (jvar == _v_vel_var_number)
    return -_grad_phi[_j][_qp](1) * _test[_i][_qp];

  else if (jvar == _w_vel_var_number)
    return -_grad_phi[_j][_qp](2) * _test[_i][_qp];
  else
    return 0;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
