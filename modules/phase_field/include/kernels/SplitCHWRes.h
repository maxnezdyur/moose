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

#include "SplitCHWResBase.h"

/**
 * SplitCHWRes creates the residual for the chemical
 * potential in the split form of the Cahn-Hilliard
 * equation with a scalar (isotropic) mobility.
 */
class SplitCHWRes : public SplitCHWResBase<Real>
{
public:
  static InputParameters validParams();

  SplitCHWRes(const InputParameters & parameters);
};
=======
#ifndef SPLITCHWRes_H
#define SPLITCHWRes_H

#include "Kernel.h"


//Forward Declarations
class SplitCHWRes;

template<>
InputParameters validParams<SplitCHWRes>();

class SplitCHWRes : public Kernel
{
public:

  SplitCHWRes(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);
private:
  std::string _mob_name;
  MaterialProperty<Real> & _mob;
  unsigned int _c_var;
  VariableValue & _c;
};
#endif //SPLITCHWRes_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
