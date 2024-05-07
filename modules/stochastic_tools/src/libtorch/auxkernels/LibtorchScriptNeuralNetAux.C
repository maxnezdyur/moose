//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifdef LIBTORCH_ENABLED

#include "LibtorchScriptNeuralNetAux.h"
registerMooseObject("StochasticToolsApp", LibtorchScriptNeuralNetAux);

InputParameters
LibtorchScriptNeuralNetAux::validParams()
{
  InputParameters params = AuxKernel::validParams();
  params.addClassDescription("AuxKernel that evaluates a torch script artificial neural net.");

  params.addRequiredParam<std::string>("filename",
                                       "The name of the file where the torch script is saved.");
  params.addRequiredParam<std::string>("output_name",
                                       "Name of the output from the neural network.");
  params.addParam<std::vector<std::string>>("parameter_names",
                                            "Names of the additional parameters.");
  params.addParam<std::vector<Real>>("parameter_values", "Values of the additional parameters.");
  params.addParam<bool>("use_double", false, "Use double type for input.");
  return params;
}

LibtorchScriptNeuralNetAux::LibtorchScriptNeuralNetAux(const InputParameters & parameters)
  : AuxKernel(parameters),
    _nn(std::make_shared<Moose::LibtorchTorchScriptNeuralNet>(getParam<std::string>("filename"))),
    _parameter_names(isParamValid("parameter_names")
                         ? getParam<std::vector<std::string>>("parameter_names")
                         : std::vector<std::string>{}),
    _parameter_values(isParamValid("parameter_values")
                          ? getParam<std::vector<Real>>("parameter_values")
                          : std::vector<Real>{}),
    _output_name(getParam<std::string>("output_name")),
    _use_double(getParam<bool>("use_double"))
{
  if (_parameter_names.size() != _parameter_values.size())
    paramError("parameter_names",
               "The number of parameter_values and the number of parameters_names needs to match.");
}

template <typename ValueType>
Real
LibtorchScriptNeuralNetAux::computeValueImpl(torch::ScalarType TensorType)
{
  // Create a dictionary to hold the input data
  torch::Dict<std::string, torch::Tensor> input_dict;

  // Get the current node coordinates and time
  auto x = (*_current_node)(0);

  torch::Tensor x_tensor = torch::tensor({static_cast<ValueType>(x)}, TensorType);
  input_dict.insert("x", x_tensor);

  if (_mesh.dimension() > 1)
  {
    auto y = (*_current_node)(1);
    torch::Tensor y_tensor = torch::tensor({static_cast<ValueType>(y)}, TensorType);
    input_dict.insert("y", y_tensor);
  }

  if (_mesh.dimension() > 2)
  {
    auto z = (*_current_node)(2);
    torch::Tensor z_tensor = torch::tensor({static_cast<ValueType>(z)}, TensorType);
    input_dict.insert("z", z_tensor);
  }

  if (_is_transient)
  {
    auto t = _t;
    torch::Tensor t_tensor = torch::tensor({static_cast<ValueType>(t)}, TensorType);
    input_dict.insert("t", t_tensor);
  }

  for (auto param_index : make_range(_parameter_names.size()))
  {
    auto val = _parameter_values[param_index];
    torch::Tensor val_tensor = torch::tensor({static_cast<ValueType>(val)}, TensorType);
    input_dict.insert(_parameter_names[param_index], val_tensor);
  }

  // Dictionary converted to general inputs
  std::vector<torch::jit::IValue> inputs;
  inputs.push_back(input_dict);

  // Compute result
  return _nn->forward(inputs).toGenericDict().at(_output_name).toTensor().item<ValueType>();
}

Real
LibtorchScriptNeuralNetAux::computeValue()
{
  if (_use_double)
    return computeValueImpl<double>(torch::kDouble);
  else
    return computeValueImpl<float>(torch::kFloat);
}

#endif
