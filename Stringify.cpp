#include "Stringify.h"

#include <cassert>
#include "TokenVisitor.h"

class TokenWriter : public TokenVisitor {
public:
	TokenWriter(Token const& token) {
		token.accept(*this);
	}
	void visit(NumberToken const& num) override {
		value = "NUMBER(" + std::to_string(num.value) + ")";
	}
	void visit(BraceToken const& brace) override {
		if (brace.ch == '(') {
			value = "LEFT";
		} else {
			value = "RIGHT";
		}
	}
	void visit(OperationToken const& op) override {
		value = [ch = op.ch]() -> std::string {
			switch (ch) {
			case '+': return "PLUS";
			case '-': return "MINUS";
			case '/': return "DIV";
			case '*': return "MUL";
			}
			assert(false);
		}();
	}

	std::string value;
};

std::string to_string(const Token& token) {
	return TokenWriter{token}.value;
}
