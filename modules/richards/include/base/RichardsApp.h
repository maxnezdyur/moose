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

#include "MooseApp.h"

/**
 * The Richards equation is a nonlinear diffusion
 * equation that models multiphase flow through
 * porous materials
 */
class RichardsApp : public MooseApp
{
public:
  static InputParameters validParams();

  RichardsApp(const InputParameters & parameters);
  virtual ~RichardsApp();

  static void registerApps();
  static void registerAll(Factory & f, ActionFactory & af, Syntax & s);
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
  static void registerExecFlags(Factory & factory);
};
=======
#ifndef RICHARDSAPP_H
#define RICHARDSAPP_H

#include "MooseApp.h"

class RichardsApp;

template<>
InputParameters validParams<RichardsApp>();

class RichardsApp : public MooseApp
{
public:
  RichardsApp(const std::string & name, InputParameters parameters);
  virtual ~RichardsApp();

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
};

#endif /* RICHARDSAPP_H */
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
