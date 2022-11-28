#ifndef SOLIDMECHIMPLICITEULER_H
#define SOLIDMECHIMPLICITEULER_H

#include "SecondDerivativeImplicitEuler.h"
#include "Material.h"

//Forward Declarations
class SolidMechImplicitEuler;

template<>
InputParameters validParams<SolidMechImplicitEuler>();

class SolidMechImplicitEuler : public SecondDerivativeImplicitEuler
{
public:

  SolidMechImplicitEuler(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  virtual Real scaling();

private:
  MaterialProperty<Real> & _density;
  const bool _artificial_scaling_set;
  const Real _artificial_scaling;
};
#endif //SOLIDMECHIMPLICITEULER_H
