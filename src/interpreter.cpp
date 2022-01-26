#include "interpreter.hpp"
/*
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
        case TokenType::BANG:
            return !isTruthy(right);
        case TokenType::MINUS:
            return std::any_cast<double>(right);
    }

    // Unreachable 
    return nullptr;
}

std::any Interpreter::visitBinaryExpr(Binary& expr) {
    std::any left = evaluate(expr.left);
    std::any right = evaluate(expr.right);

    switch(expr.operator_.token_type) {
        case TokenType::GREATER:
            return std::any_cast<double>(left) > std::any_cast<double>(right);
        case TokenType::GREATER_EQUAL:
            return std::any_cast<double>(left) >= std::any_cast<double>(right);
        case TokenType::LESS:
            return std::any_cast<double>(left) < std::any_cast<double>(right);
        case TokenType::LESS_EQUAL:
            return std::any_cast<double>(left) <= std::any_cast<double>(right);
        case TokenType::MINUS:
            return std::any_cast<double>(left) - std::any_cast<double>(right);
        case TokenType::PLUS:
            if (left.type() == typeid(double) && right.type() == typeid(double)) {
                return std::any_cast<double>(left) + std::any_cast<double>(right);
            }
            if (left.type() == typeid(std::string) && right.type() == typeid(std::string)) {
                return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
            }
        break;
        case TokenType::SLASH:
            return std::any_cast<double>(left) / std::any_cast<double>(right);
        case TokenType::STAR:
            return std::any_cast<double>(left) * std::any_cast<double>(right);
        case TokenType::BANG_EQUAL return (!isEqual(left, right));
        case TokenType::EQUAL_EQUAL return isEqual(left, right);
    }

    // unreachable 
    return nullptr;
}

}
*/