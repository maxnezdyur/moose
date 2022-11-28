#ifndef SOLID_MECHANICSAPP_H
#define SOLID_MECHANICSAPP_H

#include "MooseApp.h"

class SolidMechanicsApp;

template<>
InputParameters validParams<SolidMechanicsApp>();

class SolidMechanicsApp : public MooseApp
{
public:
  SolidMechanicsApp(const std::string & name, InputParameters parameters);
  virtual ~SolidMechanicsApp();

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
};

#endif /* SOLID_MECHANICSAPP_H */
