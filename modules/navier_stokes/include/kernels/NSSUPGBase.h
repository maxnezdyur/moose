#ifndef NSSUPGBASE_H
#define NSSUPGBASE_H

#include "NSKernel.h"

// Forward Declarations
class NSSUPGBase;

template<>
InputParameters validParams<NSSUPGBase>();


/**
 * This class acts as a base class for stabilization kernels.
 * This is useful because the stabilization kernels for different
 * equations share a lot of information...
 */
class NSSUPGBase : public NSKernel
{
public:

  NSSUPGBase(const std::string & name, InputParameters parameters);

protected:
  /**
   * This kernel is not actually called at quadrature points:
   * derived classes must implement these functions
   */
  //virtual Real computeQpResidual();
  //virtual Real computeQpJacobian();
  //virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  // Material properties
  MaterialProperty<RealTensorValue>& _viscous_stress_tensor;
  MaterialProperty<Real>& _dynamic_viscosity;
  MaterialProperty<Real>& _thermal_conductivity;

  // SUPG-related material properties.
  MaterialProperty<Real> & _hsupg;
  MaterialProperty<Real> & _tauc;
  MaterialProperty<Real> & _taum;
  MaterialProperty<Real> & _taue;
  MaterialProperty<std::vector<Real> > & _strong_residuals;

  // Momentum equation inviscid flux matrices
  MaterialProperty<std::vector<RealTensorValue> >& _calA;

  // "velocity column" matrices
  MaterialProperty<std::vector<RealTensorValue> >& _calC;

  // Energy equation inviscid flux matrices
  MaterialProperty<std::vector<std::vector<RealTensorValue> > >& _calE;

  // "Old" (from previous timestep) coupled variable values.
//  VariableValue& _rho_old;
//  VariableValue& _rho_u_old;
//  VariableValue& _rho_v_old;
//  VariableValue& _rho_w_old;
//  VariableValue& _rho_e_old;

  // The derivative of "udot" wrt u for each of the momentum variables.
  // This is always 1/dt unless you are using BDF2...
  VariableValue& _d_rhodot_du;
  VariableValue& _d_rhoudot_du;
  VariableValue& _d_rhovdot_du;
  VariableValue& _d_rhowdot_du;
  VariableValue& _d_rhoedot_du;

  // Temperature is need to compute speed of sound
  VariableValue & _temperature;

  // Enthalpy aux variable
  VariableValue& _enthalpy;
};

#endif //  NSSUPGBASE_H
