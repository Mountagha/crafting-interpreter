#include "token.hpp"

namespace lox {

Token::Token(TokenType token_type, std::string lexeme, std::string literal, unsigned int line):
    token_type{token_type}, lexeme{lexeme}, literal{literal}, line{line}
{}

inline std::string Token::to_string() const {
    return std::to_string(token_type) + " " + lexeme + " " + literal;
}

std::ostream& operator<<(std::ostream& os, const Token& token){
    os << token.to_string();
    return os;
}

}