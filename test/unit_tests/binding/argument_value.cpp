#include <binding/argument_value.h>
#include <value/boolean_value.h>
#include <value/float_value.h>
#include <value/integer_value.h>
#include <value/string_value.h>

#include <gtest/gtest.h>

using namespace sscript;

class ArgumentValue : public ::testing::Test
{
protected:
	void SetUp()
	{
		m_boolean = std::make_shared<Boolean>(true);
		m_float = std::make_shared<Float>(123.0);
		m_integer = std::make_shared<Integer>(123);
		m_string = std::make_shared<String>("abc");
	}

	std::shared_ptr<BaseValue> m_boolean;
	std::shared_ptr<BaseValue> m_float;
	std::shared_ptr<BaseValue> m_integer;
	std::shared_ptr<BaseValue> m_string;
};

TEST_F(ArgumentValue, from_Boolean_to_bool)
{
	auto a = argument_value<bool>(m_boolean);
	ASSERT_EQ(a, true);
}

TEST_F(ArgumentValue, from_Float_to_float)
{
	auto a = argument_value<float>(m_float);
	ASSERT_EQ(a, 123.0);
}

TEST_F(ArgumentValue, from_Integer_to_int)
{
	auto a = argument_value<int>(m_integer);
	ASSERT_EQ(a, 123);
}

TEST_F(ArgumentValue, from_String_to_string)
{
	auto a = argument_value<std::string>(m_string);
	ASSERT_EQ(a, "abc");
}

// TEST_F(ArgumentValue, from_Float_to_int) { argument_value<int>(m_float); }
