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
		Expr() = default;
		virtual ~Expr() = default;
		virtual std::any accept(Visitor& visitor) = 0;
};

class Binary: public Expr {
	public:
		Binary(std::unique_ptr<Expr>&& left_, Token operator__, std::unique_ptr<Expr>&& right_) {
			left = std::move (left_);
			operator_ = operator__;
			right = std::move (right_);
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
		Grouping(std::unique_ptr<Expr>&& expression_) {
			expression = std::move (expression_);
		}
		std::any accept(Visitor& visitor) override {
			return visitor.visitGroupingExpr(*this);
		}
		std::unique_ptr<Expr> expression;
};

class Literal: public Expr {
	public:
		Literal(std::any value_) {
			value = value_;
		}
		std::any accept(Visitor& visitor) override {
			std::cout << std::any_cast<double>(value);
			return visitor.visitLiteralExpr(*this);
		}
		std::any value;
};

class Unary: public Expr {
	public:
		Unary(Token operator__, std::unique_ptr<Expr>&& right_) {
			operator_ = operator__;
			right = std::move (right_);
		}
		std::any accept(Visitor& visitor) override {
			return visitor.visitUnaryExpr(*this);
		}
		Token operator_;
		std::unique_ptr<Expr> right;
};

} // AST namespace
} // lox namespace