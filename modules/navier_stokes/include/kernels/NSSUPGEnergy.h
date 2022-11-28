#ifndef NSSUPGENERGY_H
#define NSSUPGENERGY_H

#include "NSSUPGBase.h"

// Forward Declarations
class NSSUPGEnergy;

template<>
InputParameters validParams<NSSUPGEnergy>();

/**
 * Compute residual and Jacobian terms form the SUPG
 * terms in the energy equation.
 */
class NSSUPGEnergy : public NSSUPGBase
{
public:
  NSSUPGEnergy(const std::string & name, InputParameters parameters);

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

#endif // NSSUPGENERGY_H
