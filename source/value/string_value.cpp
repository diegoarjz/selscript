#include "string_value.h"

#include "value_visitor.h"

namespace sscript
{
String::String() : BaseValue("String") {}
String::String(const std::string& s) : BaseValue("String"), m_value(s) {}
String::~String() {}

std::string String::ToString() const { return m_value; }
void String::AcceptVisitor(ValueVisitorBase* v) { v->Visit(this); }
}  // namespace sscript
