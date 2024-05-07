//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#ifdef LIBTORCH_ENABLED

#pragma once

#include "LibtorchTorchScriptNeuralNet.h"
#include <torch/torch.h>
#include "AuxKernel.h"

class LibtorchScriptNeuralNetAux : public AuxKernel
{
public:
  static InputParameters validParams();

  LibtorchScriptNeuralNetAux(const InputParameters & parameters);

protected:
  virtual Real computeValue() override;

  /// Pointer to the neural net script object
  const std::shared_ptr<Moose::LibtorchTorchScriptNeuralNet> _nn;

  /// Names of the additional input parameters for the neural network model
  const std::vector<std::string> _parameter_names;

  /// Values of the additional input parameters for the neural network mode
  const std::vector<Real> _parameter_values;

  /// Name of the output variable to return for result
  const std::string _output_name;
  /// Whether to use double precision for computations
  const bool _use_double;

private:
  /**
   * Helper function template to compute the value using the specified value type and tensor type
   * @tparam ValueType The scalar value type (double or float)
   * @param TensorType The corresponding torch::ScalarType (torch::kDouble or torch::kFloat)
   */
  template <typename ValueType>
  Real computeValueImpl(torch::ScalarType TensorType);
};

#endif
