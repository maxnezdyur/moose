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

class ContactApp : public MooseApp
{
public:
  static InputParameters validParams();

  ContactApp(const InputParameters & parameters);
  virtual ~ContactApp();

  static void registerApps();
  static void registerAll(Factory & f, ActionFactory & af, Syntax & s);
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
  static void registerExecFlags(Factory & /*factory*/){};
  static void registerObjectDepends(Factory & factory);
  static void associateSyntaxDepends(Syntax & syntax, ActionFactory & action_factory);
};
=======
#ifndef CONTACTAPP_H
#define CONTACTAPP_H

#include "MooseApp.h"

class ContactApp;

template<>
InputParameters validParams<ContactApp>();

class ContactApp : public MooseApp
{
public:
  ContactApp(const std::string & name, InputParameters parameters);
  virtual ~ContactApp();

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
};

#endif /* CONTACTAPP_H */
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
