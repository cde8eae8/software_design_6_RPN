#ifndef TOKEN_H
#define TOKEN_H

#include <memory>
#include <type_traits>

class TokenVisitor;

class Token {
public:
	template <typename T>
	Token(T val)
	    : ptr{std::make_unique<Derived<T>>(std::move(val))}
	{
    }

	Token(Token const& v) : ptr{v.ptr->clone()} { }
	Token &operator=(Token const& v) {
	    ptr = v.ptr->clone();
	    return *this;
    }


	void accept(TokenVisitor& visitor) const {
	    ptr->accept(visitor);
    };

	template<typename T>
	T* holds_type() {
		if (auto *p = dynamic_cast<Derived<std::decay_t<T>>*>(ptr.get())) {
			return &p->impl;
		}
		return nullptr;
	}

	virtual ~Token() {}

private:
	class Base {
	public:
		virtual std::unique_ptr<Base> clone() const = 0;
		virtual void accept(TokenVisitor& visitor) = 0;
		virtual ~Base() = default;
	};

	template <typename T>
	class Derived : public Base {
	public:
		Derived(T val) : impl{std::move(val)} {}

		std::unique_ptr<Base> clone() const override { return std::make_unique<Derived<T>>(impl); }
		void accept(TokenVisitor& visitor) override { impl.accept(visitor); };

		T impl;
	};

	std::unique_ptr<Base> ptr;
};

class NumberToken {
public:
	NumberToken(double v) : value{v} { }

	void accept(TokenVisitor &visitor) const;

	double value;
};

class BraceToken {
public:
	BraceToken(uint8_t c) : ch{c} { }

	void accept(TokenVisitor &visitor) const;

	uint8_t ch;
};

class OperationToken {
public:
	OperationToken(uint8_t c) : ch{c} { }

	void accept(TokenVisitor &visitor) const;

	int priority() const {
		if (ch == '+' || ch == '-') {
			return 0;
		} else {
			return 1;
		}
	}

	uint8_t ch;
};

#endif // TOKEN_H
