#include "ParserVisitor.h"

class ParserVisitor : public TokenVisitor {
public:
	void visit(NumberToken const& num) override {
		result.push_back(Token{num});
	}
	void visit(BraceToken const& brace) override {
		if (brace.ch == '(') {
			stack.push_back(brace);
		} else if (brace.ch == ')') {
			while (!stack.empty()) {
				if (auto const* brace = stack.back().holds_type<BraceToken>()) {
					if (brace->ch == '(') {
						stack.pop_back();
						return;
					}
				}
				result.push_back(std::move(stack.back()));
				stack.pop_back();
			}
			throw std::runtime_error("bad expression");
		}
	}
	void visit(OperationToken const& op) override {
		while (!stack.empty()) {
			if (auto const* stackOp = stack.back().holds_type<OperationToken>()) {
				if (stackOp->priority() < op.priority()) {
					break;
				}
				result.push_back(std::move(stack.back()));
				stack.pop_back();
			}
			break;
		}
		stack.push_back(op);
	}

	std::vector<Token> getResults() {
		auto res = result;
		for (auto it = stack.rbegin(); it != stack.rend(); ++it) {
			if (it->holds_type<BraceToken>()) {
				throw std::runtime_error("bad expression");
			}
			res.push_back(*it);
		}
		return res;
	}

private:
	std::vector<Token> stack;
	std::vector<Token> result;
};

std::vector<Token> toRPN(const std::vector<Token>& tokens)
{
	ParserVisitor visitor;
	for (auto const& token : tokens) {
		token.accept(visitor);
	}
	return visitor.getResults();
}
