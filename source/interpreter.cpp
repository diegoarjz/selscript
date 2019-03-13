#include "interpreter.h"

#include "builtin_value.h"
#include "symbol_table.h"

#include <iostream>

namespace sscript
{
namespace bin_ops
{
struct add
{
	static builtin_value apply(const float &lhs, const float &rhs) { return lhs + rhs; }
	static builtin_value apply(const std::string &lhs, const std::string &rhs) { return lhs + rhs; }

	template<typename LHS, typename RHS>
	static builtin_value apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to add operands");
	}
};

struct sub
{
	static builtin_value apply(const float &lhs, const float &rhs) { return lhs - rhs; }

	template<typename LHS, typename RHS>
	static builtin_value apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to subtract operands");
	}
};

struct mul
{
	static builtin_value apply(const float &lhs, const float &rhs) { return lhs * rhs; }
	static builtin_value apply(const std::string &lhs, const float &rhs)
	{
		auto r = lhs;
		for (auto i = 0u; i < rhs; ++i)
		{
			r += lhs;
		}
		return r;
	}
	static builtin_value apply(const float &lhs, const std::string &rhs) { return apply(rhs, lhs); }

	template<typename LHS, typename RHS>
	static builtin_value apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to multiply operands");
	}
};

struct div
{
	static builtin_value apply(const float &lhs, const float &rhs) { return lhs / rhs; }

	template<typename LHS, typename RHS>
	static builtin_value apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to divide operands");
	}
};

struct eq
{
	static builtin_value apply(const float &lhs, const float &rhs) { return lhs == rhs; }
	static builtin_value apply(const bool &lhs, const bool &rhs) { return lhs == rhs; }
	static builtin_value apply(const std::string &lhs, const std::string &rhs) { return lhs == rhs; }

	template<typename LHS, typename RHS>
	static builtin_value apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to compare operands for equality");
	}
};

struct ne
{
	static builtin_value apply(const float &lhs, const float &rhs) { return lhs != rhs; }
	static builtin_value apply(const bool &lhs, const bool &rhs) { return lhs != rhs; }
	static builtin_value apply(const std::string &lhs, const std::string &rhs) { return lhs != rhs; }

	template<typename LHS, typename RHS>
	static builtin_value apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to compare operands for equality");
	}
};

struct gt
{
	static builtin_value apply(const float &lhs, const float &rhs) { return lhs > rhs; }

	template<typename LHS, typename RHS>
	static builtin_value apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to compare operands for equality");
	}
};

struct gte
{
	static builtin_value apply(const float &lhs, const float &rhs) { return lhs >= rhs; }

	template<typename LHS, typename RHS>
	static builtin_value apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to compare operands for equality");
	}
};

struct lt
{
	static builtin_value apply(const float &lhs, const float &rhs) { return lhs < rhs; }

	template<typename LHS, typename RHS>
	static builtin_value apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to compare operands for equality");
	}
};

struct lte
{
	static builtin_value apply(const float &lhs, const float &rhs) { return lhs <= rhs; }

	template<typename LHS, typename RHS>
	static builtin_value apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to compare operands for equality");
	}
};
}  // namespace bin_ops

namespace unary_ops
{
struct negate
{
	static builtin_value apply(const bool &b) { return !b; }

	template<typename T>
	static builtin_value apply(const T &v)
	{
		throw std::runtime_error("Unable to negate operand");
	}
};

struct minus
{
	static builtin_value apply(const float &f) { return -f; }

	template<typename T>
	static builtin_value apply(const T &v)
	{
		throw std::runtime_error("Unable to apply minus to operand");
	}
};
}  // namespace unary_ops

template<class Op>
struct unary_ops_dispatcher : public boost::static_visitor<builtin_value>
{
	template<typename Operand>
	builtin_value operator()(const Operand &operand)
	{
		return Op::apply(operand);
	}
};

template<class OP>
struct binary_op_dispatcher : public boost::static_visitor<builtin_value>
{
	builtin_value m_lhs;
	builtin_value m_rhs;

	binary_op_dispatcher(const builtin_value &lhs, const builtin_value &rhs) : m_lhs(lhs), m_rhs(rhs) {}

	template<typename LHS>
	struct binary_op_rhs_dispatcher : public boost::static_visitor<builtin_value>
	{
		const LHS &m_lhs;
		binary_op_rhs_dispatcher(const LHS &lhs) : m_lhs(lhs) {}

		template<typename RHS>
		builtin_value operator()(const RHS &rhs)
		{
			return OP::apply(m_lhs, rhs);
		}
	};

	template<typename LHS>
	builtin_value operator()(const LHS &lhs)
	{
		binary_op_rhs_dispatcher<LHS> rhs_visitor{lhs};
		return boost::apply_visitor(rhs_visitor, m_rhs);
	}
};

struct is_true : public boost::static_visitor<bool>
{
	bool operator()(const float &f) { return f; }
	bool operator()(const bool &b) { return b; }
	bool operator()(const std::string &s) { return s.size() != 0; }
};

struct interpreter_visitor
{
	interpreter_visitor(SymbolTable *symbolTable) : m_symbolTable(symbolTable) {}

	template<typename Ret = void>
	struct dispatcher_visitor : boost::static_visitor<Ret>
	{
		dispatcher_visitor(interpreter_visitor *v) : m_interpreterVisitor(v) {}

		template<typename T>
		Ret operator()(const T &t)
		{
			return m_interpreterVisitor->visit(t);
		}

		interpreter_visitor *m_interpreterVisitor;
	};

