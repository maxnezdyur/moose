#ifndef MATERIALVECTORAUX_H
#define MATERIALVECTORAUX_H

#include "AuxKernel.h"

class MaterialVectorAux;

template<>
InputParameters validParams<MaterialVectorAux>();


class MaterialVectorAux : public AuxKernel
{
public:
  MaterialVectorAux( const std::string & name, InputParameters parameters );

  virtual ~MaterialVectorAux() {}

protected:
  enum MVA_ENUM
  {
    MVA_COMPONENT,
    MVA_LENGTH
  };


  virtual Real computeValue();

  MaterialProperty<RealVectorValue> & _vector;
  const int _index;
  MooseEnum _quantity_moose_enum;
  MVA_ENUM _quantity;

};

#endif // MATERIALVECTORAUX_H
