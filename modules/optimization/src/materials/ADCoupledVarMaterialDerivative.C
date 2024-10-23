#include "ADCoupledVarMaterialDerivative.h"
#include "MaterialProperty.h"
#include "MooseTypes.h"
#include "libmesh/int_range.h"
#include "libmesh/libmesh_common.h"

registerMooseObject("OptimizationApp", ADCoupledVarMaterialDerivative);

InputParameters
ADCoupledVarMaterialDerivative::validParams()
{
  InputParameters params = DerivativeMaterialInterface<Material>::validParams();
  params.addClassDescription("");

  params.addRequiredCoupledVar("coupled_var", "Variable we are taking derivate with respect to.");
  params.addRequiredParam<MaterialPropertyName>("ad_prop_name", "Name of the ad material.");
  params.addRequiredParam<MaterialPropertyName>("derivative_prop_name",
                                                "Name if the derivate material to create.");
  return params;
}

ADCoupledVarMaterialDerivative::ADCoupledVarMaterialDerivative(const InputParameters & parameters)
  : DerivativeMaterialInterface<Material>(parameters),
    _coupled_var(*getVar("coupled_var", 0)),
    _ad_prop(getADMaterialProperty<Real>(getParam<MaterialPropertyName>("ad_prop_name"))),
    _derivative_prop(
        declarePropertyByName<Real>(getParam<MaterialPropertyName>("derivative_prop_name")))
{
}

void
ADCoupledVarMaterialDerivative::computeQpProperties()
{
  const auto dofs = _coupled_var.dofIndices();

  const auto & phis = _coupled_var.phi();

  auto mat_derivatives = _ad_prop[_qp].derivatives();

  _derivative_prop[_qp] = 0;

  for (const auto & dof : dofs)
  {
    for (const auto & phi : phis[_qp])
      _derivative_prop[_qp] += phi * mat_derivatives[dof];
  }
}
