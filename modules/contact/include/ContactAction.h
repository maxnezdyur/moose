#ifndef CONTACTACTION_H
#define CONTACTACTION_H

#include "Action.h"
#include "MooseTypes.h"
#include "MooseEnum.h"

class ContactAction;

template<>
InputParameters validParams<ContactAction>();

class ContactAction: public Action
{
public:
  ContactAction(const std::string & name, InputParameters params);

  virtual void act();

private:
  const BoundaryName _master;
  const BoundaryName _slave;
  const NonlinearVariableName _disp_x;
  const NonlinearVariableName _disp_y;
  const NonlinearVariableName _disp_z;
  const Real _penalty;
  const Real _friction_coefficient;
  const Real _tension_release;
  const std::string _model;
  const std::string _formulation;
  const MooseEnum _order;
};


#endif // CONTACTACTION_H
