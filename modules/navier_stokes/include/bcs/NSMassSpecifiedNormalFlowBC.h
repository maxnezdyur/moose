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

#include "NSMassBC.h"

// Forward Declarations
=======
#ifndef NSMASSSPECIFIEDNORMALFLOWBC_H
#define NSMASSSPECIFIEDNORMALFLOWBC_H

#include "NSMassBC.h"


// Forward Declarations
class NSMassSpecifiedNormalFlowBC;

template<>
InputParameters validParams<NSMassSpecifiedNormalFlowBC>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * This class implements the mass equation boundary term with
 * a specified value of rho*(u.n) imposed weakly.
 *
 * Note: if you wish to impose rho*(u.n) = 0 weakly, you don't
 * actually need this class, that is the natural boundary condition.
 */
class NSMassSpecifiedNormalFlowBC : public NSMassBC
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  NSMassSpecifiedNormalFlowBC(const InputParameters & parameters);
=======

  NSMassSpecifiedNormalFlowBC(const std::string & name, InputParameters parameters);

  virtual ~NSMassSpecifiedNormalFlowBC(){}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);

  // Required parameters
<<<<<<< HEAD
  const Real _rhoun;
};
=======
  Real _rhoun;
};



#endif // NSMASSSPECIFIEDNORMALFLOWBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
