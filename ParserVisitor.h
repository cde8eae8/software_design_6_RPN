#ifndef PARSERVISITOR_H
#define PARSERVISITOR_H

#include <vector>
#include <stdexcept>
#include "Token.h"
#include "TokenVisitor.h"

std::vector<Token> toRPN(std::vector<Token> const& tokens);

#endif // PARSERVISITOR_H
