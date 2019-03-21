#include "ast.h"

#include "ast_visitor.h"

namespace sscript
{
namespace ast
{
void Number::AcceptVisitor(AstVisitor *v) { v->Visit(std::dynamic_pointer_cast<Number>(shared_from_this())); }

void String::AcceptVisitor(AstVisitor *v) { v->Visit(std::dynamic_pointer_cast<String>(shared_from_this())); }

void Identifier::AcceptVisitor(AstVisitor *v) { v->Visit(std::dynamic_pointer_cast<Identifier>(shared_from_this())); }

void Boolean::AcceptVisitor(AstVisitor *v) { v->Visit(std::dynamic_pointer_cast<Boolean>(shared_from_this())); }

void Null::AcceptVisitor(AstVisitor *v) { v->Visit(std::dynamic_pointer_cast<Null>(shared_from_this())); }

void ArithmeticOp::AcceptVisitor(AstVisitor *v)
{
	v->Visit(std::dynamic_pointer_cast<ArithmeticOp>(shared_from_this()));
}

void Unary::AcceptVisitor(AstVisitor *v) { v->Visit(std::dynamic_pointer_cast<Unary>(shared_from_this())); }

void ComparisonOp::AcceptVisitor(AstVisitor *v)
{
	v->Visit(std::dynamic_pointer_cast<ComparisonOp>(shared_from_this()));
}

void LogicOp::AcceptVisitor(AstVisitor *v) { v->Visit(std::dynamic_pointer_cast<LogicOp>(shared_from_this())); }

void Assignment::AcceptVisitor(AstVisitor *v) { v->Visit(std::dynamic_pointer_cast<Assignment>(shared_from_this())); }

void ExpressionStatement::AcceptVisitor(AstVisitor *v)
{
	v->Visit(std::dynamic_pointer_cast<ExpressionStatement>(shared_from_this()));
}

void IfStatement::AcceptVisitor(AstVisitor *v) { v->Visit(std::dynamic_pointer_cast<IfStatement>(shared_from_this())); }

void Loop::AcceptVisitor(AstVisitor *v) { v->Visit(std::dynamic_pointer_cast<Loop>(shared_from_this())); }

void VarDecl::AcceptVisitor(AstVisitor *v) { v->Visit(std::dynamic_pointer_cast<VarDecl>(shared_from_this())); }

void StatementBlock::AcceptVisitor(AstVisitor *v)
{
	v->Visit(std::dynamic_pointer_cast<StatementBlock>(shared_from_this()));
}

void Call::AcceptVisitor(AstVisitor *v) { v->Visit(std::dynamic_pointer_cast<Call>(shared_from_this())); }

void Program::AcceptVisitor(AstVisitor *v) { v->Visit(std::dynamic_pointer_cast<Program>(shared_from_this())); }
}  // namespace ast
}  // namespace sscript
