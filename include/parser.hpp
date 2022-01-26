#pragma once

#include <vector>
#include <memory>
#include <exception>
#include "Expr.hpp"
#include "token.hpp"
#include "lox.hpp"


namespace lox {

class Parser {
    public:
        Parser(std::vector<Token>& tokens_ );
        std::unique_ptr<Expr> parse();

    private:
        using PExpr = std::unique_ptr<Expr>; 
        struct ParseError : std::runtime_error { using std::runtime_error::runtime_error; }; // constructor inheritance

        std::vector<Token> tokens;
        unsigned int current;

        PExpr expression() {
            return equality();
        }

        PExpr equality() {
            PExpr expr = comparison();

            while ( match ({BANG_EQUAL, EQUAL_EQUAL})) {
                Token operator_ = previous();
                PExpr right = comparison();
                expr = std::make_unique<Binary>(std::move(expr), operator_, std::move(right)); 
            }

            return expr;
        }

        bool match(const std::vector<TokenType>& types) {
            for (const auto& type_ : types) {
                if (check(type_)) {
                    advance();
                    return true;
                }
            }

            return false;
        }

        bool check(const TokenType& type_) {
            if (isAtEnd()) return false;
            return peek().token_type == type_;
        }

        Token advance() {
            if (!isAtEnd()) current++;
            return previous();
        }

        bool isAtEnd() {
            return peek().token_type == EOF;
        }

        Token peek() {
            return tokens[current];
        }

        Token previous() {
            return tokens[current - 1];
        }

        PExpr comparison() {
            PExpr expr = term();

            while (match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
                Token operator_ = previous();
                PExpr right = term();
                expr = std::make_unique<Binary>(std::move(expr), operator_, std::move(right));
            }

            return expr;
        }

        PExpr term() {
            PExpr expr = factor();

            while (match ({MINUS, PLUS})) {
                Token operator_ = previous();
                PExpr right = factor();
                expr = std::make_unique<Binary>(std::move(expr), operator_, std::move(right));
            }

            return expr;
        }

        PExpr factor() {
            PExpr expr = unary();

            while (match ({SLASH, STAR})) {
                Token operator_ = previous();
                PExpr right = unary();
                expr = std::make_unique<Binary>(std::move(expr), operator_, std::move(right));
            }

            return expr;
        }

        PExpr unary() {
            if (match ({BANG, MINUS})) {
                Token operator_ = previous();
                PExpr right = unary();
                return std::make_unique<Unary>(operator_, std::move(right));
            }

            return primary();
        }

        PExpr primary() {
            if (match({FALSE})) return std::make_unique<Literal>(false);
            if (match({TRUE})) return std::make_unique<Literal>(true);
            if (match({NIL})) return std::make_unique<Literal>(nullptr);
            
        

            if (match({NUMBER, STRING})) 
                return std::make_unique<Literal>(LoxObject(previous()));
            
            if (match({LEFT_PAREN})) {
                PExpr expr = expression();
                consume(RIGHT_PARENT, "Expect ')' after expression.");
                return std::make_unique<Grouping>(std::move(expr));
            }

            throw error(peek(), "Expect expression.");
        }

        Token consume(TokenType token_type, std::string message) {
            if (check(token_type)) return advance();

            throw error(peek(), message);
        }

        ParseError error(Token token, std::string message) {
            Lox::error(token, message);

            return ParseError{message};
        }

        void synchronize() {
            advance();

            while (!isAtEnd()) {
                if (previous().token_type == SEMICOLON) return;

                switch (peek().token_type) {
                    case CLASS:
                    case FUN:
                    case VAR:
                    case FOR:
                    case IF:
                    case WHILE:
                    case PRINT:
                    case RETURN:
                        return;
                }

                advance();
            }
            
        }
};

} // Namespace lox