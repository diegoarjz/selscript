#pragma once

#include "base_value.h"

namespace sscript
{
struct String : public BaseValue
{
	String();
	String(const std::string& s);
	virtual ~String();

	std::string ToString() const override;
	void AcceptVisitor(ValueVisitorBase* v) override;

	std::string m_value;
};
using StringPtr = std::shared_ptr<String>;
}  // namespace sscript
