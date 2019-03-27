#include "null_object_value.h"

#include "value_visitor.h"

namespace sscript
{
NullObject::NullObject() : BaseValue("Null") {}
NullObject::~NullObject() {}

std::string NullObject::ToString() const { return "null"; }
void NullObject::AcceptVisitor(ValueVisitorBase* v) { v->Visit(this); }
}  // namespace sscript
