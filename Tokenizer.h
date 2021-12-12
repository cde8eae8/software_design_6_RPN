#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stddef.h>
#include <stdint.h>
#include <memory>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include "Token.h"

class Tokenizer {
public:
	static constexpr uint8_t EOF_VALUE = '\0';

	Tokenizer() {
		(void)context.changeState(std::make_unique<StatelessReader>(&context));
	}

	void addChar(uint8_t ch) {
		if (context.eof) {
			throw std::runtime_error("adding char after EOF");
		}
		context.getReader().process(ch);
	}

	std::vector<Token> const& getTokens() const {
		if (!context.eof) {
			throw std::runtime_error("can't return tokens until EOF");
		}
		return context.tokens;
	}

private:
	class Context;
	class StatelessReader;

	class Reader {
	public:
		Reader(Context* c) : context{c} { }

		virtual void process(uint8_t ch) = 0;

		virtual ~Reader() {}

		Context* context;
	};

	class NumberReader : public Reader {
	public:
		NumberReader(Context* c) : Reader{c}, value{0} { }

		void process(uint8_t ch) override {
			if (isdigit(ch)) {
				value = value * 10 + (ch - '0');
				return;
			}
			context->tokens.push_back(NumberToken{static_cast<double>(value)});
			std::unique_ptr<Reader> keepAlive = context->changeState(std::make_unique<StatelessReader>(context));
			context->getReader().process(ch);
		}

	private:
		uint32_t value;
	};

	class StatelessReader : public Reader {
	public:
		StatelessReader(Context* c) : Reader{c} { }

		void process(uint8_t ch) override {
			if (ch == EOF_VALUE) {
				context->eof = true;
				return;
			}
			if (isdigit(ch)) {
				std::unique_ptr<Reader> keepAlive = context->changeState(std::make_unique<NumberReader>(context));
				context->getReader().process(ch);
				return;
			}
			if (isspace(ch)) {
				return;
			}
			std::initializer_list<uint8_t> operations{'-', '+', '*', '/'};
			if (std::find(operations.begin(), operations.end(), ch) != operations.end()) {
				context->tokens.push_back(OperationToken{ch});
				return;
			}
			if (ch == ')' || ch == '(') {
				context->tokens.push_back(BraceToken{ch});
				return;
			}
			throw std::runtime_error(std::string("found unexpected char ") + static_cast<char>(ch));
		}
	};

	class Context {
	public:
		std::vector<Token> tokens;
		bool eof = false;

		Reader& getReader() { return *reader; }

		[[nodiscard]] std::unique_ptr<Reader> changeState(std::unique_ptr<Reader> newReader) {
			return std::exchange(reader, std::move(newReader));
		}

	private:
		std::unique_ptr<Reader> reader;
	};

	Context context;
};


#endif // TOKENIZER_H
