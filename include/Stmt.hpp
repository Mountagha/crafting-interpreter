#pragma once

#include "token.hpp"
#include "loxObject.hpp"
#include <memory>
#include "Expr.hpp"
#include <vector>

namespace lox { 

class Block;
class Expression;
class If;
class Print;
class Var;

class StmtVisitor {
	public:
		virtual void visitBlockStmt( Block& stmt) = 0;
		virtual void visitExpressionStmt( Expression& stmt) = 0;
		virtual void visitIfStmt( If& stmt) = 0;
		virtual void visitPrintStmt( Print& stmt) = 0;
		virtual void visitVarStmt( Var& stmt) = 0;
};

class Stmt {

	public:
		Stmt() = default;
		~Stmt() = default;
		virtual void accept(StmtVisitor& visitor) = 0;
};

class Block: public Stmt {
	public:
		Block(std::vector<std::unique_ptr<Stmt>>&& statements_) {
			statements = statements_;
		}
		void accept(StmtVisitor& visitor) override {
			visitor.visitBlockStmt(*this);
		}
		std::vector<std::unique_ptr<Stmt>>&& statements;
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

class If: public Stmt {
	public:
		If(std::unique_ptr<Expr>&& condition_, std::unique_ptr<Stmt>&& thenBranch_, std::unique_ptr<Stmt>&& elseBranch_) {
			condition = std::move (condition_);
			thenBranch = std::move (thenBranch_);
			elseBranch = std::move (elseBranch_);
		}
		void accept(StmtVisitor& visitor) override {
			visitor.visitIfStmt(*this);
		}
		std::unique_ptr<Expr> condition;
		std::unique_ptr<Stmt> thenBranch;
		std::unique_ptr<Stmt> elseBranch;
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

class Var: public Stmt {
	public:
		Var(Token name_, std::unique_ptr<Expr>&& initializer_) {
			name = name_;
			initializer = std::move (initializer_);
		}
		void accept(StmtVisitor& visitor) override {
			visitor.visitVarStmt(*this);
		}
		Token name;
		std::unique_ptr<Expr> initializer;
};

} // lox namespace