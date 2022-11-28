#ifndef ELEMENTSONLINEAUX_H
#define ELEMENTSONLINEAUX_H

#include "AuxKernel.h"

class ElementsOnLineAux : public AuxKernel
{

public:

  ElementsOnLineAux(const std::string & name, InputParameters parameters);

  virtual ~ElementsOnLineAux() {}

protected:

  virtual void compute();
  virtual Real computeValue();

private:

  const Point _line1;
  const Point _line2;
  const Real _dist_tol;

  int _line_id;

};

template<>
InputParameters validParams<ElementsOnLineAux>();

#endif // ELEMENTSONLINEAUX_H
