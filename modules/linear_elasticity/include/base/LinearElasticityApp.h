#ifndef LINEAR_ELASTICITYAPP_H
#define LINEAR_ELASTICITYAPP_H

#include "MooseApp.h"

class LinearElasticityApp;

template<>
InputParameters validParams<LinearElasticityApp>();

class LinearElasticityApp : public MooseApp
{
public:
  LinearElasticityApp(const std::string & name, InputParameters parameters);
  virtual ~LinearElasticityApp();

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
};

#endif /* LINEAR_ELASTICITYAPP_H */
