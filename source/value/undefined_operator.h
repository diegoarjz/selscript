#pragma once

#include "type_info.h"

#include <stdexcept>

namespace sscript
{
class UndefinedBinaryOperatorException : public std::runtime_error
{
public:
	UndefinedBinaryOperatorException(const std::string &op, const TypeInfo &lhsType, const TypeInfo &rhsType);
	virtual ~UndefinedBinaryOperatorException();

	const TypeInfo &GetLhsType() const { return m_lhsType; }
	const TypeInfo &GetRhsType() const { return m_rhsType; }

private:
	std::string m_operatorName;
	const TypeInfo &m_lhsType;
	const TypeInfo &m_rhsType;
};

class UndefinedUnaryOperatorException : public std::runtime_error
{
public:
	UndefinedUnaryOperatorException(const std::string &op, const TypeInfo &operand);
	virtual ~UndefinedUnaryOperatorException();

	const TypeInfo &GetOperandType() const { return m_operandType; }

private:
	std::string m_operatorName;
	const TypeInfo &m_operandType;
};
}  // namespace sscript
