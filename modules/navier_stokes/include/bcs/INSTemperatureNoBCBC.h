<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once
=======
#ifndef INSTEMPERATURENOBCBC_H
#define INSTEMPERATURENOBCBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "IntegratedBC.h"

// Forward Declarations
<<<<<<< HEAD
=======
class INSTemperatureNoBCBC;

template<>
InputParameters validParams<INSTemperatureNoBCBC>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * This class implements the "No BC" boundary condition
 * discussed by Griffiths, Papanastiou, and others.
 */
class INSTemperatureNoBCBC : public IntegratedBC
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  INSTemperatureNoBCBC(const InputParameters & parameters);

  virtual ~INSTemperatureNoBCBC() {}
=======
  INSTemperatureNoBCBC(const std::string & name, InputParameters parameters);

  virtual ~INSTemperatureNoBCBC(){}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);

<<<<<<< HEAD
  const MaterialProperty<Real> & _k;
};
=======
  Real _k;
};


#endif // INSTEMPERATURENOBCBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
