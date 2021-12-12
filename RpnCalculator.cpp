#include "RpnCalculator.h"
#include "TokenVisitor.h"
#include <stdexcept>
#include <cassert>

class CalcVisitor : public TokenVisitor {
public:
	void visit(NumberToken const& num) override {
		stack.push_back(num);
	}

	void visit(BraceToken const& brace) override {
		assert(false);
	}

	void visit(OperationToken const& op) override {
		if (stack.size() < 2) throw std::runtime_error("bad expression");
		auto op2 = stack.back();
		stack.pop_back();
		auto op1 = stack.back();
		stack.pop_back();
		if (!op1.holds_type<NumberToken>()) throw std::runtime_error("bad expression");
		if (!op2.holds_type<NumberToken>()) throw std::runtime_error("bad expression");
		auto o1 = op1.holds_type<NumberToken>()->value;
		auto o2 = op2.holds_type<NumberToken>()->value;
		auto result = [&o1, &o2, ch = op.ch]() {
			switch (ch) {
			    case '+': return NumberToken{o1 + o2};
			    case '-': return NumberToken{o1 - o2};
			    case '*': return NumberToken{o1 * o2};
			    case '/': return NumberToken{o1 / o2};
			}
			assert(false);
		}();
		stack.push_back(result);
	}

	double value() {
		if (stack.size() != 1) throw std::runtime_error("bad expression");
		if (!stack.back().holds_type<NumberToken>()) throw std::runtime_error("bad expression");
		return stack.back().holds_type<NumberToken>()->value;
	}

private:
	std::vector<Token> stack;
};

double calculate(const std::vector<Token>& rpn)
{
	CalcVisitor visitor;
	for (auto const& token : rpn) {
		token.accept(visitor);
	}
	return visitor.value();
}
