<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once
//  Base class for Richards SUPG
//

#include "GeneralUserObject.h"

/**
 * base class for SUPG of the Richards equation
 * You must override all the functions below with your specific implementation
 */
class RichardsSUPG : public GeneralUserObject
{
public:
  static InputParameters validParams();

  RichardsSUPG(const InputParameters & parameters);
=======
/*****************************************/
/* Written by andrew.wilkins@csiro.au    */
/* Please contact me if you make changes */
/*****************************************/

//  Base class for Richards SUPG
//
#ifndef RICHARDSSUPG_H
#define RICHARDSSUPG_H

#include "GeneralUserObject.h"

class RichardsSUPG;


template<>
InputParameters validParams<RichardsSUPG>();

class RichardsSUPG : public GeneralUserObject
{
 public:
  RichardsSUPG(const std::string & name, InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  void initialize();
  void execute();
  void finalize();

<<<<<<< HEAD
  /**
   * SUPG velocity
   * This points in direction of information propagation.
   * @param perm permeability tensor
   * @param gradp gradient of porepressure
   * @param density fluid density
   * @param gravity gravitational acceleration vector
   */
  virtual RealVectorValue velSUPG(RealTensorValue perm,
                                  RealVectorValue gradp,
                                  Real density,
                                  RealVectorValue gravity) const = 0;

  /**
   * derivative of SUPG velocity wrt gradient of porepressure
   * @param perm permeability tensor
   */
  virtual RealTensorValue dvelSUPG_dgradp(RealTensorValue perm) const = 0;

  /**
   * derivative of SUPG velocity wrt porepressure (keeping gradp fixed)
   * @param perm permeability tensor
   * @param density_prime derivative of fluid density wrt porepressure
   * @param gravity gravitational acceleration vector
   */
  virtual RealVectorValue
  dvelSUPG_dp(RealTensorValue perm, Real density_prime, RealVectorValue gravity) const = 0;

  /**
   * |bb| ~ 2*velocity/element_length
   * @param vel SUPG velocity
   * @param dimen dimension of problem
   * @param xi_prime spatial gradient of the isoparametric coordinate xi
   * @param eta_prime spatial gradient of the isoparametric coordinate eta
   * @param zeta_prime spatial gradient of the isoparametric coordinate zeta
   */
  virtual RealVectorValue bb(RealVectorValue vel,
                             int dimen,
                             RealVectorValue xi_prime,
                             RealVectorValue eta_prime,
                             RealVectorValue zeta_prime) const = 0;

  /**
   * derivative of bb*bb wrt gradient of porepressure
   * @param vel SUPG velocity
   * @param dvel_dgradp derivative of velocity wrt gradient of porepressure
   * @param xi_prime spatial gradient of the isoparametric coordinate xi
   * @param eta_prime spatial gradient of the isoparametric coordinate eta
   * @param zeta_prime spatial gradient of the isoparametric coordinate zeta
   */
  virtual RealVectorValue dbb2_dgradp(RealVectorValue vel,
                                      RealTensorValue dvel_dgradp,
                                      RealVectorValue xi_prime,
                                      RealVectorValue eta_prime,
                                      RealVectorValue zeta_prime) const = 0;

  /**
   * derivative of bb*bb wrt porepressure
   * @param vel SUPG velocity
   * @param dvel_dp derivative of velocity wrt porepressure
   * @param xi_prime spatial gradient of the isoparametric coordinate xi
   * @param eta_prime spatial gradient of the isoparametric coordinate eta
   * @param zeta_prime spatial gradient of the isoparametric coordinate zeta
   */
  virtual Real dbb2_dp(RealVectorValue vel,
                       RealVectorValue dvel_dp,
                       RealVectorValue xi_prime,
                       RealVectorValue eta_prime,
                       RealVectorValue zeta_prime) const = 0;

  /**
   * The SUPG tau parameter.
   * This dictates how strong the SUPG is
   * @param vel SUPG velocity
   * @param traceperm trace of the permeability tensor for the material
   * @param b the b parameter: |b| ~ 2*SUPGvelocity/element_length
   */
  virtual Real tauSUPG(RealVectorValue vel, Real traceperm, RealVectorValue b) const = 0;

  /**
   * derivative of tau wrt gradient of porepressure
   * @param vel SUPG velocity
   * @param dvel_dgradp derivative of the SUPG velocity wrt gradient of porepressure
   * @param traceperm trace of the permeability tensor for the material
   * @param b the b parameter: |b| ~ 2*SUPGvelocity/element_length
   * @param db2_dgradp derivative of b*b wrt gradient of porepressure
   */
  virtual RealVectorValue dtauSUPG_dgradp(RealVectorValue vel,
                                          RealTensorValue dvel_dgradp,
                                          Real traceperm,
                                          RealVectorValue b,
                                          RealVectorValue db2_dgradp) const = 0;

  /**
   * derivative of tau wrt porepressure (keeping gradp fixed)
   * @param vel SUPG velocity
   * @param dvel_dp derivative of the SUPG velocity wrt porepressure
   * @param traceperm trace of the permeability tensor for the material
   * @param b the b parameter: |b| ~ 2*SUPGvelocity/element_length
   * @param db2_dp derivative of b*b wrt porepressure
   */
  virtual Real dtauSUPG_dp(RealVectorValue vel,
                           RealVectorValue dvel_dp,
                           Real traceperm,
                           RealVectorValue b,
                           Real db2_dp) const = 0;

  /**
   * Returns true if SUPG is trivial.
   * This may used for optimization since typically
   * SUPG stuff is quite expensive to calculate
   */
  virtual bool SUPG_trivial() const = 0;
};
=======
  // These functions must be over-ridden in the derived class
  // to provide the actual values of tauSUPG, etc
  virtual RealVectorValue velSUPG(RealTensorValue perm, RealVectorValue gradp, Real density, RealVectorValue gravity) const = 0;
  virtual RealTensorValue dvelSUPG_dgradp(RealTensorValue perm) const = 0;
  virtual RealVectorValue dvelSUPG_dp(RealTensorValue perm, Real density_prime, RealVectorValue gravity) const = 0;
  virtual RealVectorValue bb(RealVectorValue vel, int dimen, RealVectorValue xi_prime, RealVectorValue eta_prime, RealVectorValue zeta_prime) const = 0;
  virtual RealVectorValue dbb2_dgradp(RealVectorValue vel, RealTensorValue dvel_dgradp, RealVectorValue xi_prime, RealVectorValue eta_prime, RealVectorValue zeta_prime) const = 0;
  virtual Real dbb2_dp(RealVectorValue vel, RealVectorValue dvel_dp, RealVectorValue xi_prime, RealVectorValue eta_prime, RealVectorValue zeta_prime) const = 0;
  virtual Real tauSUPG(RealVectorValue vel, Real traceperm, RealVectorValue b) const = 0;
  virtual RealVectorValue dtauSUPG_dgradp(RealVectorValue vel, RealTensorValue dvel_dgradp, Real traceperm, RealVectorValue b, RealVectorValue db2_dgradp) const = 0;
  virtual Real dtauSUPG_dp(RealVectorValue vel, RealVectorValue dvel_dp, Real traceperm, RealVectorValue b, Real db2_dp) const = 0;
};

#endif // RICHARDSSUPG_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
