#ifndef NAVIER_STOKESAPP_H
#define NAVIER_STOKESAPP_H

#include "MooseApp.h"

class NavierStokesApp;

template<>
InputParameters validParams<NavierStokesApp>();

class NavierStokesApp : public MooseApp
{
public:
  NavierStokesApp(const std::string & name, InputParameters parameters);
  virtual ~NavierStokesApp();

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
};

#endif /* NAVIER_STOKESAPP_H */
