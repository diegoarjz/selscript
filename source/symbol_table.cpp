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

void SymbolTable::DumpSymbols() const
{
	struct type_visitor : boost::static_visitor<std::string>
	{
		std::string operator()(const bool& b) const { return "bool"; }
		std::string operator()(const float& f) const { return "float"; }
		std::string operator()(const std::string& s) const { return "string"; }
	};

	struct value_visitor : boost::static_visitor<std::string>
	{
		std::string operator()(const bool& b) const { return b ? "true" : "false"; }
		std::string operator()(const float& f) const { return std::to_string(f); }
		std::string operator()(const std::string& s) const { return s; }
	};
	value_visitor v;
	type_visitor t;

	for (auto symbol : m_symbols)
	{
		std::cout << boost::apply_visitor(t, symbol.second.m_value) << " " << symbol.first << " : "
		          << boost::apply_visitor(v, symbol.second.m_value) << std::endl;
	}
}

}  // namespace sscript
