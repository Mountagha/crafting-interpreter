#pragma once

#include "token.hpp"
#include "loxObject.hpp"
#include <memory>

namespace lox { 

class Binary;
class Grouping;
class Literal;
class Unary;

class ExprVisitor {
	public:
		virtual LoxObject visitBinaryExpr( Binary& expr) = 0;
		virtual LoxObject visitGroupingExpr( Grouping& expr) = 0;
		virtual LoxObject visitLiteralExpr( Literal& expr) = 0;
		virtual LoxObject visitUnaryExpr( Unary& expr) = 0;
};

class Expr {

	public:
		Expr() = default;
		~Expr() = default;
		virtual LoxObject accept(ExprVisitor& visitor) = 0;
};

class Binary: public Expr {
	public:
		Binary(std::unique_ptr<Expr>&& left_, Token operator__, std::unique_ptr<Expr>&& right_) {
			left = std::move (left_);
			operator_ = operator__;
			right = std::move (right_);
		}
		LoxObject accept(ExprVisitor& visitor) override {
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
		LoxObject accept(ExprVisitor& visitor) override {
			return visitor.visitGroupingExpr(*this);
		}
		std::unique_ptr<Expr> expression;
};

class Literal: public Expr {
	public:
		Literal(LoxObject value_) {
			value = value_;
		}
		LoxObject accept(ExprVisitor& visitor) override {
			return visitor.visitLiteralExpr(*this);
		}
		LoxObject value;
};

class Unary: public Expr {
	public:
		Unary(Token operator__, std::unique_ptr<Expr>&& right_) {
			operator_ = operator__;
			right = std::move (right_);
		}
		LoxObject accept(ExprVisitor& visitor) override {
			return visitor.visitUnaryExpr(*this);
		}
		Token operator_;
		std::unique_ptr<Expr> right;
};

} // lox namespace