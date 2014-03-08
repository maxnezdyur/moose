<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PrimaryTimeDerivative.h"

registerMooseObject("ChemicalReactionsApp", PrimaryTimeDerivative);

InputParameters
PrimaryTimeDerivative::validParams()
{
  InputParameters params = TimeDerivative::validParams();
  params.addClassDescription("Derivative of primary species concentration wrt time");
  return params;
}

PrimaryTimeDerivative::PrimaryTimeDerivative(const InputParameters & parameters)
  : TimeDerivative(parameters), _porosity(getMaterialProperty<Real>("porosity"))
{
}
=======
#include "PrimaryTimeDerivative.h"
#include "Material.h"

template<>
InputParameters validParams<PrimaryTimeDerivative>()
{
  InputParameters params = validParams<TimeDerivative>();
  return params;
}

PrimaryTimeDerivative::PrimaryTimeDerivative(const std::string & name, InputParameters parameters) :
    TimeDerivative(name, parameters),
    _porosity(getMaterialProperty<Real>("porosity"))
{}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

Real
PrimaryTimeDerivative::computeQpResidual()
{
<<<<<<< HEAD
  return _porosity[_qp] * TimeDerivative::computeQpResidual();
=======
  return _porosity[_qp]*TimeDerivative::computeQpResidual();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

Real
PrimaryTimeDerivative::computeQpJacobian()
{
<<<<<<< HEAD
  return _porosity[_qp] * TimeDerivative::computeQpJacobian();
=======
  return _porosity[_qp]*TimeDerivative::computeQpJacobian();
}

Real PrimaryTimeDerivative::computeQpOffDiagJacobian(unsigned int /*jvar*/)
{
  return 0.0;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
