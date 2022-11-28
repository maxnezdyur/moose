#ifndef ACBulk_H
#define ACBulk_H

#include "KernelValue.h"

//Forward Declarations
class ACBulk;

template<>
InputParameters validParams<ACBulk>();

class ACBulk : public KernelValue
{
public:

  ACBulk(const std::string & name, InputParameters parameters);

protected:
  enum PFFunctionType
  {
    Jacobian,
    Residual
  };
  virtual Real precomputeQpResidual();
  virtual Real precomputeQpJacobian();
  virtual Real computeDFDOP(PFFunctionType type) = 0;
  std::string _mob_name;
  MaterialProperty<Real> & _L;


private:


};
#endif //ACBulk_H
