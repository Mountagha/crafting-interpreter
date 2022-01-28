#pragma once

#include "Expr.hpp"
#include "lox.hpp"
#include <iostream>

namespace lox {

class Interpreter : public Visitor {

    public: 
        LoxObject visitLiteralExpr(Literal& expr) override;
        LoxObject visitGroupingExpr(Grouping& expr) override;
        LoxObject visitUnaryExpr(Unary& expr) override;
        LoxObject visitBinaryExpr(Binary& expr) override;
        void interpret(std::unique_ptr<Expr> expr);
    
    private:
        LoxObject evaluate(std::unique_ptr<Expr>& expr) {
            return expr->accept(*this);
        }

};

}