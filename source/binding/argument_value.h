#pragma once

#include "value/base_value.h"

#include <iostream>

namespace sscript
{
template<typename T>
T argument_value(const BaseValuePtr&);

template<>
bool argument_value<bool>(const BaseValuePtr&);

template<>
float argument_value<float>(const BaseValuePtr&);

template<>
int argument_value<int>(const BaseValuePtr&);

template<>
std::string argument_value<std::string>(const BaseValuePtr&);

template<>
const BaseValuePtr& argument_value<const BaseValuePtr&>(const BaseValuePtr&);

}  // namespace sscript
