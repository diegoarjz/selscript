#pragma once

#include <memory>
#include <string>

namespace sscript
{
namespace ast
{
class Program;
using ProgramPtr = std::shared_ptr<Program>;
}  // namespace ast

class SymbolTable;

class Interpreter
{
public:
	Interpreter();
	~Interpreter();

	bool Interpret(const ast::ProgramPtr &program);

	void PushExternalSymbols(std::shared_ptr<SymbolTable> &externalSymbols);
	void PopExternalSymbols();

private:
	class Impl;
	std::unique_ptr<Impl> m_implementation;
};
}  // namespace sscript
