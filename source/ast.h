#pragma once

#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include <string>
#include <vector>

#include <iostream>

namespace sscript
{
class AstVisitor;

namespace ast
{
class AstNode : public std::enable_shared_from_this<AstNode>
{
public:
	AstNode() = default;

	AstNode(const AstNode &) = delete;
	AstNode &operator=(const AstNode &) = delete;

	virtual void AcceptVisitor(AstVisitor *) = 0;

private:
};

struct Expression : public AstNode
{
	Expression() = default;
};
using ExpressionPtr = std::shared_ptr<Expression>;

struct Number : public Expression
{
	explicit Number(const float &f) : m_number(f) {}

	const float &GetNumber() const { return m_number; }

	void AcceptVisitor(AstVisitor *v) override;

private:
	float m_number;
};
using NumberPtr = std::shared_ptr<Number>;

struct String : public Expression
{
	explicit String(const std::string &&s) : m_string(s) {}
	explicit String(const std::vector<char> &s) : m_string(s.begin(), s.end()) {}

	const std::string &GetString() const { return m_string; }

	void AcceptVisitor(AstVisitor *v) override;

private:
	std::string m_string;
};
using StringPtr = std::shared_ptr<String>;

struct Identifier : public Expression
{
	explicit Identifier(const std::string &i) : m_identifier(i) {}
	explicit Identifier(const std::vector<char> &i) : m_identifier(i.begin(), i.end()) {}

	const std::string &GetIdentifier() const { return m_identifier; }

	void AcceptVisitor(AstVisitor *v) override;

private:
	std::string m_identifier;
};
using IdentifierPtr = std::shared_ptr<Identifier>;

struct Boolean : public Expression
{
	explicit Boolean(const bool &b) : m_boolean(b) {}

	const bool &GetBoolean() const { return m_boolean; }

	void AcceptVisitor(AstVisitor *v) override;

private:
	bool m_boolean;
};
using BooleanPtr = std::shared_ptr<Boolean>;

struct Null : public Expression
{
	void AcceptVisitor(AstVisitor *v) override;
};
using Nullptr = std::shared_ptr<Null>;

struct ArithmeticOp : public Expression
{
	enum class types
	{
		Add,
		Sub,
		Mul,
		Div
	};

	ArithmeticOp(const types op, const ExpressionPtr lhs, const ExpressionPtr rhs)
	    : m_operation(op), m_lhs(lhs), m_rhs(rhs)
	{
	}

	const types &GetOperationType() const { return m_operation; }
	const ExpressionPtr &GetLhs() const { return m_lhs; }
	const ExpressionPtr &GetRhs() const { return m_rhs; }

	void AcceptVisitor(AstVisitor *v) override;

private:
	types m_operation;
	ExpressionPtr m_lhs;
	ExpressionPtr m_rhs;
};
using ArithmeticOpPtr = std::shared_ptr<ArithmeticOp>;

struct Unary : public Expression
{
	enum class types
	{
		Neg,
		Min
	};

	Unary(const types &op, const ExpressionPtr &rhs) : m_operation(op), m_expression(rhs) {}

	const types &GetOperationType() const { return m_operation; }
	const ExpressionPtr &GetRhs() const { return m_expression; }

	void AcceptVisitor(AstVisitor *v) override;

private:
	types m_operation;
	const ExpressionPtr m_expression;
};
using UnaryPtr = std::shared_ptr<Unary>;

struct ComparisonOp : public Expression
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

	ComparisonOp(const types op, const ExpressionPtr lhs, const ExpressionPtr rhs)
	    : m_operation(op), m_lhs(lhs), m_rhs(rhs)
	{
	}

	const types &GetOperationType() const { return m_operation; }
	const ExpressionPtr &GetLhs() const { return m_lhs; }
	const ExpressionPtr &GetRhs() const { return m_rhs; }
	void AcceptVisitor(AstVisitor *v) override;

private:
	types m_operation;
	ExpressionPtr m_lhs;
	ExpressionPtr m_rhs;
};
using ComparisonOpPtr = std::shared_ptr<ComparisonOp>;

struct LogicOp : public Expression
{
	enum class types
	{
		And,
		Or
	};

	LogicOp(const types op, const ExpressionPtr lhs, const ExpressionPtr rhs) : m_operation(op), m_lhs(lhs), m_rhs(rhs)
	{
	}

