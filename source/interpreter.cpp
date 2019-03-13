#include "interpreter.h"

#include "symbol_table.h"
#include "value.h"

#include <cstdint>
#include <iostream>

namespace sscript
{
namespace bin_ops
{
struct add
{
	static value apply(const Float &lhs, const Float &rhs) { return Float(lhs.m_value + rhs.m_value); }
	static value apply(const String &lhs, const String &rhs) { return String(lhs.m_value + rhs.m_value); }

	template<typename LHS, typename RHS>
	static value apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to add operands");
	}
};

struct sub
{
	static value apply(const Float &lhs, const Float &rhs) { return Float(lhs.m_value - rhs.m_value); }

	template<typename LHS, typename RHS>
	static value apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to subtract operands");
	}
};

struct mul
{
	static value apply(const Float &lhs, const Float &rhs) { return Float(lhs.m_value * rhs.m_value); }
	static value apply(const String &lhs, const Float &rhs)
	{
		std::string r;
		for (auto i = 0u; i < static_cast<uint32_t>(rhs.m_value); ++i)
		{
			r += lhs.m_value;
		}
		return String(r);
	}
	static value apply(const Float &lhs, const String &rhs) { return apply(rhs, lhs); }

	template<typename LHS, typename RHS>
	static value apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to multiply operands");
	}
};

struct div
{
	static value apply(const Float &lhs, const Float &rhs) { return Float(lhs.m_value / rhs.m_value); }

	template<typename LHS, typename RHS>
	static value apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to divide operands");
	}
};

struct eq
{
	static value apply(const Float &lhs, const Float &rhs) { return Boolean(lhs.m_value == rhs.m_value); }
	static value apply(const Boolean &lhs, const Boolean &rhs) { return Boolean(lhs.m_value == rhs.m_value); }
	static value apply(const String &lhs, const String &rhs) { return Boolean(lhs.m_value == rhs.m_value); }

	template<typename LHS, typename RHS>
	static value apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to compare operands for equality");
	}
};

struct ne
{
	static value apply(const Float &lhs, const Float &rhs) { return Boolean(lhs.m_value != rhs.m_value); }
	static value apply(const Boolean &lhs, const Boolean &rhs) { return Boolean(lhs.m_value != rhs.m_value); }
	static value apply(const String &lhs, const String &rhs) { return Boolean(lhs.m_value != rhs.m_value); }

	template<typename LHS, typename RHS>
	static value apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to compare operands for equality");
	}
};

struct gt
{
	static value apply(const Float &lhs, const Float &rhs) { return Boolean(lhs.m_value > rhs.m_value); }

	template<typename LHS, typename RHS>
	static value apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to compare operands for equality");
	}
};

struct gte
{
	static value apply(const Float &lhs, const Float &rhs) { return Boolean(lhs.m_value >= rhs.m_value); }

	template<typename LHS, typename RHS>
	static value apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to compare operands for equality");
	}
};

struct lt
{
	static value apply(const Float &lhs, const Float &rhs) { return Boolean(lhs.m_value < rhs.m_value); }

	template<typename LHS, typename RHS>
	static value apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to compare operands for equality");
	}
};

struct lte
{
	static value apply(const Float &lhs, const Float &rhs) { return Boolean(lhs.m_value <= rhs.m_value); }

	template<typename LHS, typename RHS>
	static value apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to compare operands for equality");
	}
};
}  // namespace bin_ops

namespace unary_ops
{
struct negate
{
	static value apply(const Boolean &b) { return Boolean(!b.m_value); }

	template<typename T>
	static value apply(const T &v)
	{
		throw std::runtime_error("Unable to negate operand");
	}
};

struct minus
{
	static value apply(const Float &f) { return Float(-f.m_value); }

	template<typename T>
	static value apply(const T &v)
	{
		throw std::runtime_error("Unable to apply minus to operand");
	}
};
}  // namespace unary_ops

