#include "token.hpp"
#include <memory>

namespace lox { namespace AST { 

class Binary;
class Grouping;
class Literal;
class Unary;

class Visitor {
	public:
		virtual std::any visitBinaryExpr( Binary& expr) = 0;
		virtual std::any visitGroupingExpr( Grouping& expr) = 0;
		virtual std::any visitLiteralExpr( Literal& expr) = 0;
		virtual std::any visitUnaryExpr( Unary& expr) = 0;
};

class Expr {

	public:
		//Expr() = default;
		~Expr() {} 
		virtual std::any accept(Visitor& visitor) = 0;
};

class Binary: public Expr {
	public:
		Binary(std::unique_ptr<Expr>&& left, Token operator_, std::unique_ptr<Expr>&& right) {
			left = std::move (left);
			operator_ = operator_;
			right = std::move (right);
		}
		std::any accept(Visitor& visitor) override {
			return visitor.visitBinaryExpr(*this);
		}
		std::unique_ptr<Expr> left;
		Token operator_;
		std::unique_ptr<Expr> right;
};

class Grouping: public Expr {
	public:
		Grouping(std::unique_ptr<Expr>&& expression) {
			expression = std::move (expression);
		}
		std::any accept(Visitor& visitor) override {
			return visitor.visitGroupingExpr(*this);
		}
		std::unique_ptr<Expr> expression;
};

class Literal: public Expr {
	public:
		Literal(std::any value) {
			value = value;
		}
		std::any accept(Visitor& visitor) override {
			return visitor.visitLiteralExpr(*this);
		}
		std::any value;
};

class Unary: public Expr {
	public:
		Unary(Token operator_, Expr* right) {
			operator_ = operator_;
			right = right;
			std::cerr << operator_;
		}
		std::any accept(Visitor& visitor) override {
			std::cerr << "I'm here";
			return visitor.visitUnaryExpr(*this);
		}
		Token operator_;
		std::unique_ptr<Expr> right;
};

} // AST namespace
} // lox namespace