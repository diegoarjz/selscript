#include "argument_value.h"

#include "value/boolean_value.h"
#include "value/float_value.h"
#include "value/integer_value.h"
#include "value/string_value.h"
#include "value/value_visitor.h"

namespace sscript
{
template<>
bool argument_value<bool>(const BaseValuePtr& v)
{
	return value_as<Boolean>(v).m_value;
}

template<>
float argument_value<float>(const BaseValuePtr& v)
{
	return value_as<Float>(v).m_value;
}

template<>
int argument_value<int>(const BaseValuePtr& v)
{
	return value_as<Integer>(v).m_value;
}

template<>
std::string argument_value<std::string>(const BaseValuePtr& v)
{
	return value_as<String>(v).m_value;
}

template<>
const BaseValuePtr& argument_value<const BaseValuePtr&>(const BaseValuePtr& v)
{
	std::cout << "From BaseValue" << std::endl;
	return v;
}
}  // namespace sscript