	const types &GetOperationType() const { return m_operation; }
	const ExpressionPtr &GetLhs() const { return m_lhs; }
	const ExpressionPtr &GetRhs() const { return m_rhs; }

	void AcceptVisitor(AstVisitor *v) override;

private:
	types m_operation;
	ExpressionPtr m_lhs;
	ExpressionPtr m_rhs;
};
using LogicOpPtr = std::shared_ptr<LogicOp>;

struct Assignment : public Expression
{
	Assignment(std::shared_ptr<Identifier> identifier, ExpressionPtr rhs) : m_identifier(identifier), m_rhs(rhs) {}

	const std::shared_ptr<Identifier> &GetIdentifier() const { return m_identifier; }
	const ExpressionPtr &GetRhs() const { return m_rhs; }
	void AcceptVisitor(AstVisitor *v) override;

private:
	std::shared_ptr<Identifier> m_identifier;
	ExpressionPtr m_rhs;
};
using AssignmentPtr = std::shared_ptr<Assignment>;

class Statement : public AstNode
{
public:
	Statement() = default;
};
using StatementPtr = std::shared_ptr<Statement>;

struct ExpressionStatement : public Statement
{
	ExpressionStatement(ExpressionPtr expression) : m_expression(expression) {}

	const ExpressionPtr &GetExpression() const { return m_expression; }

	void AcceptVisitor(AstVisitor *v) override;

private:
	ExpressionPtr m_expression;
};
using ExpressionStatementPtr = std::shared_ptr<ExpressionStatement>;

struct IfStatement : public Statement
{
	IfStatement(ExpressionPtr condition, StatementPtr trueStatement, StatementPtr falseStatement)
	    : m_condition(condition), m_trueStatement(trueStatement), m_falseStatement(falseStatement)
	{
	}

	const ExpressionPtr &GetCondition() const { return m_condition; }
	const StatementPtr &GetTrueStatement() const { return m_trueStatement; }
	const StatementPtr &GetFalseStatement() const { return m_falseStatement; }
	void SetFalseStatement(StatementPtr &falseStatement) { m_falseStatement = falseStatement; }
	void AcceptVisitor(AstVisitor *v) override;

private:
	ExpressionPtr m_condition;
	StatementPtr m_trueStatement;
	StatementPtr m_falseStatement;
};
using IfStatementPtr = std::shared_ptr<IfStatement>;

struct Loop : public Statement
{
	Loop(ExpressionPtr condition, StatementPtr body) : m_condition(condition), m_body(body) {}

	const ExpressionPtr &GetCondition() const { return m_condition; }
	const StatementPtr &GetBody() const { return m_body; }

	void AcceptVisitor(AstVisitor *v) override;

private:
	ExpressionPtr m_condition;
	StatementPtr m_body;
};
using LoopPtr = std::shared_ptr<Loop>;

struct VarDecl : public Statement
{
	VarDecl(std::shared_ptr<Identifier> identifier, ExpressionPtr rhs) : m_identifier(identifier), m_rhs(rhs) {}

	const std::shared_ptr<Identifier> &GetIdentifier() const { return m_identifier; }
	const ExpressionPtr &GetRhs() const { return m_rhs; }
	void AcceptVisitor(AstVisitor *v) override;

private:
	std::shared_ptr<Identifier> m_identifier;
	ExpressionPtr m_rhs;
};
using VarDeclPtr = std::shared_ptr<VarDecl>;

struct StatementBlock : public Statement
{
	StatementBlock() = default;

	const std::vector<StatementPtr> &GetStatements() const { return m_statements; }
	void AddStatement(const StatementPtr &statement) { m_statements.push_back(statement); }

	void AcceptVisitor(AstVisitor *v) override;

private:
	std::vector<StatementPtr> m_statements;
};
using StatementBlockPtr = std::shared_ptr<StatementBlock>;

class Program : public AstNode
{
public:
	Program() = default;
	Program(const std::vector<StatementPtr> &statements) : m_statements(statements) {}

	const std::vector<StatementPtr> &GetStatements() const { return m_statements; }

	void AcceptVisitor(AstVisitor *v) override;

private:
	std::vector<StatementPtr> m_statements;
};
using ProgramPtr = std::shared_ptr<Program>;

}  // namespace ast
}  // namespace sscript
