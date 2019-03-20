#include "interpreter.h"

#include "ast_visitor.h"
#include "symbol_table.h"
#include "value.h"

#include <cstdint>
#include <iostream>
#include <stack>

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

struct interpreter_visitor : public AstVisitor
{
	interpreter_visitor() : m_symbolTable(std::make_shared<SymbolTable>("Global")) {}
	~interpreter_visitor()
	{
		m_symbolTable->DumpSymbols();
		if (m_values.size() > 0)
		{
			std::cout << "Value stack not empty" << std::endl;
			value_visitor vis;
			while (!m_values.empty())
			{
				std::cout << boost::apply_visitor(vis, m_values.top()) << std::endl;
				m_values.pop();
			}
		}
	}

	void Visit(ast::NumberPtr n) override { PushValue(Float(n->GetNumber())); }

	void Visit(ast::StringPtr s) override { PushValue(String(s->GetString())); }

	void Visit(ast::IdentifierPtr i) override { PushValue(m_symbolTable->Get(i->GetIdentifier()).m_value); }

	void Visit(ast::BooleanPtr b) override { PushValue(Boolean(b->GetBoolean())); }

	void Visit(ast::Nullptr n) override { PushValue(NullObject()); }

	void Visit(ast::ArithmeticOpPtr op) override
	{
		op->GetRhs()->AcceptVisitor(this);
		op->GetLhs()->AcceptVisitor(this);

		auto lhs = PopValue();
		auto rhs = PopValue();

		switch (op->GetOperationType())
		{
			case ast::ArithmeticOp::types::Add:
			{
				binary_op_dispatcher<bin_ops::add> v(lhs, rhs);
				PushValue(boost::apply_visitor(v, lhs));
				break;
			}
			case ast::ArithmeticOp::types::Sub:
			{
				binary_op_dispatcher<bin_ops::sub> v(lhs, rhs);
				PushValue(boost::apply_visitor(v, lhs));
				break;
			}
			case ast::ArithmeticOp::types::Mul:
			{
				binary_op_dispatcher<bin_ops::mul> v(lhs, rhs);
				PushValue(boost::apply_visitor(v, lhs));
				break;
			}
			case ast::ArithmeticOp::types::Div:
			{
				binary_op_dispatcher<bin_ops::div> v(lhs, rhs);
				PushValue(boost::apply_visitor(v, lhs));
				break;
			}
		}
	}

	void Visit(ast::UnaryPtr u) override
	{
		u->GetRhs()->AcceptVisitor(this);
		auto rhs = PopValue();

		switch (u->GetOperationType())
		{
			case ast::Unary::types::Neg:
			{
				unary_ops_dispatcher<unary_ops::negate> v;
				PushValue(boost::apply_visitor(v, rhs));
				break;
			}
			case ast::Unary::types::Min:
			{
				unary_ops_dispatcher<unary_ops::minus> v;
				PushValue(boost::apply_visitor(v, rhs));
				break;
			}
		}
	}

	void Visit(ast::ComparisonOpPtr op) override
	{
		op->GetRhs()->AcceptVisitor(this);
		op->GetLhs()->AcceptVisitor(this);
		auto lhs = PopValue();
		auto rhs = PopValue();

		switch (op->GetOperationType())
		{
			case ast::ComparisonOp::types::Eq:
			{
				binary_op_dispatcher<bin_ops::eq> v(lhs, rhs);
				PushValue(boost::apply_visitor(v, lhs));
				break;
			}
			case ast::ComparisonOp::types::Ne:
			{
				binary_op_dispatcher<bin_ops::ne> v(lhs, rhs);
				PushValue(boost::apply_visitor(v, lhs));
				break;
			}
			case ast::ComparisonOp::types::Gt:
			{
				binary_op_dispatcher<bin_ops::gt> v(lhs, rhs);
				PushValue(boost::apply_visitor(v, lhs));
				break;
			}
			case ast::ComparisonOp::types::Gte:
			{
				binary_op_dispatcher<bin_ops::gte> v(lhs, rhs);
				PushValue(boost::apply_visitor(v, lhs));
				break;
			}
			case ast::ComparisonOp::types::Lt:
			{
				binary_op_dispatcher<bin_ops::lt> v(lhs, rhs);
				PushValue(boost::apply_visitor(v, lhs));
				break;
			}
			case ast::ComparisonOp::types::Lte:
			{
				binary_op_dispatcher<bin_ops::lte> v(lhs, rhs);
				PushValue(boost::apply_visitor(v, lhs));
				break;
			}
		}
	}

