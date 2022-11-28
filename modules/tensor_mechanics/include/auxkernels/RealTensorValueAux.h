#ifndef REALTENSORVALUEAUX_H
#define REALTENSORVALUEAUX_H

#include "AuxKernel.h"

class RealTensorValueAux;

/**
 * RealTensorValueAux is designed to take the data in the RealTensorValue material
 * property, for example stress or strain, and output the value for the
 * supplied indices.
 */

template<>
InputParameters validParams<RealTensorValueAux>();

class RealTensorValueAux : public AuxKernel
{
public:
  RealTensorValueAux(const std::string & name, InputParameters parameters);

  virtual ~ RealTensorValueAux() {}

protected:
  virtual Real computeValue();

private:

  MaterialProperty<RealTensorValue> & _tensor;
  const unsigned int _i;
  const unsigned int _j;
};

#endif //REALTENSORVALUEAUX_H
