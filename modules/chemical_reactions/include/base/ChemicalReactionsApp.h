#ifndef CHEMICAL_REACTIONSAPP_H
#define CHEMICAL_REACTIONSAPP_H

#include "MooseApp.h"

class ChemicalReactionsApp;

template<>
InputParameters validParams<ChemicalReactionsApp>();

class ChemicalReactionsApp : public MooseApp
{
public:
  ChemicalReactionsApp(const std::string & name, InputParameters parameters);
  virtual ~ChemicalReactionsApp();

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
};

#endif /* CHEMICAL_REACTIONSAPP_H */
