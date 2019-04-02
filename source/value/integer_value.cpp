#include "integer_value.h"

#include "value_visitor.h"

#include <iostream>

namespace sscript
{
Integer::Integer() : BaseValue("Integer"), m_value(0) {}
Integer::Integer(const int& f) : BaseValue("Integer"), m_value(f) {}
Integer::~Integer() {}

std::string Integer::ToString() const { return std::to_string(m_value); }
void Integer::AcceptVisitor(ValueVisitorBase* v) { v->Visit(this); }
}  // namespace sscript
