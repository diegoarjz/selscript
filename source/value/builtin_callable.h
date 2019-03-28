#pragma once

#include "callable.h"
#include "functional.h"

namespace sscript
{
template<typename R, typename... Args>
struct return_from_function_helper
{
	static void call_and_return(std::function<R(Args...)> function, const std::vector<BaseValuePtr>& args)
	{
		throw static_cast<BaseValuePtr>(call_function(function, args));
	}
};

template<typename... Args>
struct return_from_function_helper<void, Args...>
{
	static void call_and_return(std::function<void(Args...)> function, const std::vector<BaseValuePtr>& args)
	{
		call_function(function, args);
	}
};

template<typename R, typename... Args>
void return_from_function(std::function<R(Args...)> function, const std::vector<BaseValuePtr>& args)
{
	return_from_function_helper<R, Args...>::call_and_return(function, args);
}

template<typename FunctionType>
struct BuiltinCallable : public Callable
{
public:
	BuiltinCallable(FunctionType function) : m_function(function) {}
	virtual ~BuiltinCallable() {}

	void Call(const std::vector<BaseValuePtr>& args) { return_from_function(m_function, args); }

private:
	FunctionType m_function;
};

template<typename R, typename... Args>
std::shared_ptr<BuiltinCallable<std::function<R(Args...)>>> register_callable(std::function<R(Args...)> function)
{
	return std::make_shared<BuiltinCallable<std::function<R(Args...)>>>(function);
}

}  // namespace sscript
