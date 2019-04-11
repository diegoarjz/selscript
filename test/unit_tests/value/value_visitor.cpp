#include <value/boolean_value.h>
#include <value/class_value.h>
#include <value/float_value.h>
#include <value/function.h>
#include <value/instance_value.h>
#include <value/integer_value.h>
#include <value/null_object_value.h>
#include <value/string_value.h>
#include <value/type_info.h>
#include <value/value_visitor.h>

#include <gtest/gtest.h>

using namespace sscript;

template<class Match>
struct Visitor : public ValueVisitor<void>
{
	using MatchBaseType = typename std::remove_reference<Match>::type;

	void operator()(Match r) {}

	template<class NoMatch>
	void operator()(NoMatch &m)
	{
		std::cout << m.m_type->Name() << std::endl;
		FAIL() << "Couldn't match visitor. Expected " << MatchBaseType::typeInfo->Name();
	}
};

class VisitorTests : public ::testing::Test
{
protected:
	void SetUp() { m_float = std::make_shared<Float>(123.0); }

	std::shared_ptr<BaseValue> m_float;
};

TEST_F(VisitorTests, visit_float)
{
	Visitor<Float> v;
	apply_visitor(v, m_float);
}

TEST_F(VisitorTests, visit_reference_float)
{
	Visitor<Float> rv;
	auto &rf = m_float;
	apply_visitor(rv, rf);
}

TEST_F(VisitorTests, visit_const_reference_float)
{
	Visitor<Float> crv;
	const auto &crf = m_float;
	apply_visitor(crv, crf);
}

TEST_F(VisitorTests, value_as_float)
{
	auto v = value_as<Float>(m_float);
	ASSERT_EQ(v.m_value, 123.0);
	auto rv = value_as<Float &>(m_float);
	ASSERT_EQ(rv.m_value, 123.0);
}

TEST_F(VisitorTests, value_as_integer)
{
	try
	{
		auto v = value_as<Integer>(m_float);
	}
	catch (std::exception &e)
	{
		// should catch
		return;
	}
	FAIL() << "Should have thrown an exception";
}

