#pragma once

#include "ast.h"
#include "value.h"

#include <memory>
#include <unordered_map>

namespace sscript
{
class SymbolTable
{
public:
	struct SymbolEntry
	{
		std::string m_symbolName;
		value m_value;
	};

	SymbolTable() = default;
	SymbolTable(std::shared_ptr<SymbolTable> parentScope);
	SymbolTable(const SymbolTable &) = delete;
	SymbolTable &operator=(const SymbolTable &) = delete;

	void AddSymbol(const SymbolEntry &&entry);

	SymbolEntry &GetSymbol(const std::string name);

	void DumpSymbols() const;

private:
	std::unordered_map<std::string, SymbolEntry> m_symbols;
	std::weak_ptr<SymbolTable> m_parentScope;
};
}  // namespace sscript
