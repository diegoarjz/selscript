#include <binding/functional.h>
#include <value/float_value.h>

#include <gtest/gtest.h>

using namespace sscript;

void f_no_args_no_return() {}
void f_one_arg_no_return(int a) {}
int f_two_args_no_return(int a, float b) { return a + b; }

TEST(make_function, no_args_no_return)
{
	auto f = make_function(f_no_args_no_return);
	ASSERT_TRUE(f);
}

TEST(make_function, one_arg_no_return)
{
	auto f = make_function(f_one_arg_no_return);
	ASSERT_TRUE(f);
}

TEST(make_function, two_args_no_return)
{
	auto f = make_function(f_two_args_no_return);
	ASSERT_TRUE(f);
}

