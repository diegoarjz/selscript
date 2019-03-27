#pragma once

#include "base_value.h"
#include "float_value.h"
#include "functional.h"

#include <vector>

namespace sscript
{
struct Callable : public BaseValue
{
	Callable();
	virtual ~Callable();

	std::string ToString() const override;
	void AcceptVisitor(ValueVisitorBase*) override;

	virtual void Call(const std::vector<BaseValuePtr>& args);
};
using CallablePtr = std::shared_ptr<Callable>;

}  // namespace sscript
