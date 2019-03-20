#pragma once

#include "ast.h"

namespace sscript
{
class Parser
{
public:
	Parser();

	ast::ProgramPtr Parse(const std::string &source);

private:
};
}  // namespace sscript
