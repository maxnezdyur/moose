#ifndef STRESSDIVERGENCE_H
#define STRESSDIVERGENCE_H

#include "Kernel.h"

//Forward Declarations
class ColumnMajorMatrix;
class StressDivergence;
class SymmElasticityTensor;
class SymmTensor;

template<>
InputParameters validParams<StressDivergence>();

class StressDivergence : public Kernel
{
public:

  StressDivergence(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  MaterialProperty<SymmTensor> & _stress;
  MaterialProperty<SymmElasticityTensor> & _Jacobian_mult;
  MaterialProperty<SymmTensor> & _d_stress_dT;

private:
  const unsigned int _component;

  const bool _xdisp_coupled;
  const bool _ydisp_coupled;
  const bool _zdisp_coupled;
  const bool _temp_coupled;

  const unsigned int _xdisp_var;
  const unsigned int _ydisp_var;
  const unsigned int _zdisp_var;
  const unsigned int _temp_var;
};
#endif //STRESSDIVERGENCE_H
