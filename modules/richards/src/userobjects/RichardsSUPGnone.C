<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
=======
/*****************************************/
/* Written by andrew.wilkins@csiro.au    */
/* Please contact me if you make changes */
/*****************************************/
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

//  User object to implement no SUPG
//
#include "RichardsSUPGnone.h"

<<<<<<< HEAD
registerMooseObject("RichardsApp", RichardsSUPGnone);

InputParameters
RichardsSUPGnone::validParams()
{
  InputParameters params = RichardsSUPG::validParams();
=======
template<>
InputParameters validParams<RichardsSUPGnone>()
{
  InputParameters params = validParams<RichardsSUPG>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  params.addClassDescription("User object for no SUPG");
  return params;
}

<<<<<<< HEAD
RichardsSUPGnone::RichardsSUPGnone(const InputParameters & parameters) : RichardsSUPG(parameters) {}

RealVectorValue RichardsSUPGnone::velSUPG(RealTensorValue /*perm*/,
                                          RealVectorValue /*gradp*/,
                                          Real /*density*/,
                                          RealVectorValue /*gravity*/) const
=======
RichardsSUPGnone::RichardsSUPGnone(const std::string & name, InputParameters parameters) :
  RichardsSUPG(name, parameters)
{}

RealVectorValue
RichardsSUPGnone::velSUPG(RealTensorValue /*perm*/, RealVectorValue /*gradp*/, Real /*density*/, RealVectorValue /*gravity*/) const
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
  return RealVectorValue();
}

<<<<<<< HEAD
RealTensorValue RichardsSUPGnone::dvelSUPG_dgradp(RealTensorValue /*perm*/) const
=======
RealTensorValue
RichardsSUPGnone::dvelSUPG_dgradp(RealTensorValue /*perm*/) const
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
  return RealTensorValue();
}

<<<<<<< HEAD
RealVectorValue RichardsSUPGnone::dvelSUPG_dp(RealTensorValue /*perm*/,
                                              Real /*density_prime*/,
                                              RealVectorValue /*gravity*/) const
=======
RealVectorValue
RichardsSUPGnone::dvelSUPG_dp(RealTensorValue /*perm*/, Real /*density_prime*/, RealVectorValue /*gravity*/) const
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
  return RealVectorValue();
}

<<<<<<< HEAD
RealVectorValue
RichardsSUPGnone::bb(RealVectorValue /*vel*/,
                     int /*dimen*/,
                     RealVectorValue /*xi_prime*/,
                     RealVectorValue /*eta_prime*/,
                     RealVectorValue /*zeta_prime*/) const
=======

RealVectorValue
RichardsSUPGnone::bb(RealVectorValue /*vel*/, int /*dimen*/, RealVectorValue /*xi_prime*/, RealVectorValue /*eta_prime*/, RealVectorValue /*zeta_prime*/) const
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
  return RealVectorValue();
}

// following is d(bb*bb)/d(gradp)
<<<<<<< HEAD
RealVectorValue RichardsSUPGnone::dbb2_dgradp(RealVectorValue /*vel*/,
                                              RealTensorValue /*dvel_dgradp*/,
                                              RealVectorValue /*xi_prime*/,
                                              RealVectorValue /*eta_prime*/,
                                              RealVectorValue /*zeta_prime*/) const
=======
RealVectorValue
RichardsSUPGnone::dbb2_dgradp(RealVectorValue /*vel*/, RealTensorValue /*dvel_dgradp*/, RealVectorValue /*xi_prime*/, RealVectorValue /*eta_prime*/, RealVectorValue /*zeta_prime*/) const
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
  return RealVectorValue();
}

// following is d(bb*bb)/d(p)
<<<<<<< HEAD
Real RichardsSUPGnone::dbb2_dp(RealVectorValue /*vel*/,
                               RealVectorValue /*dvel_dp*/,
                               RealVectorValue /*xi_prime*/,
                               RealVectorValue /*eta_prime*/,
                               RealVectorValue /*zeta_prime*/) const
=======
Real
RichardsSUPGnone::dbb2_dp(RealVectorValue /*vel*/, RealVectorValue /*dvel_dp*/, RealVectorValue /*xi_prime*/, RealVectorValue /*eta_prime*/, RealVectorValue /*zeta_prime*/) const
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
  return 0;
}

<<<<<<< HEAD
Real RichardsSUPGnone::tauSUPG(RealVectorValue /*vel*/,
                               Real /*traceperm*/,
                               RealVectorValue /*b*/) const
=======

Real
RichardsSUPGnone::tauSUPG(RealVectorValue /*vel*/, Real /*traceperm*/, RealVectorValue /*b*/) const
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
  return 0;
}

<<<<<<< HEAD
RealVectorValue RichardsSUPGnone::dtauSUPG_dgradp(RealVectorValue /*vel*/,
                                                  RealTensorValue /*dvel_dgradp*/,
                                                  Real /*traceperm*/,
                                                  RealVectorValue /*b*/,
                                                  RealVectorValue /*db2_dgradp*/) const
=======

RealVectorValue
RichardsSUPGnone::dtauSUPG_dgradp(RealVectorValue /*vel*/, RealTensorValue /*dvel_dgradp*/, Real /*traceperm*/, RealVectorValue /*b*/, RealVectorValue /*db2_dgradp*/) const
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
  return RealVectorValue();
}

<<<<<<< HEAD
Real RichardsSUPGnone::dtauSUPG_dp(RealVectorValue /*vel*/,
                                   RealVectorValue /*dvel_dp*/,
                                   Real /*traceperm*/,
                                   RealVectorValue /*b*/,
                                   Real /*db2_dp*/) const
{
  return 0;
}

bool
RichardsSUPGnone::SUPG_trivial() const
{
  return true;
}
=======
Real
RichardsSUPGnone::dtauSUPG_dp(RealVectorValue /*vel*/, RealVectorValue /*dvel_dp*/, Real /*traceperm*/, RealVectorValue /*b*/, Real /*db2_dp*/) const
{
  return 0;
}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
