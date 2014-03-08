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

#include "ACGrGrBase.h"

// Forward Declarations

/**
 * This kernel calculates the residual for grain growth for a single phase,
 * poly-crystal system. A single material property gamma_asymm is used for
 * the prefactor of the cross-terms between order parameters.
 */
class ACGrGrPoly : public ACGrGrBase
{
public:
  static InputParameters validParams();

  ACGrGrPoly(const InputParameters & parameters);
=======
#include "ACBulk.h"

#ifndef ACGRGRPOLY_H
#define ACGRGRPOLY_H

//Forward Declarations
class ACGrGrPoly;

template<>
InputParameters validParams<ACGrGrPoly>();

/**This kernel calculates the residual for grain growth.  It calculates the residual of the ith order parameter, and the values of all other order parameters are coupled variables and are stored in vals
**/
class ACGrGrPoly : public ACBulk
{
public:

  ACGrGrPoly(const std::string & name, InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeDFDOP(PFFunctionType type);
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

<<<<<<< HEAD
  const MaterialProperty<Real> & _gamma;
};
=======
private:
  /**
   * Coupled things come through as std::vector _refernces_.
   *
   * Since this is a reference it MUST be set in the Initialization List of the
   * constructor!
   */

  std::vector<VariableValue *> _vals;
  std::vector<unsigned int> _vals_var;

  MaterialProperty<Real> & _mu;
  MaterialProperty<Real> & _gamma;
  MaterialProperty<Real> & _tgrad_corr_mult;

  bool _has_T;
  VariableGradient * _grad_T;

  unsigned int _ncrys;

//  Real _gamma;

};
#endif //ACGRGRPOLY_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
