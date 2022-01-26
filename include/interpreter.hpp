#pragma once

#include "Expr.hpp"
/*
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

        bool isTruthy(std::any& object) {
            if (!(object.has_value())) return false;
            if (object.type() == typeid(bool)) return std::any_cast<bool>(object);
            return true;
        }

        bool isEqual(std::any& object_a, std::any& object_b) {
            if (!object_a.has_value() && !object_b.has_value()) return true;
            if (!object_a.has_value()) return false;
            return true; 
        }


};

}
*/