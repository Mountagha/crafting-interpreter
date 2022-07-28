#include "token.hpp"

namespace lox {

Token::Token(TokenType token_type, std::string lexeme, unsigned int line):
    token_type{token_type}, lexeme{lexeme}, line{line}
{}

inline std::string Token::enum_to_string(TokenType token) const {
    const std::string enum_strings[] = {"LEFT_PARENT", "RIGHT_PARENT", 
    "LEFT_BRACE", "RIGHT_BRACE", "COMMA", "DOT", "MINUS", "PLUS", "SEMICOLON",
    "SLASH", "STAR", "COLON", "QUESTION_MARK","BANG", "BANG_EQUAL", "EQUAL", "EQUAL_EQUAL",
    "GREATER", "GREATER_EQUAL", "LESS", "LESS_EQUAL", "IDENTIFIER", 
    "STRING", "NUMBER", "AND", "CLASS", "ELSE", "FALSE", "FUN", "FOR",
    "IF", "NIL", "OR", "PRINT", "RETURN", "SUPER", "THIS", "TRUE", 
    "VAR", "WHILE", "EOFILE"};
    return enum_strings[token];
}

std::ostream& operator<<(std::ostream& os, const Token& token){
    os << token.enum_to_string(token.token_type) + " " + \
    token.lexeme + "\n" ;
    return os;
}

} // namespace lox