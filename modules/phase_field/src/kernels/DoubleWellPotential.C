<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "DoubleWellPotential.h"

InputParameters
DoubleWellPotential::validParams()
{
  InputParameters params = KernelValue::validParams();
  params.addClassDescription(
      "Simple demonstration Allen-Cahn Kernel using an algebraic double-well potential");
  params.addParam<MaterialPropertyName>("mob_name", "L", "The mobility used with the kernel");
=======
#include "DoubleWellPotential.h"

// Algebraic double well potential.

template<>
InputParameters validParams<DoubleWellPotential>()
{
  InputParameters params = validParams<KernelValue>();
  params.addParam<std::string>("mob_name","L","The mobility used with the kernel");
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  return params;
}

<<<<<<< HEAD
DoubleWellPotential::DoubleWellPotential(const InputParameters & parameters)
  : ACBulk<Real>(parameters)
=======
DoubleWellPotential::DoubleWellPotential(const std::string & name, InputParameters parameters) :
  ACBulk( name, parameters )
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
}

Real
DoubleWellPotential::computeDFDOP(PFFunctionType type)
{
  switch (type)
  {
<<<<<<< HEAD
    case Residual:
      return _u[_qp] * _u[_qp] * _u[_qp] - _u[_qp];

    case Jacobian:
      return _phi[_j][_qp] * (3.0 * _u[_qp] * _u[_qp] - 1.0);
  }

  mooseError("Invalid type passed in");
}
=======
  case Residual:
    return _u[_qp]*_u[_qp]*_u[_qp] - _u[_qp] ;

  case Jacobian:
    return _phi[_j][_qp]*(3*_u[_qp]*_u[_qp] - 1. );
  }

  mooseError("Invalid type passed in");

}


>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
