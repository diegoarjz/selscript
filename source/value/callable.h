#pragma once

#include "base_value.h"
#include "float_value.h"
#include "functional.h"

#include <vector>

namespace sscript
{
namespace ast
{
struct StatementBlock;
using StatementBlockPtr = std::shared_ptr<StatementBlock>;
}  // namespace ast

struct interpreter_visitor;

struct Callable : public BaseValue
{
	Callable();
	virtual ~Callable();

	std::string ToString() const override;
	void AcceptVisitor(ValueVisitorBase*) override;

	virtual void Call(interpreter_visitor* interpreter, const std::vector<BaseValuePtr>& args);

	void SetCallableName(const std::string& n) { m_identifier = n; }
	const std::string& GetCallableName() const { return m_identifier; }

	void SetArity(const std::size_t& arity) { m_arity = arity; }
	std::size_t GetArity() const { return m_arity; }
	bool IsVariadic() const { return m_variadic; }
	void SetVariadic(bool variadic) { m_variadic = variadic; }

	void SetParameterNames(const std::vector<std::string>& parameters) { m_parameterNames = parameters; }
	const std::vector<std::string>& GetParameterNames() const { return m_parameterNames; }
	void SetCallableBody(ast::StatementBlockPtr body) { m_callableBody = body; }

private:
	ast::StatementBlockPtr m_callableBody;
	std::string m_identifier;
	std::vector<std::string> m_parameterNames;
	std::size_t m_arity;
	bool m_variadic;
};
using CallablePtr = std::shared_ptr<Callable>;

}  // namespace sscript
