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
        ~Interpreter();
        // Expr
        LoxObject visitLiteralExpr(Literal& expr) override;
        LoxObject visitGroupingExpr(Grouping& expr) override;
        LoxObject visitUnaryExpr(Unary& expr) override;
        LoxObject visitBinaryExpr(Binary& expr) override;
        LoxObject visitVariableExpr(Variable& expr) override;
        LoxObject visitAssignExpr(Assign& expr) override;
        LoxObject visitLogicalExpr(Logical& expr) override;
        LoxObject visitCallExpr(Call& expr) override;
        LoxObject visitGetExpr(Get& expr) override;
        LoxObject visitSetExpr(Set& expr) override;
        LoxObject visitSuperExpr(Super& expr) override;
        LoxObject visitThisExpr(This& expr) override;
        LoxObject visitTernaryExpr(Ternary& expr) override;

        // Stmt
        void visitExpressionStmt(Expression& stmt) override;
        void visitFunctionStmt(Function& stmt) override;
        void visitIfStmt(If& Stmt) override;
        void visitPrintStmt(Print& stmt) override;
        void visitReturnStmt(Return& stmt) override;
        void visitVarStmt(Var& stmt) override;
        void visitBlockStmt(Block& stmt) override;
        void visitWhileStmt(While& stmt) override;
        void visitClassStmt(Class& stmt) override;

        // handful functions for better memory managements with callables
        void addUser(LoxClass* klass);
        void addUser(LoxCallable* func);
        void addUser(LoxInstance* inst);
        void removeUser(LoxClass* klass);
        void removeUser(LoxCallable* func);
        void removeUser(LoxInstance* inst);

        LoxFunction* createFunction(LoxFunction* stmt, PEnvironment env);
        LoxFunction* createFunction(Function* stmt, PEnvironment env, bool initClass = false);
        LoxInstance* createInstance(LoxClass* loxklass);
        void registerFunction(LoxFunction* func, PEnvironment);
        void deleteFunction(LoxFunction* func);

        void interpret(std::vector<std::unique_ptr<Stmt>>& statements);
        void executeBlock(std::vector<std::unique_ptr<Stmt>>& statements, PEnvironment Environment); 
        void resolve(Expr* expr, unsigned int depth);
    
    private:

        PEnvironment globals;
        PEnvironment environment;
        std::map<Expr*, unsigned int> locals {};

        std::map<LoxCallable*, std::pair<std::unique_ptr<LoxCallable>, size_t>> m_callables;
        std::map<LoxClass*, std::pair<std::unique_ptr<LoxClass>, size_t>> m_classes;
        std::map<LoxInstance*, std::pair<std::unique_ptr<LoxInstance>, size_t>> m_instances;

        std::map<LoxFunction*, PEnvironment> m_funcenvs;

        bool m_destroying;
         

        LoxObject evaluate(std::unique_ptr<Expr>& expr) {
            return expr->accept(*this);
        }

        void execute(std::unique_ptr<Stmt>& stmt) {
            stmt->accept(*this);
        }

        LoxObject lookUpVariable(Token name, Expr* expr) {
            if (locals.find(expr) != locals.end()) {
                auto distance = locals.at(expr);
                return environment->getAt(distance, name.lexeme);
            } else {
                return globals->get(name);
            }
        }



};

} // lox namespace