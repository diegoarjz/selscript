#pragma once

#include "grammar_helpers.h"
#include "intermediate/ast.h"

#include <boost/phoenix/object/construct.hpp>
#include <boost/phoenix/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>

using namespace boost::spirit::qi;

namespace sscript
{
template<class Iterator>
struct grammar : boost::spirit::qi::grammar<Iterator, ast::ProgramPtr(), boost::spirit::qi::space_type>
{
	static auto kw(char const *keyword) -> boost::spirit::qi::rule<Iterator, void(), boost::spirit::qi::space_type>
	{
		using namespace boost::spirit;
		return lexeme[(qi::string(+keyword) >> !(qi::alnum | qi::char_('_')))];
	}

	static auto typed_kw(char const *keyword)
	    -> boost::spirit::qi::rule<Iterator, std::string(), boost::spirit::qi::space_type>
	{
		using namespace boost::spirit;
		return lexeme[(qi::string(keyword) >> !(qi::alnum | qi::char_('_')))[_val = qi::_1]];
	}

	grammar() : grammar::base_type(program)
	{
		using namespace boost::spirit;
		using boost::phoenix::bind;
		using boost::phoenix::construct;

		kw_true = kw("true");
		kw_false = kw("false");
		kw_null = kw("null");
		kw_var = kw("var");
		kw_if = kw("if");
		kw_else = kw("else");
		kw_while = kw("while");
		kw_for = kw("for");
		kw_or = typed_kw("or");
		kw_and = typed_kw("and");

		identifier = lexeme[(+(char_('_') | alpha) >> *(alnum | char_('_')))[_val = bind(make_identifier, qi::_1)]];
		number = float_[_val = bind(make_number, qi::_1)];
		string = lexeme[(('"' >> *(char_ - '"')) > '"')[_val = bind(make_string, qi::_1)]];

		/*
		 * program -> declaration* EOF
		 */
		program = (*declaration >> eoi)[_val = bind(make_program, qi::_1)];

		/*
		 * declaration -> var_decl | statement
		 */
		declaration = var_decl | statement;

		/*
		 * var_decl -> "var" IDENTIFIER ( "=" expression )? ";"
		 */
		var_decl = ("var" > identifier >> -('=' > expression) > ';')[_val = bind(make_var_decl, qi::_1, qi::_2)];

		/*
		 * statement -> expression_statement | if_statement | block
		 */
		statement = if_statement | while_loop | for_loop | expression_statement | statement_block;

		/*
		 * statement_block -> "{" declaration* "}"
		 */
		statement_block = qi::char_('{')[_val = bind(make_statement_block)] >>
		                  *(declaration[bind(add_statement, _val, qi::_1)]) >> '}';

		/*
		 * expression_statement -> expression ";"
		 */
		expression_statement = (expression[_val = bind(make_expression_statment, qi::_1)] > ';');

		/*
		 * if_statement -> "if" "(" expression ")" statement ("else" statement)?
		 */
		if_statement =
		    (qi::lit("if") >> '(' >> expression >> ')' >> statement)[_val = bind(make_if_statement, qi::_1, qi::_2)] >>
		    -(qi::lit("else") >> statement[_val = bind(add_else_statement, _val, qi::_1)]);

		/*
		 *  while_loop -> "while" "(" expression ")" statement
		 */
		while_loop =
		    (qi::lit("while") >> '(' >> expression >> ')' >> statement)[_val = bind(make_while_loop, qi::_1, qi::_2)];

		/*
		 * for_loop -> "for" "(" (var_decl | expression | ";")? expression? ";" expression? ")" statement
		 */
		for_loop = (qi::lit("for") >> '(' >> (var_decl | expression_statement | ';') >> -expression >> ';' >>
		            -expression >> ')' >> statement)[_val = bind(make_for_loop, qi::_1, qi::_2, qi::_3, qi::_4)];

		/*
		 * expression -> assignment;
		 */
		expression = assignment[_val = bind(make_expression, qi::_1)];

		/*
		 * assignment -> identifier "=" assignment | logic_or
		 */
		assignment =
		    (identifier >> '=' >> assignment)[_val = bind(make_assignment, qi::_1, qi::_2)] | logic_or[_val = qi::_1];

		/*
		 * logic_or -> logic_and ( "or" logic_and )*
		 */
		logic_or = logic_and[_val = qi::_1] >>
		           *((qi::string("or") >> logic_and)[_val = bind(make_logic_op, qi::_1, _val, qi::_2)]);

		/*
		 * logic_and -> equality ( "and" equality )*
		 */
		logic_and = equality[_val = qi::_1] >>
		            *((qi::string("and") >> equality)[_val = bind(make_logic_op, qi::_1, _val, qi::_2)]);

		/*
		 * equality -> comparison (("==" | "!=") comparison )*
		 */
		equality = comparison[_val = qi::_1] >> *(((qi::string("==") | qi::string("!=")) >>
		                                           comparison)[_val = bind(make_comparison_op, qi::_1, _val, qi::_2)]);

		/*
		 * comparison -> addition ((">" | ">=" | "<" | "<=") addition)*
		 */
		comparison =
		    addition[_val = qi::_1] >> *(((qi::string(">=") | qi::string(">") | qi::string("<=") | qi::string("<")) >>
		                                  addition)[_val = bind(make_comparison_op, qi::_1, _val, qi::_2)]);

		/*
		 * addition -> multiplication (("+"|"-") multiplication)*
		 */
		addition =
		    multiplication[_val = qi::_1] >>
		    *(((char_('+') | char_('-')) >> multiplication)[_val = bind(make_arithmetic_op, qi::_1, _val, qi::_2)]);

		/*
		 * multiplication -> unary (("*"|"/") unary)*
		 */
		multiplication = unary[_val = qi::_1] >>
		                 *(((char_('*') | char_('/')) >> unary)[_val = bind(make_arithmetic_op, qi::_1, _val, qi::_2)]);

		/*
		 * unary -> (("!"|"-") unary) | call
		 */
		unary = ((char_('!') | char_('-')) >> unary)[_val = bind(make_unary_op, qi::_1, qi::_2)] | call[_val = qi::_1];

		/*
		 * call -> primary ( "(" arguments? ")" )*
		 */
		call = (primary[_val = qi::_1] >> *('(' >> (-arguments)[_val = bind(make_call, _val, qi::_1)] >> ')'));

		/*
		 * arguments -> expression ( "," expression )*
		 */
		arguments = expression % ',';

		/*
		 * primary -> "true" | "false" | "null" | number | string | identifier
		 *            | "(" expression ")"
		 */
		primary = kw_true[_val = bind(make_true)] | kw_false[_val = bind(make_false)] |
		          kw_null[_val = bind(make_null)] |
		          (number[_val = qi::_1] | string[_val = qi::_1] | identifier[_val = qi::_1]) |
		          ('(' >> expression[_val = qi::_1] > ')');
	}

