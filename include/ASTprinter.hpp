#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include "Expr.hpp"


namespace lox {

class ASTprinter: public Visitor {
    public:
        void print(std::unique_ptr<Expr>& expr) {
            expr->accept(*this);
            std::cout << ast_string << '\n';
        }

        LoxObject visitBinaryExpr(Binary& expr) override {
            parenthesize(expr.operator_.lexeme, {expr.left.get(), expr.right.get()});
            return LoxObject();
        }

        LoxObject visitGroupingExpr(Grouping& expr) override {
            parenthesize("group", {expr.expression.get()});
            return LoxObject();
        }

        LoxObject visitLiteralExpr(Literal& expr) override {
            ast_string += (std::string)expr.value;
            return LoxObject(); 
        }

        LoxObject visitUnaryExpr(Unary& expr) override {
            parenthesize(expr.operator_.lexeme, {expr.right.get()});
            return LoxObject();
        }
    private: 
        void parenthesize(std::string name, const std::vector<Expr*>& exprs){
            ast_string += "(" + name;
            for (const auto& expr: exprs) {
                ast_string +=  " ";
                expr->accept(*this);
            }
            ast_string += ")";
        }

        std::string ast_string;
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
    ASTprinter{}.print(expression);
}

*/
