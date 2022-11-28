#ifndef HEATCONDUCTIONTIMEDERIVATIVE
#define HEATCONDUCTIONTIMEDERIVATIVE

#include "TimeDerivative.h"
#include "Material.h"

//Forward Declarations
class HeatConductionTimeDerivative;

template<>
InputParameters validParams<HeatConductionTimeDerivative>();

class HeatConductionTimeDerivative : public TimeDerivative
{
public:

  HeatConductionTimeDerivative(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

private:
  const MaterialProperty<Real> & _specific_heat;
  const MaterialProperty<Real> & _density;
};
#endif //HEATCONDUCTIONTIMEDERIVATIVE
