#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "Tokenizer.h"
#include "ParserVisitor.h"
#include "Stringify.h"
#include "RpnCalculator.h"

using num = NumberToken;
static const auto plus = OperationToken{'+'};
static const auto minus = OperationToken{'-'};
static const auto mul = OperationToken{'*'};
static const auto divide = OperationToken{'/'};
static const auto l = BraceToken{'('};
static const auto r = BraceToken{')'};

bool cmp(Token lhs, Token rhs) {
	if (lhs.holds_type<OperationToken>() && rhs.holds_type<OperationToken>()) {
		auto l = lhs.holds_type<OperationToken>();
		auto r = rhs.holds_type<OperationToken>();
		return l->ch == r->ch;
	}
	if (lhs.holds_type<BraceToken>() && rhs.holds_type<BraceToken>()) {
		auto l = lhs.holds_type<BraceToken>();
		auto r = rhs.holds_type<BraceToken>();
		return l->ch == r->ch;
	}
	if (lhs.holds_type<NumberToken>() && rhs.holds_type<NumberToken>()) {
		auto l = lhs.holds_type<NumberToken>();
		auto r = rhs.holds_type<NumberToken>();
		return l->value == r->value;
	}
	return false;
}

bool cmp(std::vector<Token> const& lhs, std::vector<Token> const& rhs) {
	if (lhs.size() != rhs.size()) return false;
	for (size_t i = 0; i < lhs.size(); ++i) {
		if (!cmp(lhs[i], rhs[i])) {
			return false;
		}
	}
	return true;
}

std::string to_string(std::vector<Token> const& tokens) {
	std::stringstream out;
	for (auto const& token : tokens) {
		out << to_string(token) << " ";
	}
	out << std::endl;
	return out.str();
}

void test(std::string const& input, std::vector<Token> const& expectedExpression, std::vector<Token> const& expectedRpn, double v)
{
	INFO("input: " << input);
	Tokenizer tokenizer;
	for (auto ch : input) {
		tokenizer.addChar(ch);
	}
	tokenizer.addChar(Tokenizer::EOF_VALUE);
	auto tokens = tokenizer.getTokens();
	{
		INFO("expr: " << to_string(expectedExpression) << to_string(tokens));
		REQUIRE(cmp(tokens, expectedExpression));
	}
	auto rpn = toRPN(tokens);
	{
		INFO("rpn: " << to_string(expectedRpn) << to_string(rpn));
		REQUIRE(cmp(rpn, expectedRpn));
	}
	auto value = calculate(rpn);
	{
		INFO("value: " << v << " != " << value);
		REQUIRE(v == value);
	}
}

TEST_CASE("parsing", "simple tests")
{
	test("1+2", {num(1), plus, num(2)}, {num(1), num(2), plus}, 3);
	test("1", {num(1)}, {num(1)}, 1);
	test("1 + 2 + 3 + 4",
	     {num(1), plus, num(2), plus, num(3), plus, num(4)},
	     {num(1), num(2), plus, num(3), plus, num(4), plus}, 10);
	test("1 * 2 * 3 * 4",
	     {num(1), mul, num(2), mul, num(3), mul, num(4)},
	     {num(1), num(2), mul, num(3), mul, num(4), mul}, 24);
	test("1", {num(1)}, {num(1)}, 1);
	test("1 * 2 + 3 * 4",
	     {num(1), mul, num(2), plus, num(3), mul, num(4)},
	     {num(1), num(2), mul, num(3), num(4), mul, plus}, 14);
	test("1 * (2 + 3) * 4",
	     {num(1), mul, l, num(2), plus, num(3), r, mul, num(4)},
	     {num(1), num(2), num(3), plus, mul, num(4), mul}, 20);
	test("(1 + (2 + (3 + (4 + (5 + (6 + 7))))))",
	     {l, num(1), plus, l, num(2), plus, l, num(3), plus, l, num(4), plus, l, num(5), plus, l, num(6), plus, num(7), r, r, r, r, r, r},
	     {num(1), num(2), num(3), num(4), num(5), num(6), num(7), plus, plus, plus, plus, plus, plus}, 28);
	test("1 / (2 - 3) / 4",
	     {num(1), divide, l, num(2), minus, num(3), r, divide, num(4)},
	     {num(1), num(2), num(3), minus, divide, num(4), divide}, -1.0/4.0);
	test("3-4",
	     {num(3), minus, num(4)},
	     {num(3), num(4), minus}, -1);
	test("3/4",
	     {num(3), divide, num(4)},
	     {num(3), num(4), divide}, 3.0/4);
	test("3*4",
	     {num(3), mul, num(4)},
	     {num(3), num(4), mul}, 3.0*4);
}
