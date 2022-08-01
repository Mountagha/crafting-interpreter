#pragma once 

#include "token.hpp"
#include "lox.hpp"
#include <vector>
#include <map>
#include <stack>
#include <iostream>

namespace lox {
    
class Scanner {

    public:
        Scanner(const std::string& source);
        std::vector<Token> scanTokens();

    private:
        unsigned int start;
        unsigned int current;
        unsigned int line;
        const std::string& source;
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
                case ':': addToken(COLON); break;
                case '?': addToken(QUESTION_MARK); break;
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
                    } else if (match('*')) {
                        // A C/C++ long comment like with possibility of nesting
                        std::stack<int> stack;
                        stack.push(1);
                        while(!stack.empty() && !isAtEnd()) {
                            if (peek() == '\n') line++;
                            else if (peek() == '/' && peekNext() == '*'){
                                stack.push(1);
                                advance();
                            }
                            else if (peek() == '*' && peekNext() == '/') {
                                stack.pop();
                                advance();
                            }
                            advance();
                        }
                        if (!stack.empty()) { 
                            Lox::error(line, "Unterminated comment");
                            return;
                        }
                    } else {
                        addToken(SLASH);
                    }
                    break;
                case ' ':
                case '\r':
                case '\t':
                case '\0':
                    // ignore whitespaces && nul characters
                    break;
                case '\n':
                    line++;
                    break;
                
                // literals
                case '"': string(); break;
                default:
                    if (isDigit(c)) {
                        number();
                    } else if (isAlpha(c)) {
                        identifier();
                    } else {
                        Lox::error(line, "Unexpected character");
                    }
                    break;
            }
        }

        inline bool isDigit (char c) const {
            return c >= '0' && c <= '9';
        }

        inline bool isAlpha(char c) {
            return (c >= 'a' && c <= 'z' ||
                    c >= 'A' && c <= 'Z' ||
                    c == '_');
        }
        
        inline bool is_alphanumeric(char c){
            return isAlpha(c) || isDigit(c);
        }

        inline char advance() {
            return source[current++];
        }

        void addToken(TokenType token_type, std::string lexeme, unsigned int line){
            tokens.push_back({token_type, lexeme, line});
        }

        void addToken(TokenType token_type) {
            std::string text = source.substr(start, current-start);
            addToken(token_type, text, line);
        }
        
        // @TODO: Handle it with std::optional & std::variant later maybe
        //void addToken(TokenType token_type, double n) {
        //    tokens.push_back(Token())
        //}

        bool match(char expected) {
            if (isAtEnd()) return false;
            if (source[current] != expected) return false;
            current++;
            return true;
        }

        char peek() {
            if (isAtEnd()) return '\0';
            return source[current];
        } 

        void string() {
            while(peek() != '"' && !isAtEnd()){
                if(peek() == '\n') line++;
                advance();
            }

            if(isAtEnd()) {
                Lox::error(line, "Unterminated string.");
                return;
            }

            // the closing ".
            advance();

            // trim the surrounding quotes
            std::string value = source.substr(start+1, current-start-2);
            addToken(STRING, value, line);
        }

        void number() {
            while(isDigit(peek())) advance();

            // look for a fractional part.
            if (peek() == '.' && isDigit(peekNext())){
                // consume the "."
                advance();
                while(isDigit(peek())) advance();
            }

            addToken(NUMBER); // later stock as double

        }

        char peekNext() {
            if (current + 1 >= source.size()) return '\0';
            return source[ current + 1 ];
        }

        void identifier() {
            while(is_alphanumeric(peek())) advance();
            std::string text = source.substr(start, current-start);
            addToken(reserved_or_identifier(text));
        }

        TokenType reserved_or_identifier(const std::string& str){
            static const std::map<std::string, TokenType> keywords{
                {"and", AND},
                {"class", CLASS},
                {"else", ELSE}, 
                {"false", FALSE},
                {"for", FOR},
                {"fun", FUN},
                {"if", IF},
                {"nil", NIL},
                {"or", OR},
                {"print", PRINT},
                {"return", RETURN},
                {"super", SUPER},
                {"this", THIS},
                {"true", TRUE},
                {"while", WHILE},
                {"var", VAR},
            };

            auto iter = keywords.find(str);
            if (iter == keywords.end()){
                return IDENTIFIER;
            }
            return iter->second;
        }

};
}