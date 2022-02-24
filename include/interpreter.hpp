#pragma once

#include "Expr.hpp"
#include "lox.hpp"
#include "Stmt.hpp"
#include "environment.hpp"
#include <iostream>
#include <vector>

namespace lox {

class Interpreter : public ExprVisitor, public  StmtVisitor{

    public:
        Interpreter();
        // Expr
        LoxObject visitLiteralExpr(Literal& expr) override;
        LoxObject visitGroupingExpr(Grouping& expr) override;
        LoxObject visitUnaryExpr(Unary& expr) override;
        LoxObject visitBinaryExpr(Binary& expr) override;
        LoxObject visitVariableExpr(Variable& expr) override;
        LoxObject visitAssignExpr(Assign& expr) override;
        LoxObject visitLogicalExpr(Logical& expr) override;
        LoxObject visitCallExpr(Call& expr) override;

        // Stmt
        void visitExpressionStmt(Expression& stmt) override;
        void visitIfStmt(If& Stmt) override;
        void visitPrintStmt(Print& stmt) override;
        void visitVarStmt(Var& stmt) override;
        void visitBlockStmt(Block& stmt) override;
        void visitWhileStmt(While& stmt) override;

        void interpret(std::vector<std::unique_ptr<Stmt>>& statements);
    
    private:

        PEnvironment environment;
        void executeBlock(std::vector<std::unique_ptr<Stmt>>& statements, PEnvironment Environment); 

        LoxObject evaluate(std::unique_ptr<Expr>& expr) {
            return expr->accept(*this);
        }

        void execute(std::unique_ptr<Stmt>& stmt) {
            stmt->accept(*this);
        }



};

}