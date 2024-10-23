#pragma once

#include "Material.h"
#include "MaterialProperty.h"
#include "MooseTypes.h"
#include "DerivativeMaterialInterface.h"
/**
 *
 */
class ADCoupledVarMaterialDerivative : public DerivativeMaterialInterface<Material>
{
public:
  static InputParameters validParams();

  ADCoupledVarMaterialDerivative(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// The coupled variable we are taking the derivative with respect to
  const MooseVariable & _coupled_var;

  /// AD material for the derivate dM/du
  const ADMaterialProperty<Real> & _ad_prop;

  /// Derivate Material
  MaterialProperty<Real> & _derivative_prop;
};
