#ifndef TENSORELASTICENERGYAUX_H
#define TENSORELASTICENERGYAUX_H

#include "AuxKernel.h"
#include "TensorMechanicsMaterial.h" //may not need this
#include "RankTwoTensor.h"

//Forward declarations
class TensorElasticEnergyAux;

template<>
InputParameters validParams<TensorElasticEnergyAux>();


class TensorElasticEnergyAux : public AuxKernel
{
public:
  TensorElasticEnergyAux( const std::string & name, InputParameters parameters );

  virtual ~TensorElasticEnergyAux() {}

protected:
  virtual Real computeValue();

private:
  MaterialProperty<RankTwoTensor> & _stress;
  MaterialProperty<RankTwoTensor> & _elastic_strain;

};

#endif // TENSORELASTICENERGYAUX_H
