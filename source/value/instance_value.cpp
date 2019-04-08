#include "instance_value.h"

#include "interpreter/symbol_table.h"
#include "value_visitor.h"

namespace sscript
{
const TypeInfoPtr Instance::typeInfo = std::make_shared<TypeInfo>("Instance");

Instance::Instance(const ClassPtr& klass)
    : BaseValue(klass->m_type),
      m_class(klass),
      m_symbolTable(std::make_shared<SymbolTable>(klass->m_type->Name(), klass->GetSymbolTable()))
{
}

Instance::~Instance() {}

std::string Instance::ToString() const { return "<Instance:" + m_type->Name() + ">"; }

void Instance::AcceptVisitor(ValueVisitorBase* v) { v->Visit(this); }

BaseValuePtr Instance::GetMember(const std::string& name) { return m_symbolTable->Get(name).m_value; }

}  // namespace sscript
