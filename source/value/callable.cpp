#include "callable.h"

#include "boolean_value.h"
#include "float_value.h"
#include "null_object_value.h"
#include "string_value.h"

#include "interpreter/interpreter_visitor.h"
#include "interpreter/symbol_table.h"

#include <iostream>

namespace sscript
{
const TypeInfo Callable::typeInfo("Callable");

Callable::Callable() : BaseValue(typeInfo.Name()), m_arity(0), m_variadic(false) {}
Callable::~Callable() {}

std::string Callable::ToString() const { return "<" + m_typeName + ":" + m_identifier + ">"; }
void Callable::AcceptVisitor(ValueVisitorBase *v) { v->Visit(this); }

void Callable::Call(interpreter_visitor *interpreter, const std::vector<BaseValuePtr> &args)
{
	for (std::size_t i = 0; i < GetArity(); ++i)
	{
		interpreter->GetCurrentSymbolTable()->Declare({m_parameterNames[i], args[i]});
	}

	m_callableBody->AcceptVisitor(interpreter);
}
}  // namespace sscript
