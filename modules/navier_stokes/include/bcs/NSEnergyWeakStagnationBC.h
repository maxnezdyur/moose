#ifndef NSENERGYWEAKSTAGNATIONBC_H
#define NSENERGYWEAKSTAGNATIONBC_H

#include "NSWeakStagnationBC.h"

// Forward Declarations
class NSEnergyWeakStagnationBC;

template<>
InputParameters validParams<NSEnergyWeakStagnationBC>();


/**
 * The inviscid energy BC term with specified normal flow.
 */
class NSEnergyWeakStagnationBC : public NSWeakStagnationBC
{

public:
  NSEnergyWeakStagnationBC(const std::string & name, InputParameters parameters);

  virtual ~NSEnergyWeakStagnationBC(){}

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);
};

#endif // NSENERGYWEAKSTAGNATIONBC_H
