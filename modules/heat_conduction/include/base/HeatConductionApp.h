#ifndef HEAT_CONDUCTIONAPP_H
#define HEAT_CONDUCTIONAPP_H

#include "MooseApp.h"

class HeatConductionApp;

template<>
InputParameters validParams<HeatConductionApp>();

class HeatConductionApp : public MooseApp
{
public:
  HeatConductionApp(const std::string & name, InputParameters parameters);
  virtual ~HeatConductionApp();

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
};

#endif /* HEAT_CONDUCTIONAPP_H */
