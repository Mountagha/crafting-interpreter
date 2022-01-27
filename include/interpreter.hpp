#pragma once

#include "Expr.hpp"

namespace lox {

class Interpreter : public Visitor {

    public: 
        LoxObject visitLiteralExpr(Literal& expr) override;
        LoxObject visitGroupingExpr(Grouping& expr) override;
        LoxObject visitUnaryExpr(Unary& expr) override;
        LoxObject visitBinaryExpr(Binary& expr) override;
    
    private:
        LoxObject evaluate(std::unique_ptr<Expr>& expr) {
            return expr->accept(*this);
        }
         

};

}