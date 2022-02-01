#pragma once

#include "token.hpp"
#include "loxObject.hpp"
#include <memory>
#include "Expr.hpp"

namespace lox { 

class Expression;
class Print;

class Visitor {
	public:
		virtual LoxObject visitExpressionStmt( Expression& stmt) = 0;
		virtual LoxObject visitPrintStmt( Print& stmt) = 0;
};

class Stmt {

	public:
		Stmt() = default;
		~Stmt() = default;
		virtual LoxObject accept(Visitor& visitor) = 0;
};

class Expression: public Stmt {
	public:
		Expression(std::unique_ptr<Expr>&& expression_) {
			expression = std::move (expression_);
		}
		LoxObject accept(Visitor& visitor) override {
			return visitor.visitExpressionStmt(*this);
		}
		std::unique_ptr<Expr> expression;
};

class Print: public Stmt {
	public:
		Print(std::unique_ptr<Expr>&& expression_) {
			expression = std::move (expression_);
		}
		LoxObject accept(Visitor& visitor) override {
			return visitor.visitPrintStmt(*this);
		}
		std::unique_ptr<Expr> expression;
};

} // lox namespace