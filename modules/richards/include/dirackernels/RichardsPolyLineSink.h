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
=======
/*****************************************/
/* Written by andrew.wilkins@csiro.au    */
/* Please contact me if you make changes */
/*****************************************/

#ifndef RICHARDSPOLYLINESINK_H
#define RICHARDSPOLYLINESINK_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

#include "DiracKernel.h"
#include "LinearInterpolation.h"
#include "RichardsSumQuantity.h"
<<<<<<< HEAD
#include "RichardsVarNames.h"

// Forward Declarations

/**
 * Approximates a polyline by a sequence of Dirac Points
 * the mass flux from each Dirac Point is _sink_func as a
 * function of porepressure at the Dirac Point.
 */
class RichardsPolyLineSink : public DiracKernel
{
public:
  static InputParameters validParams();

  RichardsPolyLineSink(const InputParameters & parameters);
=======

//Forward Declarations
class RichardsPolyLineSink;

template<>
InputParameters validParams<RichardsPolyLineSink>();

class RichardsPolyLineSink : public DiracKernel
{
public:
  RichardsPolyLineSink(const std::string & name, InputParameters parameters);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  virtual void addPoints();
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

<<<<<<< HEAD
  /**
   * Computes the off-diagonal part of the jacobian
   * Note: at March2014 this is never called since
   * moose does not have this functionality.  Hence
   * as of March2014 this has never been tested.
   */
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

protected:
  /**
   * This is used to hold the total fluid flowing into the sink
   * Hence, it is positive for sinks where fluid is flowing
   * from porespace into the borehole and removed from the model
   */
  RichardsSumQuantity & _total_outflow_mass;

  /// mass flux = _sink_func as a function of porepressure
  LinearInterpolation _sink_func;

  /// contains rows of the form x y z (space separated)
  std::string _point_file;

  /// Defines the richards variables in the simulation
  const RichardsVarNames & _richards_name_UO;

  /// The moose internal variable number of the richards variable of this Dirac Kernel
  unsigned int _pvar;

  /// fluid porepressure (or porepressures in case of multiphase)
  const MaterialProperty<std::vector<Real>> & _pp;

  /// d(porepressure_i)/d(variable_j)
  const MaterialProperty<std::vector<std::vector<Real>>> & _dpp_dv;

  /// vector of Dirac Points' x positions
  std::vector<Real> _xs;

  /// vector of Dirac Points' y positions
  std::vector<Real> _ys;

  /// vector of Dirac Points' z positions
  std::vector<Real> _zs;

  /**
   * reads a space-separated line of floats from ifs and puts in myvec
   * @param ifs the file stream
   * @param myvec upon return will contain the space-separated flows encountered in ifs
   */
  bool parseNextLineReals(std::ifstream & ifs, std::vector<Real> & myvec);
};
=======

protected:
  RichardsSumQuantity & _total_outflow_mass;
  LinearInterpolation _sink_func;
  std::string _point_file;

  bool _mesh_adaptivity;

  std::vector<Real> _xs;
  std::vector<Real> _ys;
  std::vector<Real> _zs;

  std::vector<const Elem *> _elemental_info;
  bool _have_constructed_elemental_info;

  bool parseNextLineReals(std::ifstream & ifs, std::vector<Real> &myvec);
};

#endif //RICHARDSPOLYLINESINK_H
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
