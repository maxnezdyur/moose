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

#include "SplitCHBase.h"

// Forward Declarations

/// The couple, SplitCHCRes and SplitCHWRes, splits the CH equation by replacing chemical potential with 'w'.
class SplitCHCRes : public SplitCHBase
{
public:
  static InputParameters validParams();

  SplitCHCRes(const InputParameters & parameters);
=======
#ifndef SPLITCHCRes_H
#define SPLITCHCRes_H

#include "SplitCHBase.h"


//Forward Declarations
class SplitCHCRes;

template<>
InputParameters validParams<SplitCHCRes>();

class SplitCHCRes : public SplitCHBase
{
public:

  SplitCHCRes(const std::string & name, InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

<<<<<<< HEAD
  const MaterialProperty<Real> & _kappa;
  unsigned int _w_var;
  const VariableValue & _w;
};
=======
private:
  std::string _kappa_name;
  MaterialProperty<Real> & _kappa;
  unsigned int _w_var;
  VariableValue & _w;
  VariableGradient & _grad_w;

};
#endif //SPLITCHCRes_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
