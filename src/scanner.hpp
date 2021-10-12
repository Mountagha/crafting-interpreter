#pragma once 

#include "token.hpp"
#include "lox.hpp"
#include <vector>

namespace lox {
    
class Scanner {

    public:
        Scanner(std::string source);
        std::vector<Token> scanTokens();

    private:
        unsigned int start;
        unsigned int current;
        unsigned int line;
        std::string source;
        std::vector<Token> tokens;

        inline bool isAtEnd() const { return current > source.length(); }

        void scanToken() {
            char c = advance();
            switch (c){ 
                case '(': addToken(LEFT_PAREN); break;
                case ')': addToken(RIGHT_PARENT); break;
                case '{': addToken(LEFT_BRACE); break;
                case '}': addToken(RIGHT_BRACE); break;
                case ',': addToken(COMMA); break;
                case '.': addToken(DOT); break;
                case '-': addToken(MINUS); break;
                case '+': addToken(PLUS); break;
                case ';': addToken(SEMICOLON); break;
                case '*': addToken(STAR); break;
                default:
                    Lox::error(line, "Unexpected character");
                    break;
            }
        }

        inline char advance() const {
            return source.at(current);
        }

        void addToken(TokenType token_type){
            addToken(token_type, "");
        }

        void addToken(TokenType token_type, std::string literal) {
            std::string text = source.substr(start, current-start+1);
            tokens.push_back(Token(token_type, text, literal, line));
        }

};
}