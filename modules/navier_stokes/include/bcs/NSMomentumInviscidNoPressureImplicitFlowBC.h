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
#ifndef NSMOMENTUMINVISCIDNOPRESSUREIMPLICITFLOWBC_H
#define NSMOMENTUMINVISCIDNOPRESSUREIMPLICITFLOWBC_H

#include "NSMomentumInviscidBC.h"


// Forward Declarations
class NSMomentumInviscidNoPressureImplicitFlowBC;

template<>
InputParameters validParams<NSMomentumInviscidNoPressureImplicitFlowBC>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * Momentum equation boundary condition used when pressure *is not*
 * integrated by parts, i.e. when there is "no pressure" term on the boundary.
 * In this case the flow is not specified either, so that the entire term is
 * handled implicitly.
 */
class NSMomentumInviscidNoPressureImplicitFlowBC : public NSMomentumInviscidBC
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  NSMomentumInviscidNoPressureImplicitFlowBC(const InputParameters & parameters);

protected:
=======
  NSMomentumInviscidNoPressureImplicitFlowBC(const std::string & name, InputParameters parameters);

  virtual ~NSMomentumInviscidNoPressureImplicitFlowBC(){}

protected:

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);
};
<<<<<<< HEAD
=======

#endif // NSMOMENTUMINVISCIDNOPRESSUREIMPLICITFLOWBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
