#pragma once

#include "ast.h"
#include "ast_visitor.h"

namespace sscript
{
class AstPrinter : public AstVisitor
{
public:
	AstPrinter();

	void Visit(ast::NumberPtr) override;
	void Visit(ast::StringPtr) override;
	void Visit(ast::IdentifierPtr) override;
	void Visit(ast::BooleanPtr) override;
	void Visit(ast::Nullptr) override;
	void Visit(ast::ArithmeticOpPtr) override;
	void Visit(ast::UnaryPtr) override;
	void Visit(ast::ComparisonOpPtr) override;
	void Visit(ast::LogicOpPtr) override;
	void Visit(ast::AssignmentPtr) override;
	void Visit(ast::ExpressionStatementPtr) override;
	void Visit(ast::IfStatementPtr) override;
	void Visit(ast::LoopPtr) override;
	void Visit(ast::VarDeclPtr) override;
	void Visit(ast::StatementBlockPtr) override;
	void Visit(ast::ProgramPtr) override;

private:
	void indent();
	std::string indentation();
	void deindent();

	uint32_t m_indentation;
};
}  // namespace sscript