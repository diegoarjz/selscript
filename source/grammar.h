#pragma once

#include "ast.h"

#include <boost/phoenix/object/construct.hpp>
#include <boost/phoenix/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>

using namespace boost::spirit::qi;

BOOST_FUSION_ADAPT_STRUCT(sscript::ast::identifier, (std::string, m_identifier))
BOOST_FUSION_ADAPT_STRUCT(sscript::ast::number, (float, m_number))
BOOST_FUSION_ADAPT_STRUCT(sscript::ast::string, (std::string, m_string))
BOOST_FUSION_ADAPT_STRUCT(sscript::ast::null)
BOOST_FUSION_ADAPT_STRUCT(sscript::ast::arithmetic_op, (sscript::ast::arithmetic_op::types, m_operation),
                          (sscript::ast::expression, m_lhs), (sscript::ast::expression, m_rhs))
BOOST_FUSION_ADAPT_STRUCT(sscript::ast::comparison_op, (sscript::ast::comparison_op::types, m_operation),
                          (sscript::ast::expression, m_lhs), (sscript::ast::expression, m_rhs))
BOOST_FUSION_ADAPT_STRUCT(sscript::ast::logic_op, (sscript::ast::logic_op::types, m_operation),
                          (sscript::ast::expression, m_lhs), (sscript::ast::expression, m_rhs))
BOOST_FUSION_ADAPT_STRUCT(sscript::ast::assignment, (sscript::ast::identifier, m_variableName),
                          (sscript::ast::expression, m_rhs))
BOOST_FUSION_ADAPT_STRUCT(sscript::ast::statement, (sscript::ast::statement_types, m_statement))
BOOST_FUSION_ADAPT_STRUCT(sscript::ast::var_decl, (sscript::ast::identifier, m_variableName),
                          (boost::optional<sscript::ast::expression>, m_rhs))
BOOST_FUSION_ADAPT_STRUCT(sscript::ast::declaration, (sscript::ast::declaration_types, m_declaration))
BOOST_FUSION_ADAPT_STRUCT(sscript::ast::program, (std::vector<sscript::ast::declaration>, m_declarations))

namespace sscript
{
ast::unary make_unary_op(char op, const ast::expression &rhs)
{
	switch (op)
	{
		case '!':
			return ast::unary(ast::unary::types::Neg, rhs);
		case '-':
			return ast::unary(ast::unary::types::Min, rhs);
	}
	throw std::runtime_error("Invalid operator");
}

ast::arithmetic_op make_arithmetic_op(char op, const ast::expression &lhs, const ast::expression &rhs)
{
	switch (op)
	{
		case '+':
			return ast::arithmetic_op(ast::arithmetic_op::types::Add, lhs, rhs);
		case '-':
			return ast::arithmetic_op(ast::arithmetic_op::types::Sub, lhs, rhs);
		case '*':
			return ast::arithmetic_op(ast::arithmetic_op::types::Mul, lhs, rhs);
		case '/':
			return ast::arithmetic_op(ast::arithmetic_op::types::Div, lhs, rhs);
	}
	throw std::runtime_error("Invalid operator");
}

ast::comparison_op make_comparison_op(const std::string &op, const ast::expression &lhs, const ast::expression &rhs)
{
	if (op == "==")
	{
		return ast::comparison_op(ast::comparison_op::types::Eq, lhs, rhs);
	}
	if (op == "!=")
	{
		return ast::comparison_op(ast::comparison_op::types::Ne, lhs, rhs);
	}
	if (op == ">")
	{
		return ast::comparison_op(ast::comparison_op::types::Gt, lhs, rhs);
	}
	if (op == ">=")
	{
		return ast::comparison_op(ast::comparison_op::types::Gte, lhs, rhs);
	}
	if (op == "<")
	{
		return ast::comparison_op(ast::comparison_op::types::Lt, lhs, rhs);
	}
	if (op == "<=")
	{
		return ast::comparison_op(ast::comparison_op::types::Lte, lhs, rhs);
	}
	throw std::runtime_error("Invalid operator");
}

ast::logic_op make_logic_op(const std::string &op, const ast::expression &lhs, const ast::expression &rhs)
{
	if (op == "and")
	{
		return ast::logic_op(ast::logic_op::types::And, lhs, rhs);
	}
	if (op == "or")
	{
		return ast::logic_op(ast::logic_op::types::Or, lhs, rhs);
	}
	throw std::runtime_error("Invalid operator");
}

ast::null make_null() { return ast::null(); }

template<class Iterator>
struct grammar : boost::spirit::qi::grammar<Iterator, ast::program(), boost::spirit::qi::space_type>
{
	grammar() : grammar::base_type(program)
	{
		using namespace boost::spirit;
		using namespace boost::phoenix;
		using boost::phoenix::construct;

		identifier %= ((char_('_') | alpha) >> *(alnum | char_('_')));
		number = float_[_val = construct<ast::number>(qi::_1)];
		string = (('"' >> *(char_ - '"')) > '"')[_val = construct<ast::string>(qi::_1)];

		/*
		 * program -> declaration* EOF
		 */
		program %= *declaration >> eoi;

		/*
		 * declaration -> var_decl | statement
		 */
		declaration %= var_decl | statement;

		/*
		 * var_decl -> "var" IDENTIFIER ( "=" expression )? ";"
		 */
		var_decl %= "var" > identifier >> -('=' > expression) > ';';

		/*
		 * statement -> expression_statement
		 */
		statement %= expression_statement;

		/*
		 * expression_statement -> expression ";"
		 */
		expression_statement %= expression > ';';

		/*
		 * expression -> assignment;
		 */
		expression %= assignment;

		/*
		 * assignment -> identifier "=" assignment | logic_or
		 */
		assignment %= (identifier >> '=' >> assignment) | logic_or;

		/*
		 * logic_or -> logic_and ( "or" logic_and )*
		 */
		logic_or = logic_and[_val = qi::_1] >>
		           *((qi::string("or") >> logic_and)[_val = boost::phoenix::bind(make_logic_op, qi::_1, _val, qi::_2)]);

		/*
		 * logic_and -> equality ( "and" equality )*
		 */
		logic_and =
		    equality[_val = qi::_1] >>
		    *((qi::string("and") >> equality)[_val = boost::phoenix::bind(make_logic_op, qi::_1, _val, qi::_2)]);

		/*
		 * equality -> comparison (("==" | "!=") comparison )*
		 */
		equality = comparison[_val = qi::_1] >>
		           *(((qi::string("==") | qi::string("!=")) >>
		              comparison)[_val = boost::phoenix::bind(make_comparison_op, qi::_1, _val, qi::_2)]);

		/*
		 * comparison -> addition ((">" | ">=" | "<" | "<=") addition)*
		 */
		comparison = addition[_val = qi::_1] >>
		             *(((qi::string(">") | qi::string(">=") | qi::string("<") | qi::string("<=")) >>
		                addition)[_val = boost::phoenix::bind(make_comparison_op, qi::_1, _val, qi::_2)]);

		/*
		 * addition -> multiplication (("+"|"-") multiplication)*
		 */
		addition = multiplication[_val = qi::_1] >>
		           *(((char_('+') | char_('-')) >>
		              multiplication)[_val = boost::phoenix::bind(make_arithmetic_op, qi::_1, _val, qi::_2)]);

		/*
		 * multiplication -> unary (("*"|"/") unary)*
		 */
		multiplication =
		    unary[_val = qi::_1] >> *(((char_('*') | char_('/')) >>
		                               unary)[_val = boost::phoenix::bind(make_arithmetic_op, qi::_1, _val, qi::_2)]);

		/*
		 * unary -> (("!"|"-") unary) | primary
		 */
		unary = ((char_('!') | char_('-')) >> unary)[_val = boost::phoenix::bind(make_unary_op, qi::_1, qi::_2)] |
		        primary[_val = qi::_1];

		/*
		 * primary -> "true" | "false" | "null" | number | string | identifier
		 *            | "(" expression ")"
		 */
		primary = qi::lit("true")[_val = true] | qi::lit("false")[_val = false] |
		          qi::lit("null")[_val = boost::phoenix::bind(make_null)] |
		          (number | string | identifier)[_val = qi::_1] | ('(' >> expression > ')');
	}

	rule<Iterator, ast::identifier(), space_type> identifier;
	rule<Iterator, ast::number(), space_type> number;
	rule<Iterator, ast::string(), space_type> string;

	rule<Iterator, ast::program(), space_type> program;
	rule<Iterator, ast::declaration(), space_type> declaration;
	rule<Iterator, ast::var_decl(), space_type> var_decl;
	rule<Iterator, ast::statement(), space_type> statement;
	rule<Iterator, ast::expression(), space_type> expression;
	rule<Iterator, ast::expression(), space_type> expression_statement;
	rule<Iterator, ast::expression(), space_type> assignment;
	rule<Iterator, ast::expression(), space_type> logic_or;
	rule<Iterator, ast::expression(), space_type> logic_and;
	rule<Iterator, ast::expression(), space_type> equality;
	rule<Iterator, ast::expression(), space_type> comparison;
	rule<Iterator, ast::expression(), space_type> addition;
	rule<Iterator, ast::expression(), space_type> multiplication;
	rule<Iterator, ast::expression(), space_type> unary;
	rule<Iterator, ast::expression(), space_type> primary;
};
}  // namespace sscript
