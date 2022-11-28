#ifndef NSMOMENTUMINVISCIDFLUX_H
#define NSMOMENTUMINVISCIDFLUX_H

#include "NSKernel.h"


// ForwardDeclarations
class NSMomentumInviscidFlux;

template<>
InputParameters validParams<NSMomentumInviscidFlux>();



/**
 * The inviscid flux (convective + pressure terms) for the
 * momentum conservation equations.
 */
class NSMomentumInviscidFlux : public NSKernel
{
public:

  NSMomentumInviscidFlux(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  // Coupled variables
  VariableValue & _pressure;

  // Parameters
  unsigned _component;

private:
  // To be used from both the on and off-diagonal
  // computeQpJacobian functions.  Variable numbering
  // should be in the canonical ordering regardless of
  // Moose's numbering.
  Real compute_jacobian(unsigned m);
};

#endif
