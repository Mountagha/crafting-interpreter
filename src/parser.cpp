#include "parser.hpp"

namespace lox {

Parser::Parser(std::vector<Token>& tokens_ ): tokens{tokens_}, current{0} {}

std::vector<std::unique_ptr<Stmt>> Parser::parse() {
    std::vector<std::unique_ptr<Stmt>> statements;
    while(!isAtEnd()){
        statements.push_back(declaration());
    }
    return statements;
}

}