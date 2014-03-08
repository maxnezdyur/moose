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

#include "AuxKernel.h"
#include "RankFourTensor.h"

/**
 * RankFourAux is designed to take the data in the RankFourTensor material
 * property, for example stiffness, and output the value for the
 * supplied indices.
 */
template <bool is_ad>
class RankFourAuxTempl : public AuxKernel
{
public:
  static InputParameters validParams();

  RankFourAuxTempl(const InputParameters & parameters);

  virtual ~RankFourAuxTempl() {}
=======
#ifndef RANKFOURAUX_H
#define RANKFOURAUX_H

#include "AuxKernel.h"
#include "ElasticityTensorR4.h"

class RankFourAux;

/**
 * RankFourAux is designed to take the data in the ElasticityTensorR4 material
 * property, for example stiffness, and output the value for the
 * supplied indices.
 */

template<>
InputParameters validParams<RankFourAux>();

class RankFourAux : public AuxKernel
{
public:
  RankFourAux(const std::string & name, InputParameters parameters);

  virtual ~ RankFourAux() {}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeValue();

<<<<<<< HEAD
private:
  const GenericMaterialProperty<RankFourTensor, is_ad> & _tensor;
  const unsigned int _i;
  const unsigned int _j;
  const unsigned int _k;
  const unsigned int _l;
};

typedef RankFourAuxTempl<false> RankFourAux;
typedef RankFourAuxTempl<true> ADRankFourAux;
=======
private:  MaterialProperty<ElasticityTensorR4> & _tensor;
  const int _i;
  const int _j;
  const int _k;
  const int _l;
};

#endif //RANKFOURAUX_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
