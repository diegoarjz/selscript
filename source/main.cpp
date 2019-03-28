#include "intermediate/ast_printer.h"
#include "interpreter/interpreter.h"
#include "parser/parser.h"

#include <boost/program_options.hpp>

#include <fstream>
#include <string>

sscript::ast::ProgramPtr parse_script(const std::string &filePath);
void dump_ast(sscript::ast::ProgramPtr program);
void run(sscript::ast::ProgramPtr program);

int main(int argc, char *argv[])
{
	namespace po = boost::program_options;

	std::string scriptFile;

	po::options_description desc("sscript");
	// clang-format off
    desc.add_options()
        ("script", po::value<std::string>(&scriptFile)->required(), "Script file to run")
        ("run", "Run the script")
        ("dump-ast", "Dump the AST to console")
        ("help", "Print help message");
	// clang-format on

	po::positional_options_description positionalOptions;
	// clang-format off
    positionalOptions.add("script", 1);
	// clang-format on

	po::variables_map vm;
	try
	{
		po::store(po::command_line_parser(argc, argv).options(desc).positional(positionalOptions).run(), vm);

		if (vm.count("help"))
		{
			std::cout << "sscript usage:" << std::endl << desc << std::endl;
			return 0;
		}

		po::notify(vm);
	}
	catch (po::error &e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
		std::cerr << desc << std::endl;
		return 1;
	}

	auto program = parse_script(scriptFile);

	if (vm.count("run"))
	{
		run(program);
	}

	if (vm.count("dump-ast"))
	{
		dump_ast(program);
	}

	return 0;
}

sscript::ast::ProgramPtr parse_script(const std::string &filePath)
{
	std::ifstream file(filePath.c_str(), std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<char> buffer(size);
	if (!file.read(buffer.data(), size))
	{
		std::cout << "Unable to read file" << std::endl;
	}

	std::string s(std::begin(buffer), std::end(buffer));

	sscript::Parser p;
	return p.Parse(s);
}

void dump_ast(sscript::ast::ProgramPtr program)
{
	sscript::AstPrinter printer;
	printer.Visit(program);
}

void run(sscript::ast::ProgramPtr program)
{
	sscript::Interpreter i;
	i.Interpret(program);
}
