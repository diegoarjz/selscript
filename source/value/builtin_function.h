#pragma once

#include "function.h"
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
struct BuiltinFunction : public Function
{
public:
	BuiltinFunction(FunctionType function) : m_function(function) {}
	virtual ~BuiltinFunction() {}

	void Call(interpreter_visitor* interpreter, const std::vector<BaseValuePtr>& args)
	{
		return_from_function(m_function, args);
	}

private:
	FunctionType m_function;
};

template<typename R, typename... Args>
struct register_callable_helper
{
	static std::shared_ptr<BuiltinFunction<std::function<R(Args...)>>> register_callable(
	    const std::string& functionName, std::function<R(Args...)> function)
	{
		auto callable = std::make_shared<BuiltinFunction<std::function<R(Args...)>>>(function);
		callable->SetArity(sizeof...(Args));
		callable->SetCallableName(functionName);

		return callable;
	}
};

template<typename R>
struct register_callable_helper<R, const std::vector<BaseValuePtr>&>
{
	static std::shared_ptr<BuiltinFunction<std::function<R(const std::vector<BaseValuePtr>&)>>> register_callable(
	    const std::string& functionName, std::function<R(const std::vector<BaseValuePtr>&)> function)
	{
		auto callable = std::make_shared<BuiltinFunction<std::function<R(const std::vector<BaseValuePtr>&)>>>(function);
		callable->SetCallableName(functionName);
		callable->SetVariadic(true);

		return callable;
	}
};

template<typename R, typename... Args>
std::shared_ptr<BuiltinFunction<std::function<R(Args...)>>> register_callable(const std::string& functionName,
                                                                              std::function<R(Args...)> function)
{
	return register_callable_helper<R, Args...>::register_callable(functionName, function);
}

}  // namespace sscript
