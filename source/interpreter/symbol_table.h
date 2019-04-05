#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace sscript
{
struct BaseValue;
using BaseValuePtr = std::shared_ptr<BaseValue>;

class SymbolTable
{
public:
	struct SymbolEntry
	{
		std::string m_symbolName;
		BaseValuePtr m_value;
	};

	SymbolTable() = delete;
	SymbolTable(const std::string &tableName);
	SymbolTable(const std::string &tableName, std::shared_ptr<SymbolTable> parentScope);
	SymbolTable(const SymbolTable &) = delete;
	SymbolTable &operator=(const SymbolTable &) = delete;

	void Declare(const SymbolEntry &&entry);
	void Assign(const std::string &name, const BaseValuePtr &v);
	SymbolEntry &Get(const std::string &name);

	void DumpSymbols() const;

private:
	SymbolEntry &findSymbol(const std::string &name);

	std::unordered_map<std::string, SymbolEntry> m_symbols;
	std::weak_ptr<SymbolTable> m_parentScope;
	std::weak_ptr<SymbolTable> m_globals;
	std::string m_symbolTableName;
};

class SymbolNotFoundException : public std::exception
{
public:
	SymbolNotFoundException(const std::string &symbolName) : m_symbolName(symbolName) {}

	const char *what() const throw() { return "Symbol referenced before declaration."; }
	const std::string &GetSymbolName() const { return m_symbolName; }

private:
	std::string m_symbolName;
};

class SymbolShadowingException : public std::exception
{
public:
	SymbolShadowingException(const std::string &symbolName, const SymbolTable::SymbolEntry &prevDeclaration)
	    : m_symbolName(symbolName), m_previousDeclaration(prevDeclaration)
	{
	}

	const char *what() const throw() { return "Symbol declaration shadows previous declaration."; }
	const std::string &GetSymbolName() const { return m_symbolName; }
	const SymbolTable::SymbolEntry &GetPreviousDeclaration() const { return m_previousDeclaration; }

private:
	std::string m_symbolName;
	const SymbolTable::SymbolEntry m_previousDeclaration;
};
}  // namespace sscript
