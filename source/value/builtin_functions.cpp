#include "builtin_functions.h"

#include "float_value.h"

#include <iostream>

namespace sscript
{
void print(const std::vector<BaseValuePtr> &args)
{
	for (auto &a : args)
	{
		std::cout << a->ToString();
	}
	std::cout << std::endl;
}
void print_float(const float &f) { std::cout << "Print float " << f << std::endl; }
void print_things() { std::cout << "things" << std::endl; }

FloatPtr time() { return std::make_shared<Float>(123); }
}  // namespace sscript
