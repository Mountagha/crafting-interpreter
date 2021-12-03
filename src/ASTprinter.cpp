#include <iostream>
#include "../include/Expr.hpp"
#include <vector>
#include <sstream>
// #include "../include/token.hpp"


namespace lox {
using namespace AST;

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
                if (literal_type == typeid(double) or literal_type == typeid(int))
                    return std::any_cast<double>(expr.value);
                else if (literal_type == typeid(std::string))
                    return std::any_cast<std::string>(expr.value);
                else if (literal_type == typeid(bool))
                    return std::any_cast<bool>(expr.value) ? "true" : "false";
                else 
                    return "unrecognized literal type";
            } else // not value 
                return "nil";
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
            return std::any_cast<std::string>(ss.str());
        }
};

} // namespace lox

using namespace lox;

int main(int argc, char *argv[]){
    /*std::unique_ptr<Expr> expression = std::make_unique<Binary>(
                                        std::make_unique<Unary>(
                                        Token{MINUS, "-", "", 1},
                                        std::make_unique<Literal>(123.)
                                        ),
                                        Token{STAR, "*", "", 1},
                                        std::make_unique<Grouping>(
                                            std::make_unique<Literal>(45.76)
                                        )
    );*/ 
    std::unique_ptr<Expr> expression = std::make_unique<Unary>(Token{MINUS, "-", "", 1}, std::make_unique<Literal>(123.));
    std::cout << ASTprinter{}.print(expression);
}
