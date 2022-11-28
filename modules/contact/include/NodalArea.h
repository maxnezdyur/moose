#ifndef NODALAREA_H
#define NODALAREA_H

#include "SideIntegralVariableUserObject.h"

class NodalArea : public SideIntegralVariableUserObject
{
public:
  NodalArea(const std::string & name, InputParameters parameters);
  virtual ~NodalArea();

  virtual void threadJoin(const UserObject & uo);

  virtual void initialize();
  virtual void execute();
  virtual void finalize();

  Real nodalArea( const Node * node ) const;

protected:
  virtual Real computeQpIntegral();

  std::map<const Node *, Real> _node_areas;

  std::map<unsigned, unsigned> _commMap;
  std::vector<Real> _commVec;

  const VariablePhiValue & _phi;

  SystemBase & _system;
  NumericVector<Number> & _aux_solution;
};

template<>
InputParameters validParams<NodalArea>();

#endif
