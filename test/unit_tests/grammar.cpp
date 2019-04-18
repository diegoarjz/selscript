#include <parser/grammar.h>

#include <gtest/gtest.h>

class GrammarTest : public ::testing::Test
{
protected:
	using iterator = std::string::const_iterator;
	virtual void SetUp() { program = std::make_shared<sscript::ast::Program>(); }

	sscript::grammar<iterator> parser;
	sscript::ast::ProgramPtr program;
};

class IdentifierTest : public GrammarTest, public ::testing::WithParamInterface<std::string>
{
};

TEST_P(IdentifierTest, testIdentifier)
{
	std::string test = GetParam();
	iterator begin = test.begin();
	iterator end = test.end();
	sscript::ast::IdentifierPtr i;

	boost::spirit::qi::phrase_parse(begin, end, parser.identifier, boost::spirit::qi::space, i);
}

TEST_P(IdentifierTest, testIdentifierAsStatement)
{
	std::string test = GetParam() + ";";
	iterator begin = test.begin();
	iterator end = test.end();

	boost::spirit::qi::phrase_parse(begin, end, parser, boost::spirit::qi::space, program);
}

// clang-format off
std::string validIdentifiers[] = {
    "a","_a","_a_", "a_",
    "abc", "a123",
    "true_i", "false_i", "null_i",
    "var_i", "if_i", "else_i", "while_i", "for_i",
    "or_i", "and_i"
};
INSTANTIATE_TEST_SUITE_P(ValidIdentifiers, IdentifierTest,
        ::testing::ValuesIn(validIdentifiers));
// clang-format on
