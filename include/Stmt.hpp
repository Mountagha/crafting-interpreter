#pragma once

#include "token.hpp"
#include "loxObject.hpp"
#include <memory>
#include "Expr.hpp"

namespace lox { 

class Expression;
class Print;

class StmtVisitor {
	public:
		virtual void visitExpressionStmt( Expression& stmt) = 0;
		virtual void visitPrintStmt( Print& stmt) = 0;
};

class Stmt {

	public:
		Stmt() = default;
		~Stmt() = default;
		virtual void accept(StmtVisitor& visitor) = 0;
};

class Expression: public Stmt {
	public:
		Expression(std::unique_ptr<Expr>&& expression_) {
			expression = std::move (expression_);
		}
		void accept(StmtVisitor& visitor) override {
			visitor.visitExpressionStmt(*this);
		}
		std::unique_ptr<Expr> expression;
};

class Print: public Stmt {
	public:
		Print(std::unique_ptr<Expr>&& expression_) {
			expression = std::move (expression_);
		}
		void accept(StmtVisitor& visitor) override {
			visitor.visitPrintStmt(*this);
		}
		std::unique_ptr<Expr> expression;
};

} // lox namespace