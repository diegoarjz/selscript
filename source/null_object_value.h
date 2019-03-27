#pragma once

#include "base_value.h"

namespace sscript
{
struct NullObject : public BaseValue
{
	NullObject();
	virtual ~NullObject();

	std::string ToString() const override;
	void AcceptVisitor(ValueVisitorBase* v) override;
};
using NullObjectPtr = std::shared_ptr<NullObject>;

}  // namespace sscript
