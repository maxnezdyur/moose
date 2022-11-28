#ifndef ADDCOUPLEDSOLIDKINSPECIESKERNELSACTION_H
#define ADDCOUPLEDSOLIDKINSPECIESKERNELSACTION_H

#include "Action.h"

class AddCoupledSolidKinSpeciesKernelsAction;

template<>
InputParameters validParams<AddCoupledSolidKinSpeciesKernelsAction>();


class AddCoupledSolidKinSpeciesKernelsAction : public Action
{
public:
  AddCoupledSolidKinSpeciesKernelsAction(const std::string & name, InputParameters params);

  virtual void act();

};

#endif // ADDCOUPLEDSOLIDKINSPECIESKERNELSACTION_H
