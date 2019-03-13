#pragma once

#include <string>

namespace sscript
{
struct BaseValue
{
	BaseValue(const std::string &type) : m_typeName(type) {}

	std::string m_typeName;
};

struct Boolean : public BaseValue
{
	Boolean() : BaseValue("Boolean") {}
	Boolean(const bool &b) : BaseValue("Boolean"), m_value(b) {}

	std::string ToString() const { return "<" + m_typeName + ":" + (m_value ? "true" : "false") + ">"; }

	bool m_value;
};

struct Float : public BaseValue
{
	Float() : BaseValue("Float") {}
	Float(const float &f) : BaseValue("Float"), m_value(f) {}

	std::string ToString() const { return "<" + m_typeName + ":" + std::to_string(m_value) + ">"; }

	float m_value;
};

struct String : public BaseValue
{
	String() : BaseValue("String") {}
	String(const std::string &s) : BaseValue("String"), m_value(s) {}

	std::string ToString() const { return "<" + m_typeName + ":\"" + m_value + "\">"; }

	std::string m_value;
};

struct NullObject : public BaseValue
{
	NullObject() : BaseValue("Null") {}

	std::string ToString() const { return "<" + m_typeName + ">"; }
};

using value = boost::variant<Boolean, Float, String, NullObject>;
}  // namespace sscript
