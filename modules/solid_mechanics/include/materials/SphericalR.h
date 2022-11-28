#ifndef SPHERICALR_H
#define SPHERICALR_H

#include "Element.h"

//Forward Declarations
class SymmElasticityTensor;
namespace Elk
{
namespace SolidMechanics
{

class SphericalR : public Element
{
public:
  SphericalR(const std::string & name, InputParameters parameters);
  virtual ~SphericalR();

protected:

  virtual void computeStrain( const unsigned qp,
                              const SymmTensor & total_strain_old,
                              SymmTensor & total_strain_new,
                              SymmTensor & strain_increment );

  virtual unsigned int getNumKnownCrackDirs() const
  {
    return 2;
  }

  VariableValue & _disp_r;

  const bool _large_strain;

  VariableGradient & _grad_disp_r;

};

}
}

#endif
