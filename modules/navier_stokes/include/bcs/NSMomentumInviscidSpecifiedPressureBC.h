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
#ifndef NSMOMENTUMINVISCIDSPECIFIEDPRESSUREBC_H
#define NSMOMENTUMINVISCIDSPECIFIEDPRESSUREBC_H

#include "NSMomentumInviscidBC.h"


// Forward Declarations
class NSMomentumInviscidSpecifiedPressureBC;

template<>
InputParameters validParams<NSMomentumInviscidSpecifiedPressureBC>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * Momentum equation boundary condition in which pressure is specified (given)
 * and the value of the convective part is allowed to vary (is computed implicitly).
 */
class NSMomentumInviscidSpecifiedPressureBC : public NSMomentumInviscidBC
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  NSMomentumInviscidSpecifiedPressureBC(const InputParameters & parameters);

  virtual ~NSMomentumInviscidSpecifiedPressureBC() {}

protected:
=======
  NSMomentumInviscidSpecifiedPressureBC(const std::string & name, InputParameters parameters);

  virtual ~NSMomentumInviscidSpecifiedPressureBC(){}

protected:

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);

  // The specified value of the pressure.  This must be passed to the parent's
<<<<<<< HEAD
  // pressureQpResidualHelper function.
  Real _specified_pressure;
};
=======
  // pressure_qp_residual function.
  Real _specified_pressure;
};

#endif // NSMOMENTUMINVISCIDSPECIFIEDPRESSUREBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
