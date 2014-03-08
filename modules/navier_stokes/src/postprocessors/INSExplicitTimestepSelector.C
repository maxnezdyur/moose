<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "INSExplicitTimestepSelector.h"

#include "libmesh/quadrature.h"

registerMooseObject("NavierStokesApp", INSExplicitTimestepSelector);

InputParameters
INSExplicitTimestepSelector::validParams()
{
  InputParameters params = ElementPostprocessor::validParams();

  params.addClassDescription("Postprocessor that computes the minimum value of h_min/|u|, where "
                             "|u| is coupled in as an aux variable.");
=======
#include "INSExplicitTimestepSelector.h"

template<>
InputParameters validParams<INSExplicitTimestepSelector>()
{
  InputParameters params = validParams<ElementPostprocessor>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  // Coupled variables
  params.addRequiredCoupledVar("vel_mag", "Velocity magnitude");

<<<<<<< HEAD
  // Required parameters
  params.addRequiredParam<Real>("beta",
                                "0 < beta < 1, choose some fraction of the limiting timestep size");

  // Optional parameters
  params.addParam<MaterialPropertyName>("mu_name", "mu", "The name of the dynamic viscosity");
  params.addParam<MaterialPropertyName>("rho_name", "rho", "The name of the density");
=======
  // Material properties
  params.addRequiredParam<Real>("mu", "dynamic viscosity");
  params.addRequiredParam<Real>("rho", "density");

  // Other parameters
  params.addRequiredParam<Real>("beta", "0 < beta < 1, choose some fraction of the limiting timestep size");
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  return params;
}

<<<<<<< HEAD
INSExplicitTimestepSelector::INSExplicitTimestepSelector(const InputParameters & parameters)
  : ElementPostprocessor(parameters),
    _vel_mag(coupledValue("vel_mag")),

    // Other parameters
    _beta(getParam<Real>("beta")),

    // Material properties
    _mu(getMaterialProperty<Real>("mu_name")),
    _rho(getMaterialProperty<Real>("rho_name"))
{
}

INSExplicitTimestepSelector::~INSExplicitTimestepSelector() {}
=======


INSExplicitTimestepSelector::INSExplicitTimestepSelector(const std::string & name, InputParameters parameters) :
  ElementPostprocessor(name, parameters),
  _vel_mag(coupledValue("vel_mag")),

  // Material properties
  _mu(getParam<Real>("mu")),
  _rho(getParam<Real>("rho")),

  // Other parameters
  _beta(getParam<Real>("beta"))
{
}



INSExplicitTimestepSelector::~INSExplicitTimestepSelector()
{
}


>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

void
INSExplicitTimestepSelector::initialize()
{
  _value = std::numeric_limits<Real>::max();
}

<<<<<<< HEAD
=======


>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
void
INSExplicitTimestepSelector::execute()
{
  Real h_min = _current_elem->hmin();

  // The space dimension plays a role in the diffusive dt limit.  The more
  // space dimensions there are, the smaller this limit is.
  Real dim = static_cast<Real>(_current_elem->dim());

<<<<<<< HEAD
  for (unsigned qp = 0; qp < _qrule->n_points(); ++qp)
=======
  for (unsigned qp=0; qp<_qrule->n_points(); ++qp)
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  {
    // Don't divide by zero...
    Real vel_mag = std::max(_vel_mag[qp], std::numeric_limits<Real>::epsilon());

    // For explicit Euler, we always have to satisfy the Courant condition for stability.
<<<<<<< HEAD
    Real courant_limit_dt = h_min / vel_mag;
=======
    Real courant_limit_dt =  h_min / vel_mag;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

    // But we also have to obey the diffusive time restriction,
    // dt <= 1/(2*nu)*(1/h1^2 + 1/h2^2 + 1/h3^2)^(-1) <=
    //    <= h_min^2 / n_dim / (2*nu)
<<<<<<< HEAD
    Real diffusive_limit_dt = 0.5 * h_min * h_min / (_mu[qp] / _rho[qp]) / dim;

    // And the "combined" condition, dt <= 2*nu/|u|^2
    Real combined_limit_dt = 2. * (_mu[qp] / _rho[qp]) / vel_mag / vel_mag;
=======
    Real diffusive_limit_dt = 0.5 * h_min * h_min / (_mu / _rho) / dim;

    // And the "combined" condition, dt <= 2*nu/|u|^2
    Real combined_limit_dt = 2.*(_mu / _rho) / vel_mag / vel_mag;
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

    // // Debugging:
    // Moose::out << "courant_limit_dt   = " << courant_limit_dt   << "\n"
    //           << "diffusive_limit_dt = " << diffusive_limit_dt << "\n"
    //           << "combined_limit_dt  = " << combined_limit_dt
    //           << std::endl;

<<<<<<< HEAD
    _value = std::min(
        _value,
        _beta * std::min(std::min(courant_limit_dt, diffusive_limit_dt), combined_limit_dt));
  }
}

Real
INSExplicitTimestepSelector::getValue()
{
  return _value;
}

void
INSExplicitTimestepSelector::finalize()
{
  gatherMin(_value);
}
=======
    _value = std::min(_value, _beta * std::min(std::min(courant_limit_dt, diffusive_limit_dt), combined_limit_dt));
  }
}



Real
INSExplicitTimestepSelector::getValue()
{
  Parallel::min(_value);
  return _value;
}


>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

void
INSExplicitTimestepSelector::threadJoin(const UserObject & uo)
{
  const INSExplicitTimestepSelector & pps = dynamic_cast<const INSExplicitTimestepSelector &>(uo);
  _value = std::min(_value, pps._value);
}
