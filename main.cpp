#include <iostream>
#include <variant>
#include <iterator>
#include <vector>
#include <algorithm>
#include <string>
#include <memory>
#include <assert.h>

#include "Token.h"
#include "TokenVisitor.h"
#include "Tokenizer.h"
#include "Stringify.h"
#include "ParserVisitor.h"
#include "RpnCalculator.h"

using namespace std;

int main()
{
	try {
		std::string input;
		std::getline(std::cin, input);
		Tokenizer tokenizer;
		for (auto ch : input) {
			tokenizer.addChar(ch);
		}
		tokenizer.addChar(Tokenizer::EOF_VALUE);
		auto tokens = tokenizer.getTokens();
		for (auto const& token : tokens) {
			std::cout << to_string(token) << " ";
		}
		std::cout << std::endl;

		auto rpn = toRPN(tokens);

		for (auto const& token : rpn) {
			std::cout << to_string(token) << " ";
		}
		std::cout << std::endl;

		auto value = calculate(rpn);
		std::cout << "result = " << value << std::endl;
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
