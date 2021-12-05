#pragma once

#include "Expr.hpp"
#include "token.hpp"
#include <vector>
#include <memory>


namespace lox {
using namespace lox::AST;

class Parser {
    public:
        Parser(std::vector<Token>& tokens_ ): tokens{tokens_}, current{0} {}

    private:

        using PExpr = std::unique_ptr<Expr>; 
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
                return std::make_unique<Unary>(operator_, right);
            }

            return primary();
        }

        PExpr primary() {
            if (match({FALSE})) return std::make_unique<Literal>(false);
            if (match({TRUE})) return std::make_unique<Literal>(true);
            if (match({NIL})) return std::make_unique<Literal>();
            
        

            if (match({NUMBER, STRING})) 
                return std::make_unique<Literal>(previous().literal);
            
            if (match({LEFT_PAREN})) {
                PExpr expr = expression();
                consume(RIGHT_PARENT, "Expect ')' after expression.");
                return std::make_unique<Grouping>(expr);
            }
        }
};

} // Namespace lox