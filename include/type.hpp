#pragma once

#include "Stmt.hpp"
#include "Expr.hpp"
#include <iostream>

namespace lox {

enum class Type {
    Assign, Binary, Grouping, Literal, Unary, Variable, Logical, Call, Get, Set, This,
    Expression, If, Print, Var, Block, While, Function, Return, Class
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
        LoxObject visitUnaryExpr(Unary& expr) override {
            type = Type::Unary; return LoxObject();
        }
        LoxObject visitLogicalExpr(Logical& expr) override {
            type = Type::Logical; return LoxObject();
        }
        LoxObject visitCallExpr(Call& expr) override {
            type = Type::Call; return LoxObject();
        }
        LoxObject visitGetExpr(Get& expr) override {
            type = Type::Get; return LoxObject();
        }
        LoxObject visitSetExpr(Set& expr) override {
            type = Type::Set; return LoxObject();
        }
        LoxObject visitThisExpr(This& expr) override {
            type = Type::This; return LoxObject();
        }

        void visitExpressionStmt(Expression& stmt) override {
            type = Type::Expression;
        }
        void visitIfStmt(If& stmt) {
            type = Type::If;
        }
        void visitFunctionStmt(Function& stmt) override {
            type = Type::Function;
        }
        void visitPrintStmt(Print& stmt) override {
            type = Type::Print;
        }
        void visitVarStmt(Var& stmt) override {
            type = Type::Var;
        }
        void visitBlockStmt(Block& stmt) override {
            type = Type::Block; 
        }
        void visitWhileStmt(While& stmt) override {
            type = Type::While;
        } 
        void visitReturnStmt(Return& stmt) override {
            type = Type::Return;
        }
        void visitClassStmt(Class& stmt) override {
            type = Type::Class;
        } 


    
    private:
        Type type;



};

} // namespace lox