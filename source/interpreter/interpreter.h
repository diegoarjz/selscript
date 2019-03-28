#pragma once

#include "symbol_table.h"

#include <memory>

namespace sscript
{
namespace ast
{
class Program;
using ProgramPtr = std::shared_ptr<Program>;
}  // namespace ast

class Interpreter
{
public:
	Interpreter();

	bool Interpret(const ast::ProgramPtr &program);
};
}  // namespace sscript
