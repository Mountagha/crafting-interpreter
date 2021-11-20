#include <iostream>
#include "../include/Expr.hpp"
#include <vector>
#include <sstream>
#include "../include/token.hpp"


namespace lox {
using namespace AST;

class ASTprinter: public Visitor {
    public:
        std::any print(Expr& expr) {
            return std::any_cast<std::string>(expr.accept(*this));
        }

        std::any visitBinaryExpr(Binary& expr) override {
            // std::vector<std::unique_ptr<Expr>> v {expr.left, expr.right};
            return parenthesize(expr.operator_.lexeme, {expr.left, expr.right});
        }

        std::any visitGroupingExpr(Grouping& expr) override {
            return parenthesize("group", {expr.expression});
        }

        std::any visitLiteralExpr(Literal& expr) override {
            if ( expr.value.has_value() ) 
                return std::any_cast<std::string>(expr.value);
            else 
                return std::any_cast<std::string>("nil");
        }

        std::any visitUnaryExpr(Unary& expr) override {
            return parenthesize(expr.operator_.lexeme, {expr.right});
        }
    private: 
        std::any parenthesize(std::string name, const std::vector<std::shared_ptr<Expr>>& exprs){
            std::stringstream ss;
            ss << "(" + name;
            for (const auto& expr: exprs) {
                ss << " ";
                ss << std::any_cast<std::string>(expr->accept(*this));
            }
            return std::any_cast<std::string>(ss.str());
        }
};

} // namespace lox

using namespace lox;

int main(int argc, char *argv[]){
    std::shared_ptr<Expr> expression = std::make_shared<Binary>(
                                        std::make_shared<Unary>(
                                        Token{MINUS, "-", nullptr, 1},
                                        std::make_shared<Literal>(123.)
                                        ),
                                        Token{STAR, "*", nullptr, 1},
                                        std::make_shared<Grouping>(
                                            std::make_shared<Literal>(45.76)
                                        )
    );
    //std::cout << ASTprinter{}.print(expression);
}
