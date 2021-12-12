#ifndef TOKENVISITOR_H
#define TOKENVISITOR_H

class NumberToken;
class BraceToken;
class OperationToken;

class TokenVisitor {
public:
	virtual void visit(NumberToken const&) = 0;
	virtual void visit(BraceToken const&) = 0;
	virtual void visit(OperationToken const&) = 0;

	virtual ~TokenVisitor() {}
};

#endif // TOKENVISITOR_H
