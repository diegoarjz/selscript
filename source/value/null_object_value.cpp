#include "null_object_value.h"

#include "value_visitor.h"

namespace sscript
{
const TypeInfo NullObject::typeInfo("Null");

NullObject::NullObject() : BaseValue(typeInfo.Name()) {}
NullObject::~NullObject() {}

std::string NullObject::ToString() const { return "null"; }
void NullObject::AcceptVisitor(ValueVisitorBase* v) { v->Visit(this); }
}  // namespace sscript
