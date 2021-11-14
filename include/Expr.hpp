#include "token.hpp"

namespace lox { namespace AST { 

template <typename T>
class Visitor {
	public:
		virtual T visitBinaryExpr( Binary& expr) = 0;
		virtual T visitGroupingExpr( Grouping& expr) = 0;
		virtual T visitLiteralExpr( Literal& expr) = 0;
		virtual T visitUnaryExpr( Unary& expr) = 0;
};

template<typename T>
class Expr {

	public:
		virtual T accept(Visitor<T>& visitor) = 0;
};

template<typename T>
class Binary: public Expr<T> {
	public:
		Binary(Expr left, Token operator_, Expr right) {
			left = left;
			operator_ = operator_;
			right = right;
		}
		T accept(Visitor<T>& visitor) override {
			return visitor.visitBinaryExpr(*this);
		}
		Expr left;
		Token operator_;
		Expr right;
};

template<typename T>
class Grouping: public Expr<T> {
	public:
		Grouping(Expr expression) {
			expression = expression;
		}
		T accept(Visitor<T>& visitor) override {
			return visitor.visitGroupingExpr(*this);
		}
		Expr expression;
};

template<typename T>
class Literal: public Expr<T> {
	public:
		Literal(std::string value) {
			value = value;
		}
		T accept(Visitor<T>& visitor) override {
			return visitor.visitLiteralExpr(*this);
		}
		std::string value;
};

template<typename T>
class Unary: public Expr<T> {
	public:
		Unary(Token operator_, Expr right) {
			operator_ = operator_;
			right = right;
		}
		T accept(Visitor<T>& visitor) override {
			return visitor.visitUnaryExpr(*this);
		}
		Token operator_;
		Expr right;
};

} // AST namespace
} // lox namespace