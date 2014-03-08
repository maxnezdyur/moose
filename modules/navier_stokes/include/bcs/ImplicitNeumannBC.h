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
#ifndef IMPLICITNEUMANNBC_H
#define IMPLICITNEUMANNBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "IntegratedBC.h"

// Forward Declarations
<<<<<<< HEAD
=======
class ImplicitNeumannBC;

template<>
InputParameters validParams<ImplicitNeumannBC>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * This class implements a form of the Neumann boundary condition in
 * which the boundary term is treated "implicitly".  This concept is
 * discussed by Griffiths, Papanastiou, and others.
 */
class ImplicitNeumannBC : public IntegratedBC
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  ImplicitNeumannBC(const InputParameters & parameters);

  virtual ~ImplicitNeumannBC() {}
=======
  ImplicitNeumannBC(const std::string & name, InputParameters parameters);

  virtual ~ImplicitNeumannBC(){}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);
};
<<<<<<< HEAD
=======


#endif // IMPLICITNEUMANNBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
