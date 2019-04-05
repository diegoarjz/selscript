#pragma once

#include <string>

namespace sscript
{
class TypeInfo
{
public:
	explicit TypeInfo(const std::string& name);

	const std::string& Name() const { return m_typeName; }

private:
	std::string m_typeName;
};
}  // namespace sscript
