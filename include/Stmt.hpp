#pragma once

#include "token.hpp"
#include "loxObject.hpp"
#include <memory>
#include <vector>
#include "Expr.hpp"

namespace lox { 

class Block;
class Class;
class Expression;
class Function;
class If;
class Print;
class Return;
class Var;
class While;

class StmtVisitor {
	public:
		virtual void visitBlockStmt( Block& stmt) = 0;
		virtual void visitClassStmt( Class& stmt) = 0;
		virtual void visitExpressionStmt( Expression& stmt) = 0;
		virtual void visitFunctionStmt( Function& stmt) = 0;
		virtual void visitIfStmt( If& stmt) = 0;
		virtual void visitPrintStmt( Print& stmt) = 0;
		virtual void visitReturnStmt( Return& stmt) = 0;
		virtual void visitVarStmt( Var& stmt) = 0;
		virtual void visitWhileStmt( While& stmt) = 0;
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
			statements = std::move (statements_);
		}
		void accept(StmtVisitor& visitor) override {
			visitor.visitBlockStmt(*this);
		}
		std::vector<std::unique_ptr<Stmt>> statements;
};

class Class: public Stmt {
	public:
		Class(Token name_, std::unique_ptr<Expr>&& superclass_, std::vector<std::unique_ptr<Function>>&& methods_) {
			name = name_;
			superclass = std::move (superclass_);
			methods = std::move (methods_);
		}
		void accept(StmtVisitor& visitor) override {
			visitor.visitClassStmt(*this);
		}
		Token name;
		std::unique_ptr<Expr> superclass;
		std::vector<std::unique_ptr<Function>> methods;
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

class Function: public Stmt {
	public:
		Function(Token name_, std::vector<Token>&& params_, std::vector<std::unique_ptr<Stmt>>&& body_) {
			name = name_;
			params = std::move (params_);
			body = std::move (body_);
		}
		void accept(StmtVisitor& visitor) override {
			visitor.visitFunctionStmt(*this);
		}
		Token name;
		std::vector<Token> params;
		std::vector<std::unique_ptr<Stmt>> body;
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

class Return: public Stmt {
	public:
		Return(Token keyword_, std::unique_ptr<Expr>&& value_) {
			keyword = keyword_;
			value = std::move (value_);
		}
		void accept(StmtVisitor& visitor) override {
			visitor.visitReturnStmt(*this);
		}
		Token keyword;
		std::unique_ptr<Expr> value;
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

class While: public Stmt {
	public:
		While(std::unique_ptr<Expr>&& condition_, std::unique_ptr<Stmt>&& body_) {
			condition = std::move (condition_);
			body = std::move (body_);
		}
		void accept(StmtVisitor& visitor) override {
			visitor.visitWhileStmt(*this);
		}
		std::unique_ptr<Expr> condition;
		std::unique_ptr<Stmt> body;
};

} // lox namespace