#include "boolean_value.h"

#include "value_visitor.h"

namespace sscript
{
const TypeInfo Boolean::typeInfo("Boolean");

Boolean::Boolean() : BaseValue(typeInfo.Name()), m_value(false) {}
Boolean::Boolean(const bool& b) : BaseValue(typeInfo.Name()), m_value(b) {}
Boolean::~Boolean() {}

std::string Boolean::ToString() const { return (m_value ? "true" : "false"); }
void Boolean::AcceptVisitor(ValueVisitorBase* v) { v->Visit(this); }
}  // namespace sscript
