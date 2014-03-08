<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PrimaryDiffusion.h"

registerMooseObject("ChemicalReactionsApp", PrimaryDiffusion);

InputParameters
PrimaryDiffusion::validParams()
{
  InputParameters params = Diffusion::validParams();
  params.addClassDescription("Diffusion of primary species");
  return params;
}

PrimaryDiffusion::PrimaryDiffusion(const InputParameters & parameters)
  : Diffusion(parameters), _diffusivity(getMaterialProperty<Real>("diffusivity"))
=======
#include "PrimaryDiffusion.h"

// If we use a material pointer we need to include the
// material class
#include "Material.h"

template<>
InputParameters validParams<PrimaryDiffusion>()
{
  InputParameters params = validParams<Diffusion>();
  return params;
}

PrimaryDiffusion::PrimaryDiffusion(const std::string & name, InputParameters parameters)
  :Diffusion(name,parameters),
   // We are grabbing the "diffusivity" material property
   _diffusivity(getMaterialProperty<Real>("diffusivity"))
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
}

Real
PrimaryDiffusion::computeQpResidual()
{
<<<<<<< HEAD
  return _diffusivity[_qp] * Diffusion::computeQpResidual();
=======
  // We're dereferencing the _diffusivity pointer to get to the
  // material properties vector... which gives us one property
  // value per quadrature point.

  // Also... we're reusing the Diffusion Kernel's residual
  // so that we don't have to recode that.
  //  if (_u[_qp]>=0.0)
    return _diffusivity[_qp]*Diffusion::computeQpResidual();

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}

Real
PrimaryDiffusion::computeQpJacobian()
{
<<<<<<< HEAD
  return _diffusivity[_qp] * Diffusion::computeQpJacobian();
=======
  // We're dereferencing the _diffusivity pointer to get to the
  // material properties vector... which gives us one property
  // value per quadrature point.

  // Also... we're reusing the Diffusion Kernel's residual
  // so that we don't have to recode that.
  return _diffusivity[_qp]*Diffusion::computeQpJacobian();
}

Real PrimaryDiffusion::computeQpOffDiagJacobian(unsigned int /*jvar*/)
{
  return 0.0;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
