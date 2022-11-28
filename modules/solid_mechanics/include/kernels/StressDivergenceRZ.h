#ifndef STRESSDIVERGENCERZ_H
#define STRESSDIVERGENCERZ_H

#include "Kernel.h"

//Forward Declarations
class StressDivergenceRZ;
class SymmElasticityTensor;
class SymmTensor;

template<>
InputParameters validParams<StressDivergenceRZ>();

class StressDivergenceRZ : public Kernel
{
public:

  StressDivergenceRZ(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  Real calculateJacobian( unsigned int ivar, unsigned int jvar );

  MaterialProperty<SymmTensor> & _stress;
  MaterialProperty<SymmElasticityTensor> & _Jacobian_mult;
  MaterialProperty<SymmTensor> & _d_stress_dT;

private:
  const unsigned int _component;

  const bool _rdisp_coupled;
  const bool _zdisp_coupled;
  const bool _temp_coupled;
  const unsigned int _rdisp_var;
  const unsigned int _zdisp_var;
  const unsigned int _temp_var;
};
#endif //STRESSDIVERGENCERZ_H
