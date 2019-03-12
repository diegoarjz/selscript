#pragma once

#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include <string>
#include <vector>

#include <iostream>

namespace sscript
{
namespace ast
{
struct identifier
{
	std::string m_identifier;

	identifier() = default;
	identifier(const std::string &name) : m_identifier(name) {}
	identifier(const std::vector<char> &name) : m_identifier(name.begin(), name.end()) {}
};

struct number
{
	float m_number;

	number() = default;
	number(const float &f) : m_number(f) {}
};

struct string
{
	std::string m_string;

	string() = default;
	string(const std::string &name) : m_string(name) {}
	string(const std::vector<char> &name) : m_string(name.begin(), name.end()) {}
};

struct null
{
};

struct arithmetic_op;
struct unary;
struct comparison_op;
struct logic_op;
struct assignment;

using expression = boost::variant<number, string, identifier, bool, null, boost::recursive_wrapper<arithmetic_op>,
                                  boost::recursive_wrapper<unary>, boost::recursive_wrapper<comparison_op>,
                                  boost::recursive_wrapper<logic_op>, boost::recursive_wrapper<assignment>>;

struct unary
{
	enum class types
	{
		Neg,
		Min
	};

	unary() = default;
	unary(const types &op, const expression &rhs) : m_operation(op), m_operand(rhs) {}

	types m_operation;
	expression m_operand;
};

struct arithmetic_op
{
	enum class types
	{
		Add,
		Sub,
		Mul,
		Div
	};

	arithmetic_op() = default;
	arithmetic_op(const types op, const expression &lhs, const expression &rhs)
	    : m_operation(op), m_lhs(lhs), m_rhs(rhs)
	{
	}

	types m_operation;
	expression m_lhs;
	expression m_rhs;
};

struct comparison_op
{
	enum class types
	{
		Eq,
		Ne,
		Gt,
		Gte,
		Lt,
		Lte
	};

	comparison_op() = default;
	comparison_op(const types &op, const expression &lhs, const expression &rhs)
	    : m_operation(op), m_lhs(lhs), m_rhs(rhs)
	{
	}

	types m_operation;
	expression m_lhs;
	expression m_rhs;
};

struct logic_op
{
	enum class types
	{
		And,
		Or
	};

	logic_op() = default;
	logic_op(const types &op, const expression &lhs, const expression &rhs) : m_operation(op), m_lhs(lhs), m_rhs(rhs) {}

	types m_operation;
	expression m_lhs;
	expression m_rhs;
};

struct assignment
{
	identifier m_variableName;
	expression m_rhs;
};

using statement_types = boost::variant<expression>;
struct statement
{
	statement_types m_statement;
};

struct var_decl
{
	identifier m_variableName;
	boost::optional<expression> m_rhs;
};

using declaration_types = boost::variant<var_decl, statement>;
struct declaration
{
	declaration_types m_declaration;
};

struct program
{
	std::vector<declaration> m_declarations;
};

}  // namespace ast
}  // namespace sscript
