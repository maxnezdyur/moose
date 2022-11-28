#ifndef TENSOR_MECHANICSAPP_H
#define TENSOR_MECHANICSAPP_H

#include "MooseApp.h"

class TensorMechanicsApp;

template<>
InputParameters validParams<TensorMechanicsApp>();

class TensorMechanicsApp : public MooseApp
{
public:
  TensorMechanicsApp(const std::string & name, InputParameters parameters);
  virtual ~TensorMechanicsApp();

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
};

#endif /* TENSOR_MECHANICSAPP_H */
