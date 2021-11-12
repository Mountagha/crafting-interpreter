#include "token.hpp"

namespace lox { namespace AST { 

class Expr {};

class Binary: public Expr {
	public:
		Binary(Expr left, Token operator, Expr right) {
			left = left;
			operator = operator;
			right = right;
		}
	private:
		Expr left;
		Token operator;
		Expr right;
};

class Grouping: public Expr {
	public:
		Grouping(Expr expression) {
			expression = expression;
		}
	private:
		Expr expression;
};

class Literal: public Expr {
	public:
		Literal(Object value) {
			value = value;
		}
	private:
		Object value;
};

class Unary: public Expr {
	public:
		Unary(Token operator, Expr right) {
			operator = operator;
			right = right;
		}
	private:
		Token operator;
		Expr right;
};

} // AST namespace
} // lox namespace