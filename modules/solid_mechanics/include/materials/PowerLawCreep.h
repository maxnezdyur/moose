#ifndef POWERLAWCREEP_H
#define POWERLAWCREEP_H

#include "SolidModel.h"

// Forward declarations
class PowerLawCreep;

template<>
InputParameters validParams<PowerLawCreep>();

/**
 * Power-law creep material
 * edot = A(sigma)**n * exp(-Q/(RT))
 */

class PowerLawCreep : public SolidModel
{
public:
  PowerLawCreep( const std::string & name,
                 InputParameters parameters );

protected:

};

#endif //POWERLAWCREEPMATERIAL_H
