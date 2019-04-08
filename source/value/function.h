#pragma once

#include "base_value.h"
#include "callable.h"
#include "float_value.h"
#include "functional.h"
#include "type_info.h"
#include "undefined_operator.h"

#include <vector>

namespace sscript
{
namespace ast
{
struct StatementBlock;
using StatementBlockPtr = std::shared_ptr<StatementBlock>;
}  // namespace ast

struct interpreter_visitor;
class SymbolTable;

struct Function : public BaseValue, public Callable
{
	static const TypeInfoPtr typeInfo;

	Function();
	virtual ~Function();

	std::string ToString() const override;
	void AcceptVisitor(ValueVisitorBase*) override;
	void Call(interpreter_visitor* interpreter, const std::vector<BaseValuePtr>& args) override;

	void SetCallableName(const std::string& n) override { m_identifier = n; }
	const std::string& GetCallableName() const override { return m_identifier; }

	void SetArity(const std::size_t& arity) override { m_arity = arity; }
	const std::size_t& GetArity() const override { return m_arity; }
	bool IsVariadic() const override { return m_variadic; }
	void SetVariadic(bool variadic) override { m_variadic = variadic; }

	void SetParameterNames(const std::vector<std::string>& parameters) override { m_parameterNames = parameters; }
	const std::vector<std::string>& GetParameterNames() const override { return m_parameterNames; }
	void SetCallableBody(ast::StatementBlockPtr body) override { m_callableBody = body; }
	void SetClosure(const std::shared_ptr<SymbolTable>& closure) override { m_closure = closure; }
	const std::shared_ptr<SymbolTable>& GetClosure() const override { return m_closure; }

	template<class T>
	std::shared_ptr<BaseValue> operator+(const T& o) const
	{
		throw UndefinedBinaryOperatorException("+", typeInfo, T::typeInfo);
	}

	template<class T>
	std::shared_ptr<BaseValue> operator-(const T& o) const
	{
		throw UndefinedBinaryOperatorException("-", typeInfo, T::typeInfo);
	}

	template<class T>
	std::shared_ptr<BaseValue> operator*(const T& o) const
	{
		throw UndefinedBinaryOperatorException("/", typeInfo, T::typeInfo);
	}

	template<class T>
	std::shared_ptr<BaseValue> operator/(const T& o) const
	{
		throw UndefinedBinaryOperatorException("/", typeInfo, T::typeInfo);
	}

	template<class T>
	std::shared_ptr<BaseValue> operator==(const T& o) const
	{
		throw UndefinedBinaryOperatorException("==", typeInfo, T::typeInfo);
	}

	template<class T>
	std::shared_ptr<BaseValue> operator!=(const T& o) const
	{
		throw UndefinedBinaryOperatorException("!=", typeInfo, T::typeInfo);
	}

	template<class T>
	std::shared_ptr<BaseValue> operator<(const T& o) const
	{
		throw UndefinedBinaryOperatorException("<", typeInfo, T::typeInfo);
	}

	template<class T>
	std::shared_ptr<BaseValue> operator<=(const T& o) const
	{
		throw UndefinedBinaryOperatorException("<=", typeInfo, T::typeInfo);
	}

	template<class T>
	std::shared_ptr<BaseValue> operator>(const T& o) const
	{
		throw UndefinedBinaryOperatorException(">", typeInfo, T::typeInfo);
	}

	template<class T>
	std::shared_ptr<BaseValue> operator>=(const T& o) const
	{
		throw UndefinedBinaryOperatorException(">=", typeInfo, T::typeInfo);
	}

	bool operator!() const { throw UndefinedUnaryOperatorException("!", typeInfo); }
	explicit operator bool() const { throw UndefinedUnaryOperatorException("conversion to bool", typeInfo); }

	std::shared_ptr<BaseValue> operator-() const { throw UndefinedUnaryOperatorException("-", typeInfo); }

private:
	std::shared_ptr<SymbolTable> m_closure;
	ast::StatementBlockPtr m_callableBody;
	std::string m_identifier;
	std::vector<std::string> m_parameterNames;
	std::size_t m_arity;
	bool m_variadic;
};
using FunctionPtr = std::shared_ptr<Function>;

}  // namespace sscript
