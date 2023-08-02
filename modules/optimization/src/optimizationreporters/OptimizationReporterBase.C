//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "MooseTypes.h"
#include "OptimizationReporterBase.h"
#include "OptUtils.h"
#include "libmesh/petsc_vector.h"

InputParameters
OptimizationReporterBase::validParams()
{
  InputParameters params = OptimizationData::validParams();
  params.registerBase("OptimizationReporterBase");
  params.addRequiredParam<std::vector<ReporterValueName>>(
      "parameter_names", "List of parameter names, one for each group of parameters.");
  params.suppressParameter<std::vector<VariableName>>("variable");
  params.suppressParameter<std::vector<std::string>>("variable_weight_names");
  params.addParam<std::vector<ReporterValueName>>(
      "equality_names", std::vector<ReporterValueName>(), "List of equality names.");
  params.addParam<std::vector<ReporterValueName>>(
      "inequality_names", std::vector<ReporterValueName>(), "List of inequality names.");
  params.registerBase("OptimizationReporterBase");
  return params;
}

OptimizationReporterBase::OptimizationReporterBase(const InputParameters & parameters)
  : OptimizationData(parameters),
    _parameter_names(getParam<std::vector<ReporterValueName>>("parameter_names")),
    _nparams(_parameter_names.size()),
    _parameters(_nparams),
    _gradients(_nparams),
    _equality_names(&getParam<std::vector<ReporterValueName>>("equality_names")),
    _n_eq_cons(_equality_names->size()),
    _eq_constraints(_n_eq_cons),
    _eq_gradients(_n_eq_cons),
    _inequality_names(&getParam<std::vector<ReporterValueName>>("inequality_names")),
    _n_ineq_cons(_inequality_names->size()),
    _ineq_constraints(_n_ineq_cons),
    _ineq_gradients(_n_ineq_cons)
{
  for (const auto & i : make_range(_nparams))
  {
    _parameters[i] =
        &declareValueByName<std::vector<Real>>(_parameter_names[i], REPORTER_MODE_REPLICATED);
    _gradients[i] = &declareValueByName<std::vector<Real>>("grad_" + _parameter_names[i],
                                                           REPORTER_MODE_REPLICATED);
  }
  for (const auto & i : make_range(_n_eq_cons))
  {
    _eq_constraints[i] =
        &declareValueByName<std::vector<Real>>(_equality_names->at(i), REPORTER_MODE_REPLICATED);
    _eq_gradients[i] = &declareValueByName<std::vector<Real>>("grad_" + _equality_names->at(i),
                                                              REPORTER_MODE_REPLICATED);
  }
  for (const auto & i : make_range(_n_ineq_cons))
  {
    _ineq_constraints[i] =
        &declareValueByName<std::vector<Real>>(_inequality_names->at(i), REPORTER_MODE_REPLICATED);
    _ineq_gradients[i] = &declareValueByName<std::vector<Real>>("grad_" + _inequality_names->at(i),
                                                                REPORTER_MODE_REPLICATED);
  }
}

Real
OptimizationReporterBase::computeObjective()
{
  // This will only be executed if measurement_values are available on the main app
  for (const auto i : index_range(_measurement_values))
    _misfit_values[i] = _simulation_values[i] - _measurement_values[i];

  Real val = 0.0;
  for (auto & misfit : _misfit_values)
    val += misfit * misfit;

  return val * 0.5;
}

void
OptimizationReporterBase::setMisfitToSimulatedValues()
{
  _misfit_values = _simulation_values;
}

// function only used for test objects
void
OptimizationReporterBase::setSimulationValuesForTesting(std::vector<Real> & data)
{
  _simulation_values.clear();
  _simulation_values = data;
}

void
OptimizationReporterBase::computeGradient(libMesh::PetscVector<Number> & gradient) const
{
  for (const auto & p : make_range(_nparams))
    if (_gradients[p]->size() != _nvalues[p])
      mooseError("The gradient for parameter ",
                 _parameter_names[p],
                 " has changed, expected ",
                 _nvalues[p],
                 " versus ",
                 _gradients[p]->size(),
                 ".");
  OptUtils::copyReporterIntoPetscVector(_gradients, gradient);
}

void
OptimizationReporterBase::setInitialCondition(libMesh::PetscVector<Number> & x)
{
  x.init(getNumParams());
  OptUtils::copyReporterIntoPetscVector(_parameters, x);
}

void
OptimizationReporterBase::updateParameters(const libMesh::PetscVector<Number> & x)
{
  OptUtils::copyPetscVectorIntoReporter(x, _parameters);
}

Real
OptimizationReporterBase::getLowerBound(dof_id_type i) const
{
  return _lower_bounds[i];
}

Real
OptimizationReporterBase::getUpperBound(dof_id_type i) const
{
  return _upper_bounds[i];
}

void
OptimizationReporterBase::computeEqualityConstraints(
    libMesh::PetscVector<Number> & eqs_constraints) const
{
  OptUtils::copyReporterIntoPetscVector(_eq_constraints, eqs_constraints);
}

void
OptimizationReporterBase::computeInequalityConstraints(
    libMesh::PetscVector<Number> & ineqs_constraints) const
{
  OptUtils::copyReporterIntoPetscVector(_ineq_constraints, ineqs_constraints);
}

void
OptimizationReporterBase::computeEqualityGradient(libMesh::PetscMatrix<Number> & jacobian) const
{
  for (const auto & p : make_range(_n_eq_cons))
    if (_eq_gradients[p]->size() != _nvalues[p])
      mooseError("The equality jacobian for parameter ",
                 _parameter_names[p],
                 " has changed, expected ",
                 _nvalues[p],
                 " versus ",
                 _eq_gradients[p]->size(),
                 ".");
  OptUtils::copyReporterIntoPetscMatrix(_eq_gradients, jacobian);
}

void
OptimizationReporterBase::computeInequalityGradient(libMesh::PetscMatrix<Number> & jacobian) const
{
  for (const auto & p : make_range(_n_ineq_cons))
    if (_ineq_gradients[p]->size() != _nvalues[p])
      mooseError("The inequality jacobian for parameter ",
                 _parameter_names[p],
                 " has changed, expected ",
                 _nvalues[p],
                 " versus ",
                 _ineq_gradients[p]->size(),
                 ".");
  OptUtils::copyReporterIntoPetscMatrix(_ineq_gradients, jacobian);
}
