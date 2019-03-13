#include "symbol_table.h"

namespace sscript
{
SymbolTable::SymbolTable(std::shared_ptr<SymbolTable> parentScope) : m_parentScope(parentScope) {}

void SymbolTable::AddSymbol(const SymbolTable::SymbolEntry&& entry) { m_symbols[entry.m_symbolName] = entry; }

SymbolTable::SymbolEntry& SymbolTable::GetSymbol(const std::string name)
{
	auto iter = m_symbols.find(name);
	if (iter != std::end(m_symbols))
	{
		return iter->second;
	}
	if (!m_parentScope.expired())
	{
		return m_parentScope.lock()->GetSymbol(name);
	}
	throw std::runtime_error("Unable to find symbol");
}

struct value_visitor : boost::static_visitor<std::string>
{
	template<typename T>
	std::string operator()(const T& v)
	{
		return v.ToString();
	}
};

void SymbolTable::DumpSymbols() const
{
	value_visitor v;

	for (auto symbol : m_symbols)
	{
		std::cout << symbol.first << ": " << boost::apply_visitor(v, symbol.second.m_value) << std::endl;
	}
}

}  // namespace sscript
