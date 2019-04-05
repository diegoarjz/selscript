#include "undefined_operator.h"

namespace sscript
{
UndefinedBinaryOperatorException::UndefinedBinaryOperatorException(const std::string &op, const TypeInfo &lhsType,
                                                                   const TypeInfo &rhsType)
    : std::runtime_error("Undefined Binary Operator " + op + " between " + lhsType.Name() + " and " + rhsType.Name()),
      m_operatorName(op),
      m_lhsType(lhsType),
      m_rhsType(rhsType)
{
}

UndefinedBinaryOperatorException::~UndefinedBinaryOperatorException() {}

UndefinedUnaryOperatorException::UndefinedUnaryOperatorException(const std::string &op, const TypeInfo &operand)
    : std::runtime_error("Undefined Unary Operator " + op + " for " + operand.Name()),
      m_operatorName(op),
      m_operandType(operand)
{
}

UndefinedUnaryOperatorException::~UndefinedUnaryOperatorException() {}
}  // namespace sscript
