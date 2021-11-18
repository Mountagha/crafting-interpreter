#include <iostream>
#include "Expr.hpp"
#include <vector>
#include <sstream>


namespace lox {
using namespace AST;

class ASTprinter: public Visitor<std::string> {
    public:
        std::string print(Expr<std::string>& expr) {
            return expr.accept(*this);
        }

        std::string visitBinaryExpr(Binary<std::string>& expr) {
            return "a";
        }

        std::string visitGroupingExpr(Grouping<std::string>& expr){
            return "b";
        }

        std::string visitLiteralExpr(Literal<std::string> expr) {
            return "c";
        }

        std::string visitUnaryExpr(Unary<std::string> expr) {
            return "d";
        }
    private: 
        std::string parenthesize(std::string name, std::vector<Expr<std::string>> exprs){
            std::stringstream ss;
            ss << "(" + name;
            return ss.str();

        }
};

} // namespace lox

using namespace lox;

int main(int argc, char *argv[]){
    //Expr<std::string> expression()
}
