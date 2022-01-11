#pragma once

#include "Expr.hpp"

namespace lox {

class Interpreter : public Visitor {

    public: 
        std::any visitLiteralExpr(Literal& expr) override;
        std::any visitGroupingExpr(Grouping& expr) override;
        std::any visitUnaryExpr(Unary& expr) override;
        std::any visitBinaryExpr(Binary& expr) override;
    
    private:
        std::any evaluate(std::unique_ptr<Expr>& expr) {
            return expr->accept(*this);
        }

};

}