#include "token.hpp"

namespace lox {

Token::Token(TokenType token_type, std::string lexeme, std::any literal, unsigned int line):
    token_type{token_type}, lexeme{lexeme}, literal{literal}, line{line}
{}

inline std::string Token::enum_to_string(TokenType token) const {
    const std::string enum_strings[] = {"LEFT_PARENT", "RIGHT_PARENT", 
    "LEFT_BRACE", "RIGHT_BRACE", "COMMA", "DOT", "MINUS", "PLUS", "SEMICOLON",
    "SLASH", "STAR", "BANG", "BANG_EQUAL", "EQUAL", "EQUAL_EQUAL",
    "GREATER", "GREATER_EQUAL", "LESS", "LESS_EQUAL", "IDENTIFIER", 
    "STRING", "NUMBER", "AND", "CLASS", "ELSE", "FALSE", "FUN", "FOR",
    "IF", "NIL", "OR", "PRINT", "RETURN", "SUPER", "THIS", "TRUE", 
    "VAR", "WHILE", "EOFILE"};
    return enum_strings[token];
}

std::ostream& operator<<(std::ostream& os, const Token& token){
    std::string literal_value;
    switch(token.token_type) {
        case (IDENTIFIER):
            literal_value = token.lexeme;
            break;
        case (STRING):
            literal_value = std::any_cast<std::string>(token.literal);
            break;
        case (NUMBER):
            literal_value = std::to_string(std::any_cast<double>(token.literal));
            break;
        case (TRUE):
            literal_value = "true";
            break;
        case (FALSE):
            literal_value = "false";
            break;
        default:
            literal_value = "nil";
            break;
    }
    os << token.enum_to_string(token.token_type) + " " + \
    token.lexeme + " " + literal_value;
    return os;
}

} // namespace lox