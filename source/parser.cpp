#include "parser.h"

#include "grammar.h"

#include <boost/spirit/include/support_line_pos_iterator.hpp>

namespace sscript
{
Parser::Parser() {}

ast::program Parser::Parse(const std::string &source)
{
	using parser_iterator = boost::spirit::line_pos_iterator<std::string::const_iterator>;
	parser_iterator begin(std::cbegin(source));
	parser_iterator end(std::cend(source));

	grammar<parser_iterator> parser;
	ast::program program;
	bool r = false;

	try
	{
		r = boost::spirit::qi::phrase_parse(begin, end, parser, boost::spirit::qi::space, program);
	}
	catch (boost::spirit::qi::expectation_failure<parser_iterator> const &x)
	{
		r = false;
		std::cout << "expected: something ";
		std::cout << "got: \"" << std::string(x.first, x.last) << '"' << std::endl;
	}

	if (!r && begin != end)
	{
		std::cout << "Failed to parse" << std::endl;
		std::cout << "Last parsed: " << std::string(begin, end) << std::endl;
	}

	return program;
}
}  // namespace sscript
