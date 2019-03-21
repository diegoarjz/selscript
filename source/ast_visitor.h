#pragma once

#include "ast.h"

namespace sscript
{
class AstVisitor
{
public:
	virtual void Visit(ast::NumberPtr) = 0;
	virtual void Visit(ast::StringPtr) = 0;
	virtual void Visit(ast::IdentifierPtr) = 0;
	virtual void Visit(ast::BooleanPtr) = 0;
	virtual void Visit(ast::Nullptr) = 0;
	virtual void Visit(ast::ArithmeticOpPtr) = 0;
	virtual void Visit(ast::UnaryPtr) = 0;
	virtual void Visit(ast::ComparisonOpPtr) = 0;
	virtual void Visit(ast::LogicOpPtr) = 0;
	virtual void Visit(ast::AssignmentPtr) = 0;
	virtual void Visit(ast::ExpressionStatementPtr) = 0;
	virtual void Visit(ast::IfStatementPtr) = 0;
	virtual void Visit(ast::LoopPtr) = 0;
	virtual void Visit(ast::VarDeclPtr) = 0;
	virtual void Visit(ast::StatementBlockPtr) = 0;
	virtual void Visit(ast::CallPtr) = 0;
	virtual void Visit(ast::ProgramPtr) = 0;
};
}  // namespace sscript
