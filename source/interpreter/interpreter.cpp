#include "interpreter.h"

#include "intermediate/ast_visitor.h"
#include "symbol_table.h"
#include "value/base_value.h"
#include "value/boolean_value.h"
#include "value/builtin_callable.h"
#include "value/builtin_functions.h"
#include "value/callable.h"
#include "value/float_value.h"
#include "value/null_object_value.h"
#include "value/string_value.h"
#include "value/value_visitor.h"

#include <cstdint>
#include <iostream>
#include <stack>

namespace sscript
{
namespace bin_ops
{
struct add
{
	static BaseValuePtr apply(const Float &lhs, const Float &rhs)
	{
		return std::make_shared<Float>(lhs.m_value + rhs.m_value);
	}
	static BaseValuePtr apply(const String &lhs, const String &rhs)
	{
		return std::make_shared<String>(lhs.m_value + rhs.m_value);
	}

	template<typename LHS, typename RHS>
	static BaseValuePtr apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to add operands");
	}
};

struct sub
{
	static BaseValuePtr apply(const Float &lhs, const Float &rhs)
	{
		return std::make_shared<Float>(lhs.m_value - rhs.m_value);
	}

	template<typename LHS, typename RHS>
	static BaseValuePtr apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to subtract operands");
	}
};

struct mul
{
	static BaseValuePtr apply(const Float &lhs, const Float &rhs)
	{
		return std::make_shared<Float>(lhs.m_value * rhs.m_value);
	}
	static BaseValuePtr apply(const String &lhs, const Float &rhs)
	{
		std::string r;
		for (auto i = 0u; i < static_cast<uint32_t>(rhs.m_value); ++i)
		{
			r += lhs.m_value;
		}
		return std::make_shared<String>(r);
	}
	static BaseValuePtr apply(const Float &lhs, const String &rhs) { return apply(rhs, lhs); }

	template<typename LHS, typename RHS>
	static BaseValuePtr apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to multiply operands");
	}
};

struct div
{
	static BaseValuePtr apply(const Float &lhs, const Float &rhs)
	{
		return std::make_shared<Float>(lhs.m_value / rhs.m_value);
	}

	template<typename LHS, typename RHS>
	static BaseValuePtr apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to divide operands");
	}
};

struct eq
{
	static BaseValuePtr apply(const Float &lhs, const Float &rhs)
	{
		return std::make_shared<Boolean>(lhs.m_value == rhs.m_value);
	}
	static BaseValuePtr apply(const Boolean &lhs, const Boolean &rhs)
	{
		return std::make_shared<Boolean>(lhs.m_value == rhs.m_value);
	}
	static BaseValuePtr apply(const String &lhs, const String &rhs)
	{
		return std::make_shared<Boolean>(lhs.m_value == rhs.m_value);
	}

	template<typename LHS, typename RHS>
	static BaseValuePtr apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to compare operands for equality");
	}
};

struct ne
{
	static BaseValuePtr apply(const Float &lhs, const Float &rhs)
	{
		return std::make_shared<Boolean>(lhs.m_value != rhs.m_value);
	}
	static BaseValuePtr apply(const Boolean &lhs, const Boolean &rhs)
	{
		return std::make_shared<Boolean>(lhs.m_value != rhs.m_value);
	}
	static BaseValuePtr apply(const String &lhs, const String &rhs)
	{
		return std::make_shared<Boolean>(lhs.m_value != rhs.m_value);
	}

	template<typename LHS, typename RHS>
	static BaseValuePtr apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to compare operands for equality");
	}
};

struct gt
{
	static BaseValuePtr apply(const Float &lhs, const Float &rhs)
	{
		return std::make_shared<Boolean>(lhs.m_value > rhs.m_value);
	}

	template<typename LHS, typename RHS>
	static BaseValuePtr apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to compare operands for equality");
	}
};

struct gte
{
	static BaseValuePtr apply(const Float &lhs, const Float &rhs)
	{
		return std::make_shared<Boolean>(lhs.m_value >= rhs.m_value);
	}

	template<typename LHS, typename RHS>
	static BaseValuePtr apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to compare operands for equality");
	}
};

