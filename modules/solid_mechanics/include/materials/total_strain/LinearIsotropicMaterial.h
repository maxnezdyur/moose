#ifndef LINEARISOTROPICMATERIAL_H
#define LINEARISOTROPICMATERIAL_H

#include "SolidMechanicsMaterial.h"

//Forward Declarations
class LinearIsotropicMaterial;
class SymmElasticityTensor;

template<>
InputParameters validParams<LinearIsotropicMaterial>();

/**
 * LinearIsotropic material for use in simple applications that don't need material properties.
 */
class LinearIsotropicMaterial : public SolidMechanicsMaterial
{
public:
  LinearIsotropicMaterial(const std::string & name,
                          InputParameters parameters);

  virtual ~LinearIsotropicMaterial();

protected:
  virtual void computeProperties();

  virtual void computeStress(const SymmTensor & strain,
                             SymmTensor & stress);

  /**
   * Will always be passed to full symmetric strain tensor.
   * What should come out is a modified strain tensor.
   */
  virtual void computeStrain(const SymmTensor & total_strain, SymmTensor & elastic_strain);

  virtual Real computeAlpha();

  Real _youngs_modulus;
  Real _poissons_ratio;

  Real _t_ref;
  Real _alpha;

  SymmElasticityTensor * _local_elasticity_tensor;

  Real _pi;
  Real _tol;

};

#endif //LINEARISOTROPICMATERIAL_H
