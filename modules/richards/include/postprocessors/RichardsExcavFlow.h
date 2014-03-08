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

#include "SideIntegralVariablePostprocessor.h"
#include "MaterialPropertyInterface.h"
#include "RichardsVarNames.h"

// Forward Declarations
class Function;

/**
 * Records total mass flow into an excavation defined by a RichardsExcavGeom function
 */
class RichardsExcavFlow : public SideIntegralVariablePostprocessor
{
public:
  static InputParameters validParams();

  RichardsExcavFlow(const InputParameters & parameters);
=======
/*****************************************/
/* Written by andrew.wilkins@csiro.au    */
/* Please contact me if you make changes */
/*****************************************/

#ifndef RICHARDSEXCAVFLOW_H
#define RICHARDSEXCAVFLOW_H

#include "SideIntegralVariablePostprocessor.h"
#include "MaterialPropertyInterface.h"
#include "FunctionInterface.h"
#include "RichardsPorepressureNames.h"

//Forward Declarations
class RichardsExcavFlow;
class Function;

template<>
InputParameters validParams<RichardsExcavFlow>();

class RichardsExcavFlow :
public SideIntegralVariablePostprocessor,
public FunctionInterface
{
public:
  RichardsExcavFlow(const std::string & name, InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeQpIntegral();

<<<<<<< HEAD
  /// holds info regarding the Richards variables
  const RichardsVarNames & _richards_name_UO;

  /// the richards variable number for which we want the mass flow
  unsigned int _pvar;

  /// mass-flux of fluid (a vector in the multicomponent case)
  const MaterialProperty<std::vector<RealVectorValue>> & _flux;

  /// the RichardsExcavGeom that defines where on the boundary we'll compute the mass flux
  const Function & _func;
};
=======
  const RichardsPorepressureNames & _pp_name_UO;
  unsigned int _pvar;

  MaterialProperty<std::vector<Real> > &_viscosity;
  MaterialProperty<RealVectorValue> &_gravity;
  MaterialProperty<RealTensorValue> & _permeability;
  MaterialProperty<std::vector<Real> > &_rel_perm;
  MaterialProperty<std::vector<Real> > &_density;
  Function & _func;
};

#endif // RICHARDSEXCAVFLOW_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
