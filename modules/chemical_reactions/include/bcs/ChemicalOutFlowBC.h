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

#include "IntegratedBC.h"

// Forward Declarations
=======
#ifndef CHEMICALOUTFLOWBC_H
#define CHEMICALOUTFLOWBC_H

#include "IntegratedBC.h"

//libMesh includes
//#include "vector_value.h"


//Forward Declarations
class ChemicalOutFlowBC;

template<>
InputParameters validParams<ChemicalOutFlowBC>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

/**
 * Implements a simple constant VectorNeumann BC where grad(u)=value on the boundary.
 * Uses the term produced from integrating the diffusion operator by parts.
 */
class ChemicalOutFlowBC : public IntegratedBC
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  ChemicalOutFlowBC(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;

private:
  /// Diffusivity
  const MaterialProperty<Real> & _diff;
  /// Porosity
  const MaterialProperty<Real> & _porosity;
};
=======

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  ChemicalOutFlowBC(const std::string & name, InputParameters parameters);

 virtual ~ChemicalOutFlowBC(){}

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  /**
   * Vector to dot with the normal.
   */
  MaterialProperty<Real> & _diff;
  MaterialProperty<Real> & _porosity;

//  std::vector<RealGradient> & _grad_p;

};

#endif //NEUMANNBC_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
