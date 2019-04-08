#include "class_value.h"

#include "function.h"
#include "instance_value.h"
#include "interpreter/symbol_table.h"
#include "value_visitor.h"

namespace sscript
{
const TypeInfoPtr Class::typeInfo = std::make_shared<TypeInfo>("Class");

Class::Class(const std::string& className)
    : BaseValue(std::make_shared<TypeInfo>(className)),
      m_className(className),
      m_symbolTable(std::make_shared<SymbolTable>(className))
{
}

Class::~Class() {}

std::string Class::ToString() const { return "<Class:" + m_className + ">"; }

void Class::AcceptVisitor(ValueVisitorBase* v) { v->Visit(this); }

void Class::AddMethod(const std::string& methodName, FunctionPtr& callable)
{
	m_symbolTable->Declare({methodName, callable});
}

void Class::SetCallableName(const std::string&) {}

const std::string& Class::GetCallableName() const { return m_className; }

void Class::SetArity(const std::size_t&) {}

const std::size_t& Class::GetArity() const { return getConstructor()->GetArity(); }

bool Class::IsVariadic() const { return getConstructor()->IsVariadic(); }

void Class::SetVariadic(bool) {}

void Class::SetParameterNames(const std::vector<std::string>&) {}

const std::vector<std::string>& Class::GetParameterNames() const { return getConstructor()->GetParameterNames(); }

void Class::SetClosure(const std::shared_ptr<SymbolTable>&) {}

const std::shared_ptr<SymbolTable>& Class::GetClosure() const { return getConstructor()->GetClosure(); }

void Class::SetCallableBody(ast::StatementBlockPtr) {}

void Class::Call(interpreter_visitor* v, const std::vector<BaseValuePtr>& args)
{
	auto instance = std::make_shared<Instance>(std::dynamic_pointer_cast<Class>(shared_from_this()));
	getConstructor()->Call(v, args);
	throw static_cast<BaseValuePtr>(instance);
}

CallablePtr Class::getConstructor() const
{
	if (m_constructor)
	{
		return m_constructor;
	}

	auto symbol = m_symbolTable->Get(m_className);
	m_constructor = std::dynamic_pointer_cast<Callable>(symbol.m_value);

	return m_constructor;
}
}  // namespace sscript
