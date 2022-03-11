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
};

} // lox namespace