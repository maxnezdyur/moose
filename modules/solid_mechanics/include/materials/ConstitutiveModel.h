#ifndef CONSTITUTIVEMODEL_H
#define CONSTITUTIVEMODEL_H

#include "Material.h"

#include "SymmElasticityTensor.h"
#include "SymmTensor.h"

/**
 */

class ConstitutiveModel : public Material
{
public:
  ConstitutiveModel( const std::string & name,
                     InputParameters parameters );

  virtual ~ConstitutiveModel() {}

  virtual void initStatefulProperties(unsigned int n_points);

  virtual void computeStress( const Elem & /*current_elem*/,
                              unsigned /*qp*/,
                              const SymmElasticityTensor & elasticityTensor,
                              const SymmTensor & stress_old,
                              SymmTensor & strain_increment,
                              SymmTensor & stress_new );

  virtual bool modifyStrainIncrement(const Elem & /*elem*/,
                                     unsigned qp,
                                     SymmTensor & strain_increment,
                                     SymmTensor & d_strain_dT)
  {
    return applyThermalStrain(qp, strain_increment, d_strain_dT);
  }
  virtual bool updateElasticityTensor(unsigned /*qp*/, SymmElasticityTensor & /*elasticityTensor*/)
  {
    return false;
  }

  virtual bool applyThermalStrain(unsigned qp, SymmTensor & strain_increment, SymmTensor & d_strain_dT);

protected:

  const bool _has_temp;
  VariableValue & _temperature;
  VariableValue & _temperature_old;
  const Real _alpha;
  Function * const _alpha_function;
  const bool _has_stress_free_temp;
  const Real _stress_free_temp;

private:
  using Material::computeProperties;
  using Material::_qp;

};

template<>
InputParameters validParams<ConstitutiveModel>();

#endif // CONSTITUTIVEMODEL_H
