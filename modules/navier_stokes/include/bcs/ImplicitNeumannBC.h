#ifndef IMPLICITNEUMANNBC_H
#define IMPLICITNEUMANNBC_H

#include "IntegratedBC.h"

// Forward Declarations
class ImplicitNeumannBC;

template<>
InputParameters validParams<ImplicitNeumannBC>();

/**
 * This class implements a form of the Neumann boundary condition in
 * which the boundary term is treated "implicitly".  This concept is
 * discussed by Griffiths, Papanastiou, and others.
 */
class ImplicitNeumannBC : public IntegratedBC
{
public:
  ImplicitNeumannBC(const std::string & name, InputParameters parameters);

  virtual ~ImplicitNeumannBC(){}

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);
};


#endif // IMPLICITNEUMANNBC_H
