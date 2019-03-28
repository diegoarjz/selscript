#include "callable.h"

#include "boolean_value.h"
#include "float_value.h"
#include "null_object_value.h"
#include "string_value.h"

#include <iostream>

namespace sscript
{
Callable::Callable() : BaseValue("Callable") {}
Callable::~Callable() {}

std::string Callable::ToString() const { return "<" + m_typeName + ">"; }
void Callable::AcceptVisitor(ValueVisitorBase *v) { v->Visit(this); }

struct visitor : ValueVisitor<std::string>
{
	template<typename T>
	std::string operator()(T &v)
	{
		return v.ToString();
	}
};

void Callable::Call(const std::vector<BaseValuePtr> &args) {}
}  // namespace sscript
