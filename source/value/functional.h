#pragma once

#include "float_value.h"
#include "integer_value.h"
#include "value_visitor.h"

#include <functional>
#include <iostream>
#include <vector>

namespace sscript
{
template<typename ReturnType, typename... Args>
auto make_function(ReturnType (*p)(Args...)) -> std::function<ReturnType(Args...)>
{
	return {p};
}

template<typename R>
R& argument_value(BaseValuePtr)
{
	return R{};
}

template<>
const float& argument_value<const float&>(BaseValuePtr);

template<typename R, typename... Args>
struct call_function_helper
{
	static R call(std::function<R(Args...)>& f, const std::vector<BaseValuePtr>& args) {}
};

template<typename R>
struct call_function_helper<R, const std::vector<BaseValuePtr>&>
{
	static R call(std::function<R(const std::vector<BaseValuePtr>&)> f, const std::vector<BaseValuePtr>& args)
	{
		return f(args);
	}
};

template<typename R>
struct call_function_helper<R>
{
	static R call(std::function<R(void)> f, const std::vector<BaseValuePtr>& args) { return f(); }
};

template<typename R, typename A1>
struct call_function_helper<R, A1>
{
	static R call(std::function<R(A1)> f, const std::vector<BaseValuePtr>& args)
	{
		auto a1 = argument_value<A1>(args[0]);
		return f(a1);
	}
};

template<typename R, typename... Args>
R call_function(std::function<R(Args...)> f, const std::vector<BaseValuePtr>& args)
{
	return call_function_helper<R, Args...>::call(f, args);
}
}  // namespace sscript