template<class Op>
struct unary_ops_dispatcher : public boost::static_visitor<value>
{
	template<typename Operand>
	value operator()(const Operand &operand)
	{
		return Op::apply(operand);
	}
};

template<class OP>
struct binary_op_dispatcher : public boost::static_visitor<value>
{
	value m_lhs;
	value m_rhs;

	binary_op_dispatcher(const value &lhs, const value &rhs) : m_lhs(lhs), m_rhs(rhs) {}

	template<typename LHS>
	struct binary_op_rhs_dispatcher : public boost::static_visitor<value>
	{
		const LHS &m_lhs;
		binary_op_rhs_dispatcher(const LHS &lhs) : m_lhs(lhs) {}

		template<typename RHS>
		value operator()(const RHS &rhs)
		{
			return OP::apply(m_lhs, rhs);
		}
	};

	template<typename LHS>
	value operator()(const LHS &lhs)
	{
		binary_op_rhs_dispatcher<LHS> rhs_visitor{lhs};
		return boost::apply_visitor(rhs_visitor, m_rhs);
	}
};

struct is_true : public boost::static_visitor<bool>
{
	bool operator()(const Float &f) { return f.m_value; }
	bool operator()(const Boolean &b) { return b.m_value; }
	bool operator()(const String &s) { return s.m_value.size() != 0; }
	bool operator()(const NullObject &n) { return false; }
};

struct interpreter_visitor
{
	interpreter_visitor() : m_symbolTable(std::make_shared<SymbolTable>("Global")) {}
	~interpreter_visitor() { m_symbolTable->DumpSymbols(); }

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
			m_symbolTable->Declare({decl.m_variableName.m_identifier, rhs});
		}
		else
		{
			m_symbolTable->Declare({decl.m_variableName.m_identifier, value()});
		}
	}

	void visit(const ast::statement &statement)
	{
		dispatcher_visitor<value> v(this);
		boost::apply_visitor(v, statement.m_statement);
	}

	value visit(const ast::expression &expresion)
	{
		dispatcher_visitor<value> v(this);
		return boost::apply_visitor(v, expresion);
	}

	value visit(const ast::statement_block &block)
	{
		auto previousSymbolTable = m_symbolTable;
		m_symbolTable = std::make_shared<SymbolTable>("Anonymous Block", previousSymbolTable);

		for (const auto &declaration : block.m_declarations)
		{
			visit(declaration);
		}

		m_symbolTable->DumpSymbols();
		m_symbolTable = previousSymbolTable;

		return NullObject();
	}

	value visit(const ast::number &n) { return Float(n.m_number); }

	value visit(const ast::string &s) { return String(s.m_string); }

	value visit(const bool &b) { return Boolean(b); }
	value visit(const ast::null &n) { return NullObject(); }

	value visit(const ast::identifier &i) { return m_symbolTable->Get(i.m_identifier).m_value; }

	value visit(const ast::arithmetic_op &op)
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

		throw std::runtime_error("Unknown arithmetic_op");
	}

	value visit(const ast::unary &op)
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

	value visit(const ast::comparison_op &op)
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

	value visit(const ast::logic_op &op)
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
					return Boolean(boost::apply_visitor(t, rhs));
				}
				return Boolean(false);
			}
			case ast::logic_op::types::Or:
			{
				if (lhs_true)
				{
					return Boolean(true);
				}
				auto rhs = visit(op.m_rhs);
				return Boolean(boost::apply_visitor(t, rhs));
			}
		}

		throw std::runtime_error("Unknown logic op");
	}

	value visit(const ast::assignment &assignment)
	{
		auto rhs = visit(assignment.m_rhs);
		m_symbolTable->Assign(assignment.m_variableName.m_identifier, rhs);
		return rhs;
	}

	std::shared_ptr<SymbolTable> m_symbolTable;
};

Interpreter::Interpreter() {}

bool Interpreter::Interpret(const ast::program &program)
{
	interpreter_visitor vis;
	vis.visit(program);

	return false;
}
}  // namespace sscript
