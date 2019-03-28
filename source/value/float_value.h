#pragma once

#include "base_value.h"

namespace sscript
{
struct Float : public BaseValue
{
	Float();
	Float(const float& f);
	virtual ~Float();

	std::string ToString() const override;
	void AcceptVisitor(ValueVisitorBase* v) override;

	float m_value;
};
using FloatPtr = std::shared_ptr<Float>;
}  // namespace sscript
