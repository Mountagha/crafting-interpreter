#include "interpreter.hpp"

namespace lox {

LoxObject Interpreter::visitLiteralExpr(Literal& expr) {
    return expr.value;
}

LoxObject Interpreter::visitGroupingExpr(Grouping& expr) {
    return evaluate(expr.expression);
}

LoxObject Interpreter::visitUnaryExpr(Unary& expr) {
    LoxObject right = evaluate(expr.right);

    switch(expr.operator_.token_type) {
        case TokenType::BANG:
            return !right;
        case TokenType::MINUS:
            return -right;
        default:
            std::runtime_error("Invalid unary expression.");
    }

}

LoxObject Interpreter::visitBinaryExpr(Binary& expr) {
    LoxObject left = evaluate(expr.left);
    LoxObject right = evaluate(expr.right);

    switch(expr.operator_.token_type) {
        case TokenType::GREATER:
            return LoxObject(left > right);
        case TokenType::GREATER_EQUAL:
            return LoxObject(left >= right); 
        case TokenType::LESS:
            return LoxObject(left < right); 
        case TokenType::LESS_EQUAL:
            return LoxObject(left <= right); 
        case TokenType::MINUS:
            return left - right;
        case TokenType::PLUS:
            return left + right;
        case TokenType::SLASH:
            return left / right; 
        case TokenType::STAR:
            return left * right; 
        case TokenType::BANG_EQUAL: return LoxObject(left != right);
        case TokenType::EQUAL_EQUAL: return LoxObject(left == right);
        default:
            std::runtime_error("unknown binary expression");
    }

}

LoxObject Interpreter::visitVariableExpr(Variable& expr) {
    return environment.get(expr.name);
}

LoxObject Interpreter::visitAssignExpr(Assign& expr) {
    LoxObject value = evaluate(expr.value);
    environment.assign(expr.name, value);
    return value;
}

// Stmt
void Interpreter::visitExpressionStmt(Expression& stmt) {
    evaluate(stmt.expression);
}

void Interpreter::visitPrintStmt(Print& stmt) {
    LoxObject value = evaluate(stmt.expression);
    std::cout << value << '\n';
}

void Interpreter::visitVarStmt(Var& stmt) {
    LoxObject value;
    if (stmt.initializer) {
        value = evaluate(stmt.initializer);
    }
    environment.define(stmt.name.lexeme, value); 
}

void Interpreter::executeBlock(std::vector<std::unique_ptr<Stmt>>& statements, PEnvironment env) {
    auto previous = environment;
    try {
        environment = env;

        for (auto& statement: statements) {
            execute(statement);
        }
    } catch (...) {
        environment = previous;
    }
    environment = previous; // @TODO change this bad code
}

void Interpreter::visitBlockStmt(Block& stmt) {
    auto env = std::make_shared<Environment>(environment);
    executeBlock(stmt.statements, env);
}



void Interpreter::interpret(std::vector<std::unique_ptr<Stmt>>& statements) {
    try
    {
        for (auto& stmt : statements) {
            execute(stmt);
        }
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << '\n';
        Lox::runtimeError();
    }
}

}