<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

// Navier-Stokes includes
#include "NSTemperatureL2.h"
#include "NS.h"

// MOOSE includes
#include "MooseMesh.h"

registerMooseObject("NavierStokesApp", NSTemperatureL2);

InputParameters
NSTemperatureL2::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addClassDescription(
      "This class was originally used to solve for the temperature using an L2-projection.");
  params.addRequiredCoupledVar(NS::velocity_x, "x-direction velocity component");
  params.addCoupledVar(NS::velocity_y, "y-direction velocity component"); // only reqiured in >= 2D
  params.addCoupledVar(NS::velocity_z, "z-direction velocity component"); // only required in 3D
  params.addRequiredCoupledVar("rhoe", "Total energy");
  params.addRequiredCoupledVar(NS::density, "Density");
  return params;
}

NSTemperatureL2::NSTemperatureL2(const InputParameters & parameters)
  : Kernel(parameters),
    _rho_var(coupled(NS::density)),
    _rho(coupledValue(NS::density)),
    _rhoe_var(coupled("rhoe")),
    _rhoe(coupledValue("rhoe")),
    _u_vel_var(coupled(NS::velocity_x)),
    _u_vel(coupledValue(NS::velocity_x)),
    _v_vel_var(_mesh.dimension() >= 2 ? coupled(NS::velocity_y) : libMesh::invalid_uint),
    _v_vel(_mesh.dimension() >= 2 ? coupledValue(NS::velocity_y) : _zero),
    _w_vel_var(_mesh.dimension() == 3 ? coupled(NS::velocity_z) : libMesh::invalid_uint),
    _w_vel(_mesh.dimension() == 3 ? coupledValue(NS::velocity_z) : _zero),
    _c_v(getMaterialProperty<Real>("c_v"))
{
}
=======
#include "NSTemperatureL2.h"


template<>
InputParameters validParams<NSTemperatureL2>()
{
  InputParameters params = validParams<Kernel>();

  // Make coupled variables required in the input file
  params.addRequiredCoupledVar("u", "");
  params.addRequiredCoupledVar("v", "");
  params.addCoupledVar("w", ""); // only required in 3D
  params.addRequiredCoupledVar("pe", "");
  params.addRequiredCoupledVar("p", "");

  return params;
}

NSTemperatureL2::NSTemperatureL2(const std::string & name, InputParameters parameters)
  :Kernel(name, parameters),
    _p_var(coupled("p")),
    _p(coupledValue("p")),
    _pe_var(coupled("pe")),
    _pe(coupledValue("pe")),
    _u_vel_var(coupled("u")),
    _u_vel(coupledValue("u")),
    _v_vel_var(coupled("v")),
    _v_vel(coupledValue("v")),
    _w_vel_var(_mesh.dimension() == 3 ? coupled("w") : 0),
    _w_vel(_mesh.dimension() == 3 ? coupledValue("w") : _zero),
    _c_v(getMaterialProperty<Real>("c_v"))
{}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

Real
NSTemperatureL2::computeQpResidual()
{
<<<<<<< HEAD
  Real value = 1.0 / _c_v[_qp];

  const Real et = _rhoe[_qp] / _rho[_qp];
  const RealVectorValue vec(_u_vel[_qp], _v_vel[_qp], _w_vel[_qp]);
=======
  Real value = 1.0/_c_v[_qp];

  Real et = _pe[_qp]/_p[_qp];

  RealVectorValue vec(_u_vel[_qp],_v_vel[_qp],_w_vel[_qp]);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  value *= et - ((vec * vec) / 2.0);

  // L2-projection
<<<<<<< HEAD
  return (_u[_qp] - value) * _test[_i][_qp];
}

Real
NSTemperatureL2::computeQpJacobian()
{
  return _phi[_j][_qp] * _test[_i][_qp];
=======
  return (_u[_qp]-value)*_test[_i][_qp];
}


Real
NSTemperatureL2::computeQpJacobian()
{
  Real value = 1.0/_c_v[_qp];

  Real et = _pe[_qp]/_p[_qp];

  RealVectorValue vec(_u_vel[_qp],_v_vel[_qp],_w_vel[_qp]);

  value *= et - ((vec * vec) / 2.0);

  return _phi[_j][_qp]*_test[_i][_qp];
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

Real
NSTemperatureL2::computeQpOffDiagJacobian(unsigned int jvar)
{
<<<<<<< HEAD
  if (jvar == _rho_var)
  {
    const Real et = (_rhoe[_qp] / (-_rho[_qp] * _rho[_qp])) * _phi[_j][_qp];
    Real value = et / _c_v[_qp];

    return -value * _test[_i][_qp];
  }
  else if (jvar == _rhoe_var)
  {
    const Real et = _phi[_j][_qp] / _rho[_qp];
    Real value = et / _c_v[_qp];

    return -value * _test[_i][_qp];
  }

  return 0.0;
}
=======
  if(jvar == _p_var)
  {
    Real value = 1.0/_c_v[_qp];

    Real et = (_pe[_qp]/(-_p[_qp]*_p[_qp]))*_phi[_j][_qp];

    value *= et;

    return (-value)*_test[_i][_qp];
  }
  else if(jvar == _pe_var)
  {
    Real value = 1.0/_c_v[_qp];

    Real et = _phi[_j][_qp]/_p[_qp];

    value *= et;

    return (-value)*_test[_i][_qp];
  }

  return 0;
}

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