	rule<Iterator, void(), space_type> kw_true;
	rule<Iterator, void(), space_type> kw_false;
	rule<Iterator, void(), space_type> kw_null;
	rule<Iterator, void(), space_type> kw_var;
	rule<Iterator, void(), space_type> kw_if;
	rule<Iterator, void(), space_type> kw_else;
	rule<Iterator, void(), space_type> kw_while;
	rule<Iterator, void(), space_type> kw_for;
	rule<Iterator, std::string(), space_type> kw_or;
	rule<Iterator, std::string(), space_type> kw_and;

	rule<Iterator, ast::IdentifierPtr(), space_type> identifier;
	rule<Iterator, ast::NumberPtr(), space_type> number;
	rule<Iterator, ast::StringPtr(), space_type> string;

	rule<Iterator, ast::ProgramPtr(), space_type> program;
	rule<Iterator, ast::StatementPtr(), space_type> declaration;
	rule<Iterator, ast::VarDeclPtr(), space_type> var_decl;
	rule<Iterator, ast::StatementPtr(), space_type> statement;
	rule<Iterator, ast::StatementBlockPtr(), space_type> statement_block;
	rule<Iterator, ast::IfStatementPtr(), space_type> if_statement;
	rule<Iterator, ast::LoopPtr(), space_type> while_loop;
	rule<Iterator, ast::StatementPtr(), space_type> for_loop;
	rule<Iterator, ast::ExpressionPtr(), space_type> expression;
	rule<Iterator, ast::ExpressionStatementPtr(), space_type> expression_statement;
	rule<Iterator, ast::ExpressionPtr(), space_type> assignment;
	rule<Iterator, ast::ExpressionPtr(), space_type> logic_or;
	rule<Iterator, ast::ExpressionPtr(), space_type> logic_and;
	rule<Iterator, ast::ExpressionPtr(), space_type> equality;
	rule<Iterator, ast::ExpressionPtr(), space_type> comparison;
	rule<Iterator, ast::ExpressionPtr(), space_type> addition;
	rule<Iterator, ast::ExpressionPtr(), space_type> multiplication;
	rule<Iterator, ast::ExpressionPtr(), space_type> unary;
	rule<Iterator, ast::ExpressionPtr(), space_type> call;
	rule<Iterator, ast::ExpressionPtr(), space_type> primary;
	rule<Iterator, std::vector<ast::ExpressionPtr>(), space_type> arguments;
};
}  // namespace sscript
