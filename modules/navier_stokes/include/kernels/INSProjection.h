#ifndef INSPROJECTION_H
#define INSPROJECTION_H

#include "Kernel.h"

// Forward Declarations
class INSProjection;

template<>
InputParameters validParams<INSProjection>();

/**
 * This class computes the "projection" part of the "split" method for
 * solving incompressible Navier-Stokes.  This is a time-varying equation
 * for u that is coupled to both the acceleration "a" and the pressue.
 *
 * Do not use, USE INSChorinCorrector and related classes instead.
 */
class INSProjection : public Kernel
{
public:
  INSProjection(const std::string & name, InputParameters parameters);

  virtual ~INSProjection(){}

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);

  // Coupled variables
  VariableValue& _a1;
  VariableValue& _a2;
  VariableValue& _a3;

  // Gradients
  VariableGradient& _grad_p;

  // Variable numberings
  unsigned _a1_var_number;
  unsigned _a2_var_number;
  unsigned _a3_var_number;
  unsigned _p_var_number;

  // Material properties
  Real _rho;

  // Parameters
  unsigned _component;
};


#endif // INSPROJECTION_H
