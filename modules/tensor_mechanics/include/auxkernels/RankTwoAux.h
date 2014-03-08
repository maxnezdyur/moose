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

#include "NodalPatchRecovery.h"
#include "RankTwoTensor.h"

=======
#ifndef RANKTWOAUX_H
#define RANKTWOAUX_H

#include "AuxKernel.h"
#include "RankTwoTensor.h"

class RankTwoAux;

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
/**
 * RankTwoAux is designed to take the data in the RankTwoTensor material
 * property, for example stress or strain, and output the value for the
 * supplied indices.
 */

<<<<<<< HEAD
template <bool is_ad>
class RankTwoAuxTempl : public NodalPatchRecovery
{
public:
  static InputParameters validParams();

  RankTwoAuxTempl(const InputParameters & parameters);
=======
template<>
InputParameters validParams<RankTwoAux>();

class RankTwoAux : public AuxKernel
{
public:
  RankTwoAux(const std::string & name, InputParameters parameters);

  virtual ~ RankTwoAux() {}
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual Real computeValue();

private:
<<<<<<< HEAD
  const GenericMaterialProperty<RankTwoTensor, is_ad> & _tensor;
  const unsigned int _i;
  const unsigned int _j;

  /// whether or not selected_qp has been set
  const bool _has_selected_qp;

  /// The std::vector will be evaluated at this quadpoint only if defined
  const unsigned int _selected_qp;
};

typedef RankTwoAuxTempl<false> RankTwoAux;
typedef RankTwoAuxTempl<true> ADRankTwoAux;
=======

  MaterialProperty<RankTwoTensor> & _tensor;
  const int _i;
  const int _j;
};

#endif //RANKTWOAUX_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
