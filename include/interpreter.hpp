#pragma once

#include "loxCallable.hpp"
#include "environment.hpp"
#include "loxObject.hpp"
#include "Expr.hpp"
#include "lox.hpp"
#include "Stmt.hpp"
#include <iostream>
#include <vector>
#include <map>

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
        void visitFunctionStmt(Function& stmt) override;
        void visitIfStmt(If& Stmt) override;
        void visitPrintStmt(Print& stmt) override;
        void visitReturnStmt(Return& stmt) override;
        void visitVarStmt(Var& stmt) override;
        void visitBlockStmt(Block& stmt) override;
        void visitWhileStmt(While& stmt) override;

        void interpret(std::vector<std::unique_ptr<Stmt>>& statements);
        void executeBlock(std::vector<std::unique_ptr<Stmt>>& statements, PEnvironment Environment); 
        void resolve(Expr* expr, unsigned int depth);
    
    private:

        PEnvironment globals;
        PEnvironment environment;
        std::map<Expr*, unsigned int> locals {};

        LoxObject evaluate(std::unique_ptr<Expr>& expr) {
            return expr->accept(*this);
        }

        void execute(std::unique_ptr<Stmt>& stmt) {
            stmt->accept(*this);
        }

        LoxObject lookUpVariable(Token name, Expr* expr) {
            if (locals.find(expr) != locals.end()) {
                auto distance = locals.at(expr);
                if (distance) {
                    return environment->getAt(distance, name.lexeme);
                }
            } else {
                return globals->get(name);
            }
        }



};

}