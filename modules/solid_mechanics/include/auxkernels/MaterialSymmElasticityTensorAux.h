/*************************************************************************
*
*  Welcome to HYRAX!
*  Andrea M. Jokisaari
*  CASL/MOOSE
*
*  18 April 2012
*
*************************************************************************/

#ifndef MATERIALSYMMELASTICITYTENSORAUX_H
#define MATERIALSYMMELASTICITYTENSORAUX_H

#include "AuxKernel.h"
#include "SymmElasticityTensor.h"

//Forward declarations
class MaterialSymmElasticityTensorAux;
class SymmElasticityTensor;

template<>
InputParameters validParams<MaterialSymmElasticityTensorAux>();

class MaterialSymmElasticityTensorAux : public AuxKernel
{
public:

  MaterialSymmElasticityTensorAux(const std::string & name, InputParameters parameters);

protected:

  virtual Real computeValue();
  std::string _tensor_matpro;
  int _index;

private:

  MaterialProperty<SymmElasticityTensor> & _tensor_prop;
};

#endif //MATERIALSYMMELASTICITYTENSORAUX_H