struct lt
{
	static BaseValuePtr apply(const Float &lhs, const Float &rhs)
	{
		return std::make_shared<Boolean>(lhs.m_value < rhs.m_value);
	}

	template<typename LHS, typename RHS>
	static BaseValuePtr apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to compare operands for equality");
	}
};

struct lte
{
	static BaseValuePtr apply(const Float &lhs, const Float &rhs)
	{
		return std::make_shared<Boolean>(lhs.m_value <= rhs.m_value);
	}

	template<typename LHS, typename RHS>
	static BaseValuePtr apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to compare operands for equality");
	}
};
}  // namespace bin_ops

namespace unary_ops
{
struct negate
{
	static BaseValuePtr apply(const Boolean &b) { return std::make_shared<Boolean>(!b.m_value); }

	template<typename T>
	static BaseValuePtr apply(const T &v)
	{
		throw std::runtime_error("Unable to negate operand");
	}
};

struct minus
{
	static BaseValuePtr apply(const Float &f) { return std::make_shared<Float>(-f.m_value); }

	template<typename T>
	static BaseValuePtr apply(const T &v)
	{
		throw std::runtime_error("Unable to apply minus to operand");
	}
};
}  // namespace unary_ops

template<class Op>
struct unary_ops_dispatcher : public ValueVisitor<BaseValuePtr>
{
	template<typename Operand>
	BaseValuePtr operator()(const Operand &operand)
	{
		return Op::apply(operand);
	}
};

template<class OP>
struct binary_op_dispatcher : public ValueVisitor<BaseValuePtr>
{
	BaseValuePtr m_lhs;
	BaseValuePtr m_rhs;

	binary_op_dispatcher(const BaseValuePtr &lhs, const BaseValuePtr &rhs) : m_lhs(lhs), m_rhs(rhs) {}

	template<typename LHS>
	struct binary_op_rhs_dispatcher : public ValueVisitor<BaseValuePtr>
	{
		const LHS &m_lhs;
		binary_op_rhs_dispatcher(const LHS &lhs) : m_lhs(lhs) {}

		template<typename RHS>
		BaseValuePtr operator()(const RHS &rhs)
		{
			return OP::apply(m_lhs, rhs);
		}
	};

	template<typename LHS>
	BaseValuePtr operator()(const LHS &lhs)
	{
		binary_op_rhs_dispatcher<LHS> rhs_visitor{lhs};
		return apply_visitor(rhs_visitor, m_rhs);
	}
};

struct is_true : public ValueVisitor<bool>
{
	bool operator()(const Float &f) { return f.m_value; }
	bool operator()(const Boolean &b) { return b.m_value; }
	bool operator()(const String &s) { return s.m_value.size() != 0; }
	bool operator()(const NullObject &n) { return false; }

	template<typename T>
	bool operator()(const T &t)
	{
		throw std::runtime_error("Unable to evaluate truthness");
	}
};

struct interpreter_visitor : public AstVisitor
{
	interpreter_visitor() : m_symbolTable(std::make_shared<SymbolTable>("Global"))
	{
		m_symbolTable->Declare({"print", register_callable(make_function(print))});
		m_symbolTable->Declare({"time", register_callable(make_function(time))});
	}

	~interpreter_visitor() {}

	void Visit(ast::NumberPtr n) override { PushValue(std::make_shared<Float>(n->GetNumber())); }

	void Visit(ast::StringPtr s) override { PushValue(std::make_shared<String>(s->GetString())); }

	void Visit(ast::IdentifierPtr i) override { PushValue(m_symbolTable->Get(i->GetIdentifier()).m_value); }

	void Visit(ast::BooleanPtr b) override { PushValue(std::make_shared<Boolean>(b->GetBoolean())); }

