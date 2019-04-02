#include "interpreter.h"

#include "intermediate/ast_visitor.h"
#include "interpreter_visitor.h"

#include <cstdint>
#include <iostream>
#include <stack>

namespace sscript
{
Interpreter::Interpreter() {}

bool Interpreter::Interpret(const ast::ProgramPtr &program)
{
	interpreter_visitor vis;
	program->AcceptVisitor(&vis);

	return false;
}
}  // namespace sscript
