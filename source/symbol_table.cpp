#include "symbol_table.h"

namespace sscript
{
SymbolTable::SymbolTable(const std::string& tableName) : m_symbolTableName(tableName) {}

SymbolTable::SymbolTable(const std::string& tableName, std::shared_ptr<SymbolTable> parentScope)
    : m_parentScope(parentScope), m_symbolTableName(tableName)
{
}

void SymbolTable::Declare(const SymbolEntry&& entry)
{
	try
	{
		Get(entry.m_symbolName);
	}
	catch (std::runtime_error e)
	{
		m_symbols[entry.m_symbolName] = entry;
		return;
	}
	throw std::runtime_error("Symbol declaration shadows previous declaration");
}

void SymbolTable::Assign(const std::string& name, const value& v) { Get(name).m_value = v; }

SymbolTable::SymbolEntry& SymbolTable::Get(const std::string& name)
{
	auto iter = m_symbols.find(name);
	if (iter != std::end(m_symbols))
	{
		return iter->second;
	}
	if (!m_parentScope.expired())
	{
		return m_parentScope.lock()->Get(name);
	}
	throw std::runtime_error("Unable to find symbol");
}

void SymbolTable::DumpSymbols() const
{
	value_visitor v;

	std::cout << "Symbols for " << m_symbolTableName << ":" << std::endl;
	for (auto symbol : m_symbols)
	{
		std::cout << symbol.first << ": " << boost::apply_visitor(v, symbol.second.m_value) << std::endl;
	}
}

}  // namespace sscript
