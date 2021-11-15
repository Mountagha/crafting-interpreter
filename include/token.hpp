#pragma once

#include <string>
#include <any>

namespace lox {

enum TokenType {
    // Single character tokens
    LEFT_PAREN, RIGHT_PARENT, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    // One or two character tokens
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // literas
    IDENTIFIER, STRING, NUMBER,

    // Keywords.
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

    EOFILE
};

class Token {
    public:
        Token(TokenType token_type, std::string lexeme, std::any literal, unsigned int line);
        inline std::string enum_to_string(TokenType token) const;
        friend std::ostream& operator<<(std::ostream& os, const Token& token);
    private:
        TokenType token_type;
        std::string lexeme;
        std::any literal; 
        unsigned int line;

};

} // lox namespace

