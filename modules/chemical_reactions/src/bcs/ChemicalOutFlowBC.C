<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ChemicalOutFlowBC.h"

registerMooseObject("ChemicalReactionsApp", ChemicalOutFlowBC);

InputParameters
ChemicalOutFlowBC::validParams()
{
  InputParameters params = IntegratedBC::validParams();
  params.addClassDescription("Chemical flux boundary condition");
  return params;
}

ChemicalOutFlowBC::ChemicalOutFlowBC(const InputParameters & parameters)
  : IntegratedBC(parameters),
    _diff(getMaterialProperty<Real>("diffusivity")),
    _porosity(getMaterialProperty<Real>("porosity"))
=======
#include "ChemicalOutFlowBC.h"
#include "Material.h"

template<>
InputParameters validParams<ChemicalOutFlowBC>()
{
  InputParameters params = validParams<IntegratedBC>();
  return params;
}

ChemicalOutFlowBC::ChemicalOutFlowBC(const std::string & name, InputParameters parameters)
  :IntegratedBC(name, parameters),
   _diff(getMaterialProperty<Real>("diffusivity")),
   _porosity(getMaterialProperty<Real>("porosity"))
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
}

Real
ChemicalOutFlowBC::computeQpResidual()
{
<<<<<<< HEAD
  return -_test[_i][_qp] * _porosity[_qp] * _diff[_qp] * _grad_u[_qp] * _normals[_qp];
}

Real
ChemicalOutFlowBC::computeQpJacobian()
{
  return -_test[_i][_qp] * _porosity[_qp] * _diff[_qp] * _grad_phi[_j][_qp] * _normals[_qp];
=======
//    RealGradient _Darcy_vel = -_cond*_grad_p[_qp];
//    Moose::out<<"Darcy velocity" << _Darcy_vel(1);
//    Moose::out << "porosity,diffusivity, cond " << _porosity <<" " << _diff <<" "<< _cond << std::endl;

  Real var = -_test[_i][_qp]*_porosity[_qp]*_diff[_qp]*_grad_u[_qp]*_normals[_qp];

//    if (var <= 1.0e-12)
//      var=0.0;

//    Moose::out << "utlet_flux " << var << std::endl;

  return var;
}


Real
ChemicalOutFlowBC::computeQpJacobian()
{
//    RealGradient _Darcy_vel = -_cond*_grad_p[_qp];
  Real var = -_test[_i][_qp]*_porosity[_qp]*_diff[_qp]*_grad_phi[_j][_qp]*_normals[_qp];

  return var;

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
