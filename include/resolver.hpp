#pragma once

#include <stack>
#include <map>
#include "Expr.hpp"
#include "Stmt.hpp"
#include "interpreter.hpp"

namespace lox {

class Resolver : public ExprVisitor, public StmtVisitor {

    public:
        using SExpr = std::unique_ptr<Stmt>;
        using PExpr = std::unique_ptr<Expr>;
        Resolver(Interpreter* intp): interpreter{intp} {}

        void resolve(std::vector<SExpr>& statements) {
            for (auto& statement : statements ) {
                resolve(statement);
            }
        }

        void visitVarStmt(Var& stmt) override {
            declare(stmt.name);
            if (stmt.initializer) {
                resolve(stmt.initializer);
            }
            define(stmt.name);
        }

        LoxObject visitVariableExpr(Variable& expr) {
            if (!scopes.empty() && 
                scopes.top().at(expr.name.lexeme) == false) {
                    Lox::error(expr.name, 
                        "Can't read local variable in its own initializer");
            }

            resolveLocal(expr, expr.name);
        }







    private:
        Interpreter* interpreter;
        std::stack<std::map<std::string, bool>> scopes {};

        void resolve(SExpr& stmt) {
            stmt->accept(*this);
        }

        void resolve(PExpr& expr){
            expr->accept(*this);
        }

        void beginScope() {
            std::map<std::string, bool> newMap{};
            scopes.push(newMap);
        }

        void endScope() {
            scopes.pop();
        }

        void declare(Token name) {
            if (scopes.empty()) return;

            auto scope = scopes.top();
            scope[name.lexeme] = false;
        }

        void define(Token name) {
            if (scopes.empty()) return;
            auto scope = scopes.top();
            scope[name.lexeme] = true;
        }

        void resolveLocal(Variable& expr, Token name) {
            /* for (int i = scopes.size() -1; i >= 0; i--) {
                if (scopes.)
            } */
        }

};

} // lox namespace