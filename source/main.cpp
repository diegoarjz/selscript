#include "interpreter.h"
#include "parser.h"

#include <iostream>

#include <fstream>
#include <unordered_map>

#include <boost/bind.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/tuple.hpp>
#include <boost/phoenix/object/construct.hpp>
#include <boost/phoenix/phoenix.hpp>
#include <boost/spirit/include/support_line_pos_iterator.hpp>

using namespace boost::spirit;

int main(int argc, char *argv[])
{
	std::string filePath = argv[1];
	std::cout << "Reading from file " << filePath << std::endl;

	std::ifstream file(filePath.c_str(), std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<char> buffer(size);
	if (!file.read(buffer.data(), size))
	{
		std::cout << "Unable to read file" << std::endl;
	}

	std::string s(std::begin(buffer), std::end(buffer));
	std::cout << "Processing \n" << s << std::endl;

	sscript::Parser p;
	auto program = p.Parse(s);
	std::cout << "Program has " << program.m_declarations.size() << " statements" << std::endl;

	sscript::Interpreter i;
	i.Interpret(program);

	return 0;
}