	void Visit(ast::LogicOpPtr op) override
	{
		op->GetLhs()->AcceptVisitor(this);

		auto lhs = PopValue();

		is_true t;
		auto lhs_true = boost::apply_visitor(t, lhs);
		switch (op->GetOperationType())
		{
			case ast::LogicOp::types::And:
			{
				if (lhs_true)
				{
					op->GetRhs()->AcceptVisitor(this);
					auto rhs = PopValue();
					PushValue(Boolean(boost::apply_visitor(t, rhs)));
					return;
				}
				PushValue(Boolean(false));
				break;
			}
			case ast::LogicOp::types::Or:
			{
				if (lhs_true)
				{
					PushValue(Boolean(true));
					return;
				}
				op->GetRhs()->AcceptVisitor(this);
				auto rhs = PopValue();
				PushValue(Boolean(boost::apply_visitor(t, rhs)));
				break;
			}
		}
	}

	void Visit(ast::AssignmentPtr a) override
	{
		a->GetRhs()->AcceptVisitor(this);
		auto rhs = PopValue();
		m_symbolTable->Assign(a->GetIdentifier()->GetIdentifier(), rhs);
		PushValue(rhs);
	}

	void Visit(ast::ExpressionStatementPtr e) override
	{
		e->GetExpression()->AcceptVisitor(this);
		PopValue();
	}

	void Visit(ast::IfStatementPtr i) override
	{
		i->GetCondition()->AcceptVisitor(this);
		auto condition = PopValue();

		auto falseStatement = i->GetFalseStatement();

		is_true t;
		if (boost::apply_visitor(t, condition))
		{
			i->GetTrueStatement()->AcceptVisitor(this);
		}
		else if (falseStatement)
		{
			falseStatement->AcceptVisitor(this);
		}
	}

	void Visit(ast::LoopPtr l) override
	{
		is_true t;
		while (true)
		{
			l->GetCondition()->AcceptVisitor(this);
			auto condition = PopValue();
			if (!boost::apply_visitor(t, condition))
			{
				break;
			}
			l->GetBody()->AcceptVisitor(this);
		}
	}

	void Visit(ast::VarDeclPtr v) override
	{
		auto rhs = v->GetRhs();

		if (rhs)
		{
			rhs->AcceptVisitor(this);
			m_symbolTable->Declare({v->GetIdentifier()->GetIdentifier(), PopValue()});
		}
		else
		{
			m_symbolTable->Declare({v->GetIdentifier()->GetIdentifier(), NullObject()});
		}
	}

	void Visit(ast::StatementBlockPtr b) override
	{
		auto previousSymbolTable = m_symbolTable;
		m_symbolTable = std::make_shared<SymbolTable>("Anonymous Block", previousSymbolTable);

		for (const auto &statement : b->GetStatements())
		{
			statement->AcceptVisitor(this);
		}

		m_symbolTable = previousSymbolTable;
	}

	void Visit(ast::ProgramPtr p) override
	{
		for (const auto &statement : p->GetStatements())
		{
			statement->AcceptVisitor(this);
		}
	}

	void PushValue(const value &v)
	{
		// value_visitor vis;
		// std::cout << "Pushing value " << boost::apply_visitor(vis, v) << std::endl;
		m_values.push(v);
	}

	value PopValue()
	{
		value v = m_values.top();
		m_values.pop();
		// value_visitor vis;
		// std::cout << "Popping value " << boost::apply_visitor(vis, v) << std::endl;
		return v;
	}

	std::shared_ptr<SymbolTable> m_symbolTable;

	std::stack<value> m_values;
};

Interpreter::Interpreter() {}

bool Interpreter::Interpret(const ast::ProgramPtr &program)
{
	interpreter_visitor vis;
	program->AcceptVisitor(&vis);

	return false;
}
}  // namespace sscript
