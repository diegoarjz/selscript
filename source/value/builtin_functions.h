#pragma once

#include <memory>
#include <vector>

namespace sscript
{
struct BaseValue;
using BaseValuePtr = std::shared_ptr<BaseValue>;
struct Float;
using FloatPtr = std::shared_ptr<Float>;

void print(const std::vector<BaseValuePtr>& args);
FloatPtr time();
}  // namespace sscript
