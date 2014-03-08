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
#ifndef NSMASSUNSPECIFIEDNORMALFLOWBC_H
#define NSMASSUNSPECIFIEDNORMALFLOWBC_H

#include "NSMassBC.h"


// Forward Declarations
class NSMassUnspecifiedNormalFlowBC;

template<>
InputParameters validParams<NSMassUnspecifiedNormalFlowBC>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * This class implements the mass equation boundary term with
 * the rho*(u.n) boundary integral computed implicitly.
 */
class NSMassUnspecifiedNormalFlowBC : public NSMassBC
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  NSMassUnspecifiedNormalFlowBC(const InputParameters & parameters);

protected:
=======

  NSMassUnspecifiedNormalFlowBC(const std::string & name, InputParameters parameters);

  virtual ~NSMassUnspecifiedNormalFlowBC(){}

protected:

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);
};
<<<<<<< HEAD
=======



#endif // NSMASSUNSPECIFIEDNORMALFLOWBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
