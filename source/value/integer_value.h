#pragma once

#include "base_value.h"

namespace sscript
{
struct Integer : public BaseValue
{
	Integer();
	Integer(const int& f);
	virtual ~Integer();

	std::string ToString() const override;
	void AcceptVisitor(ValueVisitorBase* v) override;

	int m_value;
};
using IntegerPtr = std::shared_ptr<Integer>;
}  // namespace sscript
