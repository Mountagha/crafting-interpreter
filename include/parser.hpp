#pragma once

#include <vector>
#include <memory>
#include <exception>
#include "Expr.hpp"
#include "token.hpp"
#include "lox.hpp"
#include "Stmt.hpp"


namespace lox {

class Parser {
    public:
        Parser(std::vector<Token>& tokens_ );
        std::vector<std::unique_ptr<Stmt>> parse();

    private:
        using PExpr = std::unique_ptr<Expr>; 
        using SExpr = std::unique_ptr<Stmt>;
        struct ParseError : std::runtime_error { using std::runtime_error::runtime_error; }; // constructor inheritance

        std::vector<Token> tokens;
        unsigned int current;

        PExpr expression() {
            return equality();
        }

        SExpr statement() {
            if (match ({PRINT})) return printStatement();

            return expressionStatement();
        }        

        SExpr printStatement() {
            PExpr value = expression();
            consume(SEMICOLON, "Expect ';' after value.");
            return std::make_unique<Print>(std::move(value));
        }

        SExpr expressionStatement() {
            PExpr expr = expression();
            consume(SEMICOLON, "Expect ';' after expression.");
            return std::make_unique<Expression>(std::move(expr));
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
            return peek().token_type == EOFILE;
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
            if (match({FALSE})) return std::make_unique<Literal>(LoxObject(false));
            if (match({TRUE})) return std::make_unique<Literal>(LoxObject(true));
            if (match({NIL})) return std::make_unique<Literal>(LoxObject());
            
        

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