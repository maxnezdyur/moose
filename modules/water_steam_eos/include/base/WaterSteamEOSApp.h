#ifndef WATER_STEAM_EOSAPP_H
#define WATER_STEAM_EOSAPP_H

#include "MooseApp.h"

class WaterSteamEOSApp;

template<>
InputParameters validParams<WaterSteamEOSApp>();

class WaterSteamEOSApp : public MooseApp
{
public:
  WaterSteamEOSApp(const std::string & name, InputParameters parameters);
  virtual ~WaterSteamEOSApp();

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
};

#endif /* WATER_STEAM_EOSAPP_H */