	void Visit(ast::Nullptr n) override { PushValue(std::make_shared<NullObject>()); }

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
				PushValue(apply_visitor(v, lhs));
				break;
			}
			case ast::ArithmeticOp::types::Sub:
			{
				binary_op_dispatcher<bin_ops::sub> v(lhs, rhs);
				PushValue(apply_visitor(v, lhs));
				break;
			}
			case ast::ArithmeticOp::types::Mul:
			{
				binary_op_dispatcher<bin_ops::mul> v(lhs, rhs);
				PushValue(apply_visitor(v, lhs));
				break;
			}
			case ast::ArithmeticOp::types::Div:
			{
				binary_op_dispatcher<bin_ops::div> v(lhs, rhs);
				PushValue(apply_visitor(v, lhs));
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
				PushValue(apply_visitor(v, rhs));
				break;
			}
			case ast::Unary::types::Min:
			{
				unary_ops_dispatcher<unary_ops::minus> v;
				PushValue(apply_visitor(v, rhs));
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
				PushValue(apply_visitor(v, lhs));
				break;
			}
			case ast::ComparisonOp::types::Ne:
			{
				binary_op_dispatcher<bin_ops::ne> v(lhs, rhs);
				PushValue(apply_visitor(v, lhs));
				break;
			}
			case ast::ComparisonOp::types::Gt:
			{
				binary_op_dispatcher<bin_ops::gt> v(lhs, rhs);
				PushValue(apply_visitor(v, lhs));
				break;
			}
			case ast::ComparisonOp::types::Gte:
			{
				binary_op_dispatcher<bin_ops::gte> v(lhs, rhs);
				PushValue(apply_visitor(v, lhs));
				break;
			}
			case ast::ComparisonOp::types::Lt:
			{
				binary_op_dispatcher<bin_ops::lt> v(lhs, rhs);
				PushValue(apply_visitor(v, lhs));
				break;
			}
			case ast::ComparisonOp::types::Lte:
			{
				binary_op_dispatcher<bin_ops::lte> v(lhs, rhs);
				PushValue(apply_visitor(v, lhs));
				break;
			}
		}
	}

	void Visit(ast::LogicOpPtr op) override
	{
		op->GetLhs()->AcceptVisitor(this);

		auto lhs = PopValue();

		is_true t;
		auto lhs_true = apply_visitor(t, lhs);
		switch (op->GetOperationType())
		{
			case ast::LogicOp::types::And:
			{
				if (lhs_true)
				{
					op->GetRhs()->AcceptVisitor(this);
					auto rhs = PopValue();
					PushValue(std::make_shared<Boolean>(apply_visitor(t, rhs)));
					return;
				}
				PushValue(std::make_shared<Boolean>(false));
				break;
			}
			case ast::LogicOp::types::Or:
			{
				if (lhs_true)
				{
					PushValue(std::make_shared<Boolean>(true));
					return;
				}
				op->GetRhs()->AcceptVisitor(this);
				auto rhs = PopValue();
				PushValue(std::make_shared<Boolean>(apply_visitor(t, rhs)));
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
		if (apply_visitor(t, condition))
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
			if (!apply_visitor(t, condition))
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
			m_symbolTable->Declare({v->GetIdentifier()->GetIdentifier(), std::make_shared<NullObject>()});
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

	void Visit(ast::CallPtr c) override
	{
		c->GetCallee()->AcceptVisitor(this);
		auto callee = PopValue();

		std::vector<BaseValuePtr> args;
		for (auto &a : c->GetArguments())
		{
			a->AcceptVisitor(this);
			args.push_back(PopValue());
		}

		try
		{
			std::dynamic_pointer_cast<Callable>(callee)->Call(args);
		}
		catch (BaseValuePtr &f)
		{
			PushValue(f);
			return;
		}

		PushValue(std::make_shared<NullObject>());
	}

	void Visit(ast::ProgramPtr p) override
	{
		for (const auto &statement : p->GetStatements())
		{
			statement->AcceptVisitor(this);
		}
	}

	void PushValue(const BaseValuePtr &v)
	{
		// value_visitor vis;
		// std::cout << "Pushing value " << apply_visitor(vis, v) << std::endl;
		m_values.push(v);
	}

	BaseValuePtr PopValue()
	{
		BaseValuePtr v = m_values.top();
		m_values.pop();
		// value_visitor vis;
		// std::cout << "Popping value " << apply_visitor(vis, v) << std::endl;
		return v;
	}

	std::shared_ptr<SymbolTable> m_symbolTable;

	std::stack<BaseValuePtr> m_values;
};

Interpreter::Interpreter() {}

bool Interpreter::Interpret(const ast::ProgramPtr &program)
{
	interpreter_visitor vis;
	program->AcceptVisitor(&vis);

	return false;
}
}  // namespace sscript
