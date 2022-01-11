#include "interpreter.hpp"

namespace lox {

std::any Interpreter::visitLiteralExpr(Literal& expr) {
    return expr.value;
}

std::any Interpreter::visitGroupingExpr(Grouping& expr) {
    return evaluate(expr.expression);
}

std::any Interpreter::visitUnaryExpr(Unary& expr) {
    std::any right = evaluate(expr.right);

    switch(expr.operator_.token_type) {
        case TokenType::MINUS:
            return std::any_cast<double>(right);
    }

    // Unreachable 
    return nullptr;
}







}