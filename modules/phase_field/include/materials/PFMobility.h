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

#include "Material.h"

// Forward Declarations
=======
#ifndef PFMobility_H
#define PFMobility_H

#include "Material.h"

//Forward Declarations
class PFMobility;

template<>
InputParameters validParams<PFMobility>();
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

class PFMobility : public Material
{
public:
<<<<<<< HEAD
  static InputParameters validParams();

  PFMobility(const InputParameters & parameters);
=======
  PFMobility(const std::string & name,
          InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

protected:
  virtual void computeProperties();

private:
<<<<<<< HEAD
=======

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  MaterialProperty<Real> & _M;
  MaterialProperty<RealGradient> & _grad_M;
  MaterialProperty<Real> & _kappa_c;

  Real _mob;
  Real _kappa;
<<<<<<< HEAD
};
=======


};

#endif //PFMobility_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
