#include "functional.h"

#include "float_value.h"

#include <iostream>

namespace sscript
{
template<>
const float& argument_value<const float&>(const BaseValuePtr& v)
{
	return value_as<Float>(v).m_value;
}

template<>
const BaseValuePtr& argument_value<const BaseValuePtr&>(const BaseValuePtr& v)
{
	return v;
}
}  // namespace sscript
