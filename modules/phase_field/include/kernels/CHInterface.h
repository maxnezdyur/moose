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

#include "CHInterfaceBase.h"

/**
 * This is the Cahn-Hilliard equation base class that implements the interfacial
 * or gradient energy term of the equation. With a scalar (isotropic) mobility.
 */
class CHInterface : public CHInterfaceBase<Real>
{
public:
  static InputParameters validParams();

  CHInterface(const InputParameters & parameters);
};
=======
#ifndef CHInterface_H
#define CHInterface_H

#include "Kernel.h"
#include "Material.h"

//Forward Declarations
class CHInterface;

template<>
InputParameters validParams<CHInterface>();
/** This is the Cahn-Hilliard equation base class that implements the interfacial or gradient energy term of the equation.
 *  See M.R. Tonks et al. / Computational Materials Science 51 (2012) 20–29 for more information.
 **/

class CHInterface : public Kernel
{
public:

  CHInterface(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:

  std::string _kappa_name;
  std::string _mob_name;
  std::string _Dmob_name;
  std::string _grad_mob_name;
  bool _implicit;

  MaterialProperty<Real> & _kappa;
  MaterialProperty<Real> & _M;
  bool _has_MJac;
  MaterialProperty<Real> * _DM;
  MaterialProperty<RealGradient> & _grad_M;
  MaterialProperty<RealGradient> * _Dgrad_Mnp;
  MaterialProperty<Real> * _Dgrad_Mngp;

  VariableSecond & _second_u;
  VariableSecond & _second_u_old;
  VariableTestSecond & _second_test;
  VariablePhiSecond & _second_phi;
};
#endif //CHInterface_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
