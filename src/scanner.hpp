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
                // operator
                case '!':
                    addToken(match('=') ? BANG_EQUAL : BANG);
                    break;
                case '=':
                    addToken(match('=') ? EQUAL_EQUAL : EQUAL);
                    break;
                case '<':
                    addToken(match('=') ? LESS_EQUAL : LESS);
                    break;
                case '>':
                    addToken(match('=') ? GREATER_EQUAL : GREATER);
                    break;
                case '/':
                    if (match('/')) {
                        // A comment goes until the end of the line
                        while (peek() != '\n' && !isAtEnd()) advance();
                    } else {
                        addToken(SLASH);
                    }
                    break;
                case ' ':
                case '\r':
                case '\t':
                    // ignore whitespaces
                    break;
                case '\n':
                    line++;
                
                // literals
                case '"': string(); break;
                default:
                    if (isDigit(c)) {
                        number();
                    } else {
                        Lox::error(line, "Unexpected character");
                    }
                    break;
            }
        }

        inline bool isDigit (char c) const {
            return c >= '0' && c <= '9';
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

        bool match(char expected) {
            if (isAtEnd()) return false;
            if (source.at(current) != expected) return false;
            current++;
            return true;
        }

        char peek() {
            if (isAtEnd()) return '\0';
            return source.at(current);
        } 

        void string() {
            while(peek() != '"' && !isAtEnd()){
                if(peek() == '\n') line++;
                advance();
            }

            if(isAtEnd()) {
                Lox::error(line, "Unterminted string.");
                return;
            }

            // the closing ".
            advance();

            // trim the surrounding quotes
            std::string value = source.substr(start+1, current-start);
            addToken(STRING, value);
        }

        void number() {
            while(isDigit(peek())) advance();

            // look for a fractional part.
            if (peek() == '.' && isDigit(peekNext())){
                // consume the "."
                advance();
                while(isDigit(peek())) advance();
            }

            addToken(NUMBER, std::stod(source.substr(start, current-start+1)));

        }



};
}