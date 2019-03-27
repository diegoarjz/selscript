#include "float_value.h"

#include "value_visitor.h"

namespace sscript
{
Float::Float() : BaseValue("Float"), m_value(0) {}
Float::Float(const float& f) : BaseValue("Float"), m_value(f) {}
Float::~Float() {}

std::string Float::ToString() const { return std::to_string(m_value); }
void Float::AcceptVisitor(ValueVisitorBase* v) { v->Visit(this); }
}  // namespace sscript
