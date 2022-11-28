#ifndef SOLIDMECHY
#define SOLIDMECHY

#include "SolidMech.h"

//Forward Declarations
class SolidMechY;

template<>
InputParameters validParams<SolidMechY>();


class SolidMechY : public SolidMech
{
public:

  SolidMechY(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  const unsigned int _mesh_dimension;

  unsigned int _x_var;
  VariableValue  & _x;
  VariableGradient & _grad_x;

  unsigned int _z_var;
  VariableValue  & _z;
  VariableGradient & _grad_z;
};
#endif //SOLIDMECHY
