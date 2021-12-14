#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include "Expr.hpp"


namespace lox {

class ASTprinter: public Visitor {
    public:
        std::string print(std::unique_ptr<Expr>& expr) {
            return std::any_cast<std::string>(expr->accept(*this));
        }

        std::any visitBinaryExpr(Binary& expr) override {
            return parenthesize(expr.operator_.lexeme, {expr.left.get(), expr.right.get()});
        }

        std::any visitGroupingExpr(Grouping& expr) override {
            return parenthesize("group", {expr.expression.get()});
        }

        std::any visitLiteralExpr(Literal& expr) override {
            if ( expr.value.has_value() ) {
                auto& literal_type = expr.value.type();
                if (literal_type == typeid(double))
                    return std::to_string(std::any_cast<double>(expr.value));
                else if (literal_type == typeid(int))
                    return std::to_string(std::any_cast<int>(expr.value));
                else if (literal_type == typeid(std::string))
                    return std::any_cast<std::string>(expr.value);
                else if (literal_type == typeid(bool))
                    return std::any_cast<bool>(expr.value) ? std::string("true") : std::string("false");
                else 
                    return std::string("unrecognized literal type");
            } else // not value 
                return std::string("nil");
        }

        std::any visitUnaryExpr(Unary& expr) override {
            return parenthesize(expr.operator_.lexeme, {expr.right.get()});
        }
    private: 
        std::string parenthesize(std::string name, const std::vector<Expr*>& exprs){
            std::stringstream ss;
            ss << "(" + name;
            for (const auto& expr: exprs) {
                ss << " ";
                ss << std::any_cast<std::string>(expr->accept(*this));
            }
            ss << ")";
            return std::any_cast<std::string>(ss.str());
        }
};

} // namespace lox
/*
this was just for testing.

using namespace lox;

int main(int argc, char *argv[]){
    std::unique_ptr<Expr> expression = std::make_unique<Binary>(
                                        std::make_unique<Unary>(
                                        Token{MINUS, "-", "", 1},
                                        std::make_unique<Literal>(123)
                                        ),
                                        Token{STAR, "*", "", 1},
                                        std::make_unique<Grouping>(
                                            std::make_unique<Literal>(45.76)
                                        )
    ); 
    std::cout << ASTprinter{}.print(expression) << '\n';
}

*/
