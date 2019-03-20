#include "ast_printer.h"

namespace sscript
{
AstPrinter::AstPrinter() : m_indentation(0) {}

void AstPrinter::Visit(ast::NumberPtr n)
{
	indent();
	std::cout << indentation() << "<Number:" << n->GetNumber() << ">" << std::endl;
	deindent();
}

void AstPrinter::Visit(ast::StringPtr s)
{
	indent();
	std::cout << indentation() << "<String:" << s->GetString() << ">" << std::endl;
	deindent();
}

void AstPrinter::Visit(ast::IdentifierPtr i)
{
	indent();
	std::cout << indentation() << "<Identifier:" << i->GetIdentifier() << ">" << std::endl;
	deindent();
}

void AstPrinter::Visit(ast::BooleanPtr b)
{
	indent();
	std::cout << indentation() << "<Boolean:" << (b->GetBoolean() ? "true" : "false") << ">" << std::endl;
	deindent();
}

void AstPrinter::Visit(ast::Nullptr)
{
	indent();
	std::cout << indentation() << "<Null>" << std::endl;
	deindent();
}

void AstPrinter::Visit(ast::ArithmeticOpPtr a)
{
	static const std::string ops[] = {"+", "-", "*", "/"};
	indent();
	std::cout << indentation() << "<ArithmeticOp:" << ops[static_cast<uint32_t>(a->GetOperationType())] << ">"
	          << std::endl;
	a->GetLhs()->AcceptVisitor(this);
	a->GetRhs()->AcceptVisitor(this);
	deindent();
}

void AstPrinter::Visit(ast::UnaryPtr u)
{
	static const std::string ops[] = {"!", "-"};
	indent();
	std::cout << indentation() << "<Unary:" << ops[static_cast<uint32_t>(u->GetOperationType())] << ">" << std::endl;
	u->GetRhs()->AcceptVisitor(this);
	deindent();
}

void AstPrinter::Visit(ast::ComparisonOpPtr c)
{
	static const std::string ops[] = {"==", "!=", ">", ">=", "<", "<="};
	indent();
	std::cout << indentation() << "<Comparison:" << ops[static_cast<uint32_t>(c->GetOperationType())] << ">"
	          << std::endl;
	c->GetLhs()->AcceptVisitor(this);
	c->GetRhs()->AcceptVisitor(this);
	deindent();
}

void AstPrinter::Visit(ast::LogicOpPtr l)
{
	static const std::string ops[] = {"and", "or"};
	indent();
	std::cout << indentation() << "<Logic:" << ops[static_cast<uint32_t>(l->GetOperationType())] << ">" << std::endl;
	l->GetLhs()->AcceptVisitor(this);
	l->GetRhs()->AcceptVisitor(this);
	deindent();
}

void AstPrinter::Visit(ast::AssignmentPtr a)
{
	indent();
	std::cout << indentation() << "<Assignment>" << std::endl;
	a->GetIdentifier()->AcceptVisitor(this);
	a->GetRhs()->AcceptVisitor(this);
	deindent();
}

void AstPrinter::Visit(ast::ExpressionStatementPtr e)
{
	indent();
	std::cout << indentation() << "<Expression>" << std::endl;
	e->GetExpression()->AcceptVisitor(this);
	deindent();
}

void AstPrinter::Visit(ast::IfStatementPtr i)
{
	indent();
	std::cout << indentation() << "<IfStatement>" << std::endl;
	i->GetCondition()->AcceptVisitor(this);
	i->GetTrueStatement()->AcceptVisitor(this);
	auto falseStatement = i->GetFalseStatement();
	if (falseStatement)
	{
		falseStatement->AcceptVisitor(this);
	}
	deindent();
}

void AstPrinter::Visit(ast::LoopPtr l)
{
	indent();
	std::cout << indentation() << "<Loop>" << std::endl;
	l->GetCondition()->AcceptVisitor(this);
	l->GetBody()->AcceptVisitor(this);
	deindent();
}

void AstPrinter::Visit(ast::VarDeclPtr v)
{
	indent();
	std::cout << indentation() << "<VarDecl>" << std::endl;
	v->GetIdentifier()->AcceptVisitor(this);
	auto rhs = v->GetRhs();
	if (rhs)
	{
		rhs->AcceptVisitor(this);
	}
	deindent();
}

void AstPrinter::Visit(ast::StatementBlockPtr b)
{
	indent();
	std::cout << indentation() << "<StatementBlock>" << std::endl;
	for (auto &s : b->GetStatements())
	{
		s->AcceptVisitor(this);
	}
	deindent();
}

void AstPrinter::Visit(ast::ProgramPtr p)
{
	std::cout << "<Program>" << std::endl;
	for (auto &s : p->GetStatements())
	{
		s->AcceptVisitor(this);
	}
}

void AstPrinter::indent() { ++m_indentation; }

std::string AstPrinter::indentation() { return std::string(m_indentation, ' '); }
void AstPrinter::deindent() { --m_indentation; }
}  // namespace sscript
