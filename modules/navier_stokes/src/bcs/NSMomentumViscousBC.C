<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "NSMomentumViscousBC.h"

registerMooseObject("NavierStokesApp", NSMomentumViscousBC);

InputParameters
NSMomentumViscousBC::validParams()
{
  InputParameters params = NSIntegratedBC::validParams();
  params.addClassDescription("This class corresponds to the viscous part of the 'natural' boundary "
                             "condition for the momentum equations.");
  params.addRequiredParam<unsigned>(
      "component", "(0,1,2) = (x,y,z) for which momentum component this BC is applied to");
  return params;
}

NSMomentumViscousBC::NSMomentumViscousBC(const InputParameters & parameters)
  : NSIntegratedBC(parameters),
    _component(getParam<unsigned>("component")),
    // Derivative computing object
    _vst_derivs(*this)
{
}

Real
NSMomentumViscousBC::computeQpResidual()
=======
#include "NSMomentumViscousBC.h"

template<>
InputParameters validParams<NSMomentumViscousBC>()
{
  InputParameters params = validParams<NSIntegratedBC>();

  // Required parameters
  params.addRequiredParam<unsigned>("component", "(0,1,2) = (x,y,z) for which momentum component this BC is applied to");

  return params;
}




NSMomentumViscousBC::NSMomentumViscousBC(const std::string & name, InputParameters parameters)
    : NSIntegratedBC(name, parameters),

      // Parameters to be specified in input file block...
      _component(getParam<unsigned>("component")),

      // Derivative computing object
      _vst_derivs(*this)
{
}



Real NSMomentumViscousBC::computeQpResidual()
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
  // n . (-tau) . v

  // Vector-valued test function
  RealVectorValue v_test;
  v_test(_component) = _test[_i][_qp];

  // The viscous contribution: n . tau . v
  Real visc_term = _normals[_qp] * (_viscous_stress_tensor[_qp] * v_test);

  // Note the sign...
  return -visc_term;
}

<<<<<<< HEAD
Real
NSMomentumViscousBC::computeQpJacobian()
{
  // See Eqns. (41)--(43) from the notes for the viscous boundary term contributions
  Real visc_term = 0.0;

  // Set variable names as in the notes
  const unsigned int k = _component;
  const unsigned int m = _component + 1; // _component = 0,1,2 -> m = 1,2,3 global variable number

  // FIXME: attempt calling shared dtau function
  for (unsigned int ell = 0; ell < LIBMESH_DIM; ++ell)
=======


Real NSMomentumViscousBC::computeQpJacobian()
{
  // See Eqns. (41)--(43) from the notes for the viscous boundary term contributions
  Real visc_term = 0.;

  // Set variable names as in the notes
  const unsigned k = _component;
  const unsigned m = _component+1; // _component = 0,1,2 -> m = 1,2,3 global variable number

  // FIXME: attempt calling shared dtau function
  for (unsigned ell=0; ell<LIBMESH_DIM; ++ell)
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
    visc_term += _vst_derivs.dtau(k, ell, m) * _normals[_qp](ell);

  // Multiply visc_term by test function
  visc_term *= _test[_i][_qp];

  // Note the sign...
  return -visc_term;
}

<<<<<<< HEAD
Real
NSMomentumViscousBC::computeQpOffDiagJacobian(unsigned jvar)
{
  if (isNSVariable(jvar))
  {

    // See Eqns. (41)--(43) from the notes for the viscous boundary
    // term contributions.

    // Map jvar into the variable m for our problem, regardless of
    // how Moose has numbered things.
    unsigned m = mapVarNumber(jvar);

    // Now compute viscous contribution
    Real visc_term = 0.0;

    // Set variable names as in the notes
    const unsigned int k = _component;

    for (unsigned int ell = 0; ell < LIBMESH_DIM; ++ell)
      visc_term += _vst_derivs.dtau(k, ell, m) * _normals[_qp](ell);

    // Multiply visc_term by test function
    visc_term *= _test[_i][_qp];

    // Note the sign...
    return -visc_term;
  }
  else
    return 0.0;
=======


Real NSMomentumViscousBC::computeQpOffDiagJacobian(unsigned jvar)
{
  // See Eqns. (41)--(43) from the notes for the viscous boundary
  // term contributions.

  // Map jvar into the variable m for our problem, regardless of
  // how Moose has numbered things.
  unsigned m = this->map_var_number(jvar);

  // Now compute viscous contribution
  Real visc_term = 0.;

  // Set variable names as in the notes
  const unsigned k = _component;

  for (unsigned ell=0; ell<LIBMESH_DIM; ++ell)
    visc_term += _vst_derivs.dtau(k, ell, m) * _normals[_qp](ell);

  // Multiply visc_term by test function
  visc_term *= _test[_i][_qp];

  // Note the sign...
  return -visc_term;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
}
