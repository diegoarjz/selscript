#pragma once

#include "ast.h"
#include "symbol_table.h"

#include <memory>

namespace sscript
{
class Interpreter
{
public:
	Interpreter();

	bool Interpret(const ast::ProgramPtr &program);
};
}  // namespace sscript
