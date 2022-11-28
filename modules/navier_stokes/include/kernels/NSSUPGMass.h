#ifndef NSSUPGMASS_H
#define NSSUPGMASS_H

#include "NSSUPGBase.h"

// Forward Declarations
class NSSUPGMass;

template<>
InputParameters validParams<NSSUPGMass>();

/**
 * Compute residual and Jacobian terms form the SUPG
 * terms in the mass equation.
 */
class NSSUPGMass : public NSSUPGBase
{
public:
  NSSUPGMass(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  // Single function for computing on and off-diagonal Jacobian
  // entries in a single function.  The input index is in Moose
  // variable numbering.
  Real compute_jacobian(unsigned var);
};

#endif // NSSUPGMASS_H
