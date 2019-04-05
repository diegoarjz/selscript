#include "float_value.h"

#include "boolean_value.h"
#include "integer_value.h"
#include "value_visitor.h"

namespace sscript
{
const TypeInfo Float::typeInfo("Float");

Float::Float() : BaseValue(typeInfo.Name()), m_value(0) {}
Float::Float(const float& f) : BaseValue(typeInfo.Name()), m_value(f) {}
Float::~Float() {}

std::string Float::ToString() const { return std::to_string(m_value); }
void Float::AcceptVisitor(ValueVisitorBase* v) { v->Visit(this); }

std::shared_ptr<Float> Float::operator+(const Float& f) const { return std::make_shared<Float>(m_value + f.m_value); }
std::shared_ptr<Float> Float::operator-(const Float& f) const { return std::make_shared<Float>(m_value - f.m_value); }
std::shared_ptr<Float> Float::operator*(const Float& f) const { return std::make_shared<Float>(m_value * f.m_value); }
std::shared_ptr<Float> Float::operator/(const Float& f) const { return std::make_shared<Float>(m_value / f.m_value); }

std::shared_ptr<Boolean> Float::operator==(const Float& f) const
{
	return std::make_shared<Boolean>(m_value == f.m_value);
}
std::shared_ptr<Boolean> Float::operator!=(const Float& f) const
{
	return std::make_shared<Boolean>(m_value != f.m_value);
}
std::shared_ptr<Boolean> Float::operator<(const Float& f) const
{
	return std::make_shared<Boolean>(m_value < f.m_value);
}
std::shared_ptr<Boolean> Float::operator<=(const Float& f) const
{
	return std::make_shared<Boolean>(m_value <= f.m_value);
}
std::shared_ptr<Boolean> Float::operator>(const Float& f) const
{
	return std::make_shared<Boolean>(m_value > f.m_value);
}
std::shared_ptr<Boolean> Float::operator>=(const Float& f) const
{
	return std::make_shared<Boolean>(m_value >= f.m_value);
}

std::shared_ptr<Float> Float::operator+(const Integer& i) const { return std::make_shared<Float>(m_value + i.m_value); }
std::shared_ptr<Float> Float::operator-(const Integer& i) const { return std::make_shared<Float>(m_value - i.m_value); }
std::shared_ptr<Float> Float::operator*(const Integer& i) const { return std::make_shared<Float>(m_value * i.m_value); }
std::shared_ptr<Float> Float::operator/(const Integer& i) const { return std::make_shared<Float>(m_value / i.m_value); }

std::shared_ptr<Boolean> Float::operator==(const Integer& f) const
{
	return std::make_shared<Boolean>(m_value == f.m_value);
}
std::shared_ptr<Boolean> Float::operator!=(const Integer& f) const
{
	return std::make_shared<Boolean>(m_value != f.m_value);
}
std::shared_ptr<Boolean> Float::operator<(const Integer& f) const
{
	return std::make_shared<Boolean>(m_value < f.m_value);
}
std::shared_ptr<Boolean> Float::operator<=(const Integer& f) const
{
	return std::make_shared<Boolean>(m_value <= f.m_value);
}
std::shared_ptr<Boolean> Float::operator>(const Integer& f) const
{
	return std::make_shared<Boolean>(m_value > f.m_value);
}
std::shared_ptr<Boolean> Float::operator>=(const Integer& f) const
{
	return std::make_shared<Boolean>(m_value >= f.m_value);
}
}  // namespace sscript
