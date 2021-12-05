#include "parser.hpp"

namespace lox {

Parser::Parser(std::vector<Token>& tokens_ ): tokens{tokens_}, current{0} {}

PExpr Parser::parse() {
    try {
        return expression();
    } catch (ParseError error) {
        return nullptr;
    }
}

}