#ifndef ELASTICMODEL_H
#define ELASTICMODEL_H

#include "ConstitutiveModel.h"

class ElasticModel : public ConstitutiveModel
{
public:
  ElasticModel( const std::string & name,
                InputParameters parameters );
  virtual ~ElasticModel();

protected:

  /// Compute the stress (sigma += deltaSigma)
  virtual void computeStress( const Elem & current_elem,
                              unsigned qp,
                              const SymmElasticityTensor & elasticity_tensor,
                              const SymmTensor & stress_old,
                              SymmTensor & strain_increment,
                              SymmTensor & stress_new );

};

template<>
InputParameters validParams<ElasticModel>();


#endif
