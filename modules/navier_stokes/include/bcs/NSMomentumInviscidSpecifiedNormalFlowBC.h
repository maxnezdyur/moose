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

#include "NSMomentumInviscidBC.h"

// Forward Declarations
=======
#ifndef NSMOMENTUMINVISCIDSPECIFIEDNORMALFLOWBC_H
#define NSMOMENTUMINVISCIDSPECIFIEDNORMALFLOWBC_H

#include "NSMomentumInviscidBC.h"


// Forward Declarations
class NSMomentumInviscidSpecifiedNormalFlowBC;

template<>
InputParameters validParams<NSMomentumInviscidSpecifiedNormalFlowBC>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * Momentum equation boundary condition in which pressure is specified (given)
 * and the value of the convective part is allowed to vary (is computed implicitly).
 */
class NSMomentumInviscidSpecifiedNormalFlowBC : public NSMomentumInviscidBC
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  NSMomentumInviscidSpecifiedNormalFlowBC(const InputParameters & parameters);

protected:
=======
  NSMomentumInviscidSpecifiedNormalFlowBC(const std::string & name, InputParameters parameters);

  virtual ~NSMomentumInviscidSpecifiedNormalFlowBC(){}

protected:

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);

<<<<<<< HEAD
  const VariableValue & _pressure;
  const Real _rhou_udotn;
};
=======
  // Aux Variables
  VariableValue& _pressure;

  // Required parameters
  Real _rhou_udotn;
};

#endif // NSMOMENTUMINVISCIDSPECIFIEDPRESSUREBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
