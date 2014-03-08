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
#include "ACBulk.h"

// Forward Declarations

class ACGBPoly : public ACBulk<Real>
{
public:
  static InputParameters validParams();

  ACGBPoly(const InputParameters & parameters);
=======
#include "ACBulk.h"

#ifndef ACGBPOLY_H
#define ACGBPOLY_H

//Forward Declarations
class ACGBPoly;

template<>
InputParameters validParams<ACGBPoly>();

class ACGBPoly : public ACBulk
{
public:

  ACGBPoly(const std::string & name, InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeDFDOP(PFFunctionType type);
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

<<<<<<< HEAD
  const VariableValue & _c;
  unsigned int _c_var;

  const MaterialProperty<Real> & _mu;
  const MaterialProperty<Real> & _gamma;

  Real _en_ratio;
};
=======
private:
  /**
   * Coupled things come through as std::vector _refernces_.
   *
   * Since this is a reference it MUST be set in the Initialization List of the
   * constructor!
   */

  VariableValue & _c;
  unsigned int _c_var;
  MaterialProperty<Real> & _mu;
  MaterialProperty<Real> & _gamma;
  Real _en_ratio;

};
#endif //ACGBPOLY_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
