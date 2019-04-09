#include "function.h"

#include "boolean_value.h"
#include "float_value.h"
#include "null_object_value.h"
#include "string_value.h"

#include "interpreter/interpreter_visitor.h"
#include "interpreter/symbol_table.h"

#include <iostream>

namespace sscript
{
const TypeInfoPtr Function::typeInfo = std::make_shared<TypeInfo>("Function");

Function::Function() : BaseValue(typeInfo), m_arity(0), m_variadic(false) {}
Function::~Function() {}

Function::Function(const Function &o)
    : BaseValue(typeInfo),
      m_closure(o.m_closure),
      m_callableBody(o.m_callableBody),
      m_identifier(o.m_identifier),
      m_parameterNames(o.m_parameterNames),
      m_arity(o.m_arity),
      m_variadic(o.m_variadic),
      m_boundInstance(o.m_boundInstance)
{
}

std::string Function::ToString() const { return "<Function:" + m_identifier + ">"; }
void Function::AcceptVisitor(ValueVisitorBase *v) { v->Visit(this); }

void Function::Call(interpreter_visitor *interpreter, const std::vector<BaseValuePtr> &args)
{
	for (std::size_t i = 0; i < GetArity(); ++i)
	{
		interpreter->GetCurrentSymbolTable()->Declare({m_parameterNames[i], args[i]});
	}

	const auto &statements = m_callableBody->GetStatements();
	for (const auto &s : statements)
	{
		s->AcceptVisitor(interpreter);
	}
}
}  // namespace sscript