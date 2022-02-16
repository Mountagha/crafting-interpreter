#pragma once

#include "Stmt.hpp"
#include "Expr.hpp"
#include <iostream>

namespace lox {

enum class Type {
    Assign, Binary, Grouping, Literal, Unary, Variable, Logical,
    Expression, If, Print, Var, Block, While
};

class TypeIdentifier : public StmtVisitor, public ExprVisitor {
    
    public:

        TypeIdentifier() = default;

        Type identify(std::unique_ptr<Expr>& expr) {
            expr->accept(*this);
            return type;
        }

        Type identify(std::unique_ptr<Stmt>& expr) {
            expr->accept(*this);
            return type;
        }


        LoxObject visitAssignExpr(Assign& expr) override {
            type = Type::Assign; return LoxObject();
        }
        LoxObject visitBinaryExpr(Binary& expr) override {
            type = Type::Binary; return LoxObject();
        }
        LoxObject visitGroupingExpr(Grouping& expr) override {
            type = Type::Grouping; return LoxObject();
        }
        LoxObject visitLiteralExpr(Literal& expr) override {
            type = Type::Literal; return LoxObject();
        }
        LoxObject visitVariableExpr(Variable& expr) override {
            type = Type::Variable; return LoxObject();
        }
        LoxObject visitUnaryExpr(Unary& expr) {
            type = Type::Unary; return LoxObject();
        }
        LoxObject visitLogicalExpr(Logical& expr) {
            type = Type::Logical; return LoxObject();
        }

        void visitExpressionStmt(Expression& stmt) override {
            type = Type::Expression;
        }
        void visitIfStmt(If& stmt) {
            type = Type::If;
        }
        void visitPrintStmt(Print& stmt) override {
            type = Type::Print;
        }
        void visitVarStmt(Var& stmt) override {
            type = Type::Var;
        }
        void visitBlockStmt(Block& stmt) {
            type = Type::Block; 
        }
        void visitWhileStmt(While& stmt) {
            type = Type::While;
        }  


    
    private:
        Type type;



};

} // namespace lox