#pragma once

#include <memory>
#include <vector>

namespace sscript
{
struct BaseValue;
using BaseValuePtr = std::shared_ptr<BaseValue>;
struct Float;
using FloatPtr = std::shared_ptr<Float>;
struct Integer;
using IntegerPtr = std::shared_ptr<Integer>;

void print(const std::vector<BaseValuePtr>& args);
IntegerPtr time();
}  // namespace sscript
