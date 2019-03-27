#include "functional.h"

#include "float_value.h"

#include <iostream>

namespace sscript
{
template<>
const float& argument_value<const float&>(BaseValuePtr v)
{
	return value_as<Float>(v).m_value;
}
}  // namespace sscript
