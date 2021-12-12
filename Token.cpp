#include "Token.h"
#include "TokenVisitor.h"

void OperationToken::accept(TokenVisitor &visitor) const {
	visitor.visit(*this);
}

void NumberToken::accept(TokenVisitor &visitor) const {
	visitor.visit(*this);
}

void BraceToken::accept(TokenVisitor &visitor) const {
	visitor.visit(*this);
}
