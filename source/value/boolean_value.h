#pragma once

#include "base_value.h"

namespace sscript
{
struct Boolean : public BaseValue
{
	Boolean();
	Boolean(const bool& b);
	virtual ~Boolean();

	std::string ToString() const override;
	void AcceptVisitor(ValueVisitorBase* v) override;

	bool m_value;
};
using BooleanPtr = std::shared_ptr<Boolean>;
}  // namespace sscript
