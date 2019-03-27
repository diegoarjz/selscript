#pragma once

#include <memory>
#include <string>

namespace sscript
{
struct ValueVisitorBase;

struct BaseValue : public std::enable_shared_from_this<BaseValue>
{
	BaseValue(const std::string &type);
	virtual ~BaseValue() {}

	virtual std::string ToString() const = 0;
	virtual void AcceptVisitor(ValueVisitorBase *) = 0;

	std::string m_typeName;
};
using BaseValuePtr = std::shared_ptr<BaseValue>;
}  // namespace sscript