	void visit(const ast::program &program)
	{
		for (const auto &declaration : program.m_declarations)
		{
			visit(declaration);
		}
	}

	void visit(const ast::declaration &declaration)
	{
		dispatcher_visitor<> decl(this);
		boost::apply_visitor(decl, declaration.m_declaration);
	}

	void visit(const ast::var_decl &decl)
	{
		if (decl.m_rhs)
		{
			auto rhs = visit(decl.m_rhs.get());
			m_symbolTable->AddSymbol({decl.m_variableName.m_identifier, rhs});
		}
		else
		{
			m_symbolTable->AddSymbol({decl.m_variableName.m_identifier, builtin_value()});
		}
	}

	void visit(const ast::statement &statement)
	{
		dispatcher_visitor<builtin_value> v(this);
		boost::apply_visitor(v, statement.m_statement);
	}

	builtin_value visit(const ast::expression &expresion)
	{
		dispatcher_visitor<builtin_value> v(this);
		return boost::apply_visitor(v, expresion);
	}

	builtin_value visit(const ast::number &n) { return n.m_number; }

	builtin_value visit(const ast::string &s) { return s.m_string; }

	builtin_value visit(const bool &b) { return b; }
	builtin_value visit(const ast::null &n) { return false; }

	builtin_value visit(const ast::identifier &i) { return m_symbolTable->GetSymbol(i.m_identifier).m_value; }

	builtin_value visit(const ast::arithmetic_op &op)
	{
		auto lhs = visit(op.m_lhs);
		auto rhs = visit(op.m_rhs);

		switch (op.m_operation)
		{
			case ast::arithmetic_op::types::Add:
			{
				binary_op_dispatcher<bin_ops::add> v(lhs, rhs);
				return boost::apply_visitor(v, lhs);
			}
			case ast::arithmetic_op::types::Sub:
			{
				binary_op_dispatcher<bin_ops::sub> v(lhs, rhs);
				return boost::apply_visitor(v, lhs);
			}
			case ast::arithmetic_op::types::Mul:
			{
				binary_op_dispatcher<bin_ops::mul> v(lhs, rhs);
				return boost::apply_visitor(v, lhs);
			}
			case ast::arithmetic_op::types::Div:
			{
				binary_op_dispatcher<bin_ops::div> v(lhs, rhs);
				return boost::apply_visitor(v, lhs);
			}
		}

		return 0.0f;
	}

	builtin_value visit(const ast::unary &op)
	{
		auto operand = visit(op.m_operand);

		switch (op.m_operation)
		{
			case ast::unary::types::Neg:
			{
				unary_ops_dispatcher<unary_ops::negate> v;
				return boost::apply_visitor(v, operand);
			}
			case ast::unary::types::Min:
			{
				unary_ops_dispatcher<unary_ops::minus> v;
				return boost::apply_visitor(v, operand);
			}
		}

		throw std::runtime_error("Unknown unary op");
	}

	builtin_value visit(const ast::comparison_op &op)
	{
		auto lhs = visit(op.m_lhs);
		auto rhs = visit(op.m_rhs);

		switch (op.m_operation)
		{
			case ast::comparison_op::types::Eq:
			{
				binary_op_dispatcher<bin_ops::eq> v(lhs, rhs);
				return boost::apply_visitor(v, lhs);
			}
			case ast::comparison_op::types::Ne:
			{
				binary_op_dispatcher<bin_ops::ne> v(lhs, rhs);
				return boost::apply_visitor(v, lhs);
			}
			case ast::comparison_op::types::Gt:
			{
				binary_op_dispatcher<bin_ops::gt> v(lhs, rhs);
				return boost::apply_visitor(v, lhs);
			}
			case ast::comparison_op::types::Gte:
			{
				binary_op_dispatcher<bin_ops::gte> v(lhs, rhs);
				return boost::apply_visitor(v, lhs);
			}
			case ast::comparison_op::types::Lt:
			{
				binary_op_dispatcher<bin_ops::lt> v(lhs, rhs);
				return boost::apply_visitor(v, lhs);
			}
			case ast::comparison_op::types::Lte:
			{
				binary_op_dispatcher<bin_ops::lte> v(lhs, rhs);
				return boost::apply_visitor(v, lhs);
			}
		}

		throw std::runtime_error("Unknown comparison op");
	}

	builtin_value visit(const ast::logic_op &op)
	{
		auto lhs = visit(op.m_lhs);
		is_true t;
		auto lhs_true = boost::apply_visitor(t, lhs);
		switch (op.m_operation)
		{
			case ast::logic_op::types::And:
			{
				if (lhs_true)
				{
					auto rhs = visit(op.m_rhs);
					return boost::apply_visitor(t, rhs);
				}
				return false;
			}
			case ast::logic_op::types::Or:
			{
				if (lhs_true)
				{
					return true;
				}
				auto rhs = visit(op.m_rhs);
				return boost::apply_visitor(t, rhs);
			}
		}

		return true;
	}

	builtin_value visit(const ast::assignment &assignment)
	{
		auto rhs = visit(assignment.m_rhs);
		auto symbol = m_symbolTable->GetSymbol(assignment.m_variableName.m_identifier);
		symbol.m_value = rhs;
		return rhs;
	}

	SymbolTable *m_symbolTable;
};

Interpreter::Interpreter() : m_symbolTable(std::make_unique<SymbolTable>()) {}

bool Interpreter::Interpret(const ast::program &program)
{
	interpreter_visitor vis(m_symbolTable.get());
	vis.visit(program);

	m_symbolTable->DumpSymbols();

	return false;
}
}  // namespace sscript
