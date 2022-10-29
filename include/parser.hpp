#pragma once

#include <vector>
#include <memory>
#include <exception>
#include "Expr.hpp"
#include "token.hpp"
#include "lox.hpp"
#include "Stmt.hpp"
#include "type.hpp"


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
        //We use the boolean variable below to prevent commaOperator to parse inside function call. 
        // or on Maybe there's a better way to do that ? I DUNNO.
        bool isCall{false}; // 

        PExpr expression() {
            PExpr expr = assignment();
            if(check(COMMA) && !isCall) {
                return CommaBlock(std::move(expr));
            }
            return expr;
        }

        SExpr declaration() {
            try {
                if (match ({CLASS})) return classDeclaration();
                if (match ({FUN})) return function("function");
                if (match ({VAR})) return varDeclaration(); 

                return statement();
            } catch (ParseError error) {
                synchronize();
                return SExpr{};
            }
        }

        SExpr classDeclaration() {
            Token name = consume(IDENTIFIER, "Expect class name.");
            // handle inheritance
            PExpr superclass;
            if (match({LESS})) {
                consume(IDENTIFIER, "Expect superclass name.");
                superclass = std::make_unique<Variable>(previous());
            }

            consume(LEFT_BRACE, "Expect '{' before class body.");

            std::vector<std::unique_ptr<Function>> methods;
            while (!check(RIGHT_BRACE) && !isAtEnd()) {
                SExpr m;
                if (match ({CLASS})) { // class method declaration.
                    m = function("class_method");
                } else { // object method declaration.
                    m = function("method");
                }
                methods.push_back(std::unique_ptr<Function>(static_cast<Function*>(m.release())));
            }

            consume(RIGHT_BRACE, "Expect '}' after class body.");

            return std::make_unique<Class>(name, std::move(superclass), std::move(methods));
        }

        SExpr statement() {
            if (match ({FOR})) return forStatement();
            if (match ({IF})) return ifStatement();
            if (match ({PRINT})) return printStatement();
            if (match ({RETURN})) return returnStatement();
            if (match ({WHILE})) return whileStatement();
            if (match ({LEFT_BRACE})) return std::make_unique<Block>(block());

            return expressionStatement();
        }

        SExpr forStatement() {
            consume(LEFT_PAREN, "Expect '(' after 'for'.");

            SExpr initalizer;
            if (match ({SEMICOLON})); // do nothing i.e keep initializer to nullptr
            else if (match ({VAR})) {
                initalizer = varDeclaration();
            } else {
                initalizer = expressionStatement();
            }

            PExpr condition;
            if ( !check(SEMICOLON)) {
                condition = expression();
            }
            consume(SEMICOLON, "Expect ';' after loop condition.");

            PExpr increment;
            if (!check(RIGHT_PARENT)) {
                increment = expression();
            }
            consume(RIGHT_PARENT, "Expect ')' after for clauses.");

            SExpr body = statement();

            if (increment) {
                std::vector<SExpr> statements;
                statements.push_back(std::move(body));
                statements.push_back(std::make_unique<Expression>(std::move(increment)));
                body = std::make_unique<Block>(std::move(statements));
            }

            if (!condition) condition = std::make_unique<Literal>(LoxObject(true));
            body = std::make_unique<While>(std::move(condition), std::move(body));

            if (initalizer) {
                std::vector<SExpr> statements;
                statements.push_back(std::move(initalizer));
                statements.push_back(std::move(body));
                body = std::make_unique<Block>(std::move(statements));
            }
            return body;   
            
        }        

        SExpr ifStatement() {
            consume(LEFT_PAREN, "Expect '(' after 'if'.");
            PExpr condition = expression();
            consume(RIGHT_PARENT, "Expect ')' after if condition.");

            SExpr thenBranch = statement();
            SExpr elseBranch;
            if (match ({ELSE})) {
                elseBranch = statement();
            }

            return std::make_unique<If>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
        }

        SExpr printStatement() {
            PExpr value = expression();
            consume(SEMICOLON, "Expect ';' after value.");
            return std::make_unique<Print>(std::move(value));
        }

        SExpr returnStatement() {
            Token keyword = previous();
            PExpr value;
            if (!check(SEMICOLON)) {
                value = expression();
            }

            consume(SEMICOLON, "Expect ';' after return value.");
            return std::make_unique<Return>(keyword, std::move(value));
        }

        SExpr varDeclaration() {
            Token name = consume(IDENTIFIER, "Expect variable name.");

            PExpr initializer;
            if (match ({EQUAL})) {
                initializer = assignment(); //we changed expression() by assignment
                // just to avoid code like [var x = 1, 2, 3;]
            }

            consume(SEMICOLON, "Expect ';' after variable declaration.");
            return std::make_unique<Var>(name, std::move(initializer));
        }

        SExpr whileStatement() {
            consume(LEFT_PAREN, "Expect '(' after a 'while'.");
            PExpr condition = expression();
            consume(RIGHT_PARENT, "Expect ')' after condition");
            SExpr body = statement();

            return std::make_unique<While>(std::move(condition), std::move(body));
        }

        SExpr expressionStatement() {
            PExpr expr = expression();
            consume(SEMICOLON, "Expect ';' after expression.");
            return std::make_unique<Expression>(std::move(expr));
        }

        SExpr function(std::string kind) {
            Token name = consume(IDENTIFIER, "Expect " + kind + " name");
            consume(LEFT_PAREN, "Expect '(' after " + kind + " name.");
            std::vector<Token> parameters {};
            if (!check(RIGHT_PARENT)){
                do {
                    if (parameters.size() >= 255) {
                        Lox::error(peek(), "Can't have more than 255 parameters");
                    }

                    parameters.push_back(
                        consume(IDENTIFIER, "Expect parameter name.")
                    );
                } while (match ({COMMA}));
            }
            consume(RIGHT_PARENT, "Expect ')' after parameteres.");

            consume(LEFT_BRACE, "Expect '{' before " + kind + " body.");
            std::vector<SExpr> body = block();
            return std::make_unique<Function>(name, std::move(parameters), std::move(body));
        }

        std::vector<SExpr> block() {
            std::vector<SExpr> statements{};
            while(!check(RIGHT_BRACE) && !isAtEnd()) {
                statements.push_back(declaration());
            }
            consume(RIGHT_BRACE, "Expect '}' after block.");
            return statements;
        }

        PExpr assignment(){
            // PExpr expr = Or();
            PExpr expr = ternary();

            if (match ({EQUAL})) {
                Token equals = previous();
                PExpr value = assignment();

                TypeIdentifier identifier{};
                if (identifier.identify(expr) == Type::Variable) {
                    Token name = static_cast<Variable*>(expr.get())->name;
                    return std::make_unique<Assign>(name, std::move(value));
                } else if (identifier.identify(expr) == Type::Get) {
                    auto getExpr = std::unique_ptr<Get>(static_cast<Get*>(expr.release()));
                    return std::make_unique<Set>(std::move(getExpr->object), getExpr->name, std::move(value));
                } 
                Lox::error(equals, "Invalid assignment target.");
            }
            return expr;
        }

        PExpr ternary() {
            PExpr expr = Or();
            
            if (match ({QUESTION_MARK})) {
                PExpr thenBranch = expression();
                consume(COLON, "Expect ':' after expression in ternary.");
                PExpr elseBranch = expression();
                return std::make_unique<Ternary>(std::move(expr), std::move(thenBranch), std::move(elseBranch));
            }
            return expr;
        }

        PExpr Or() {
            PExpr expr = And();

            while (match ({OR})) {
                Token operator_ = previous();
                PExpr right = And();
                expr = std::make_unique<Logical>(
                        std::move(expr), operator_, std::move(right));
            }
            return expr;
        }

        PExpr And() {
            PExpr expr = equality();

            while (match ({AND})) {
                Token operator_ = previous();
                PExpr right = equality();
                expr = std::make_unique<Logical>(
                        std::move(expr), operator_, std::move(right));
            }

            return expr;
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

            return call();
        }

        PExpr call() {
            PExpr expr = primary();

            while (true){
                if (match ({LEFT_PAREN})) {
                    isCall = true;
                    expr = finishCall(expr);
                } else if (match ({DOT})) {
                    Token name = consume(IDENTIFIER,
                        "Expect propert name after '.'.");
                    expr = std::make_unique<Get>(std::move(expr), name);
                } else {
                    break;
                }
            }

            return expr;
        }

        PExpr CommaBlock(PExpr&& left_hand) {
            consume(COMMA, "Expect ',' while parsing comma block.");
            std::vector<PExpr> commaExps; 
            commaExps.push_back(std::move(left_hand));

            do {
                commaExps.push_back(expression());
            } while(match ({COMMA}) && !isAtEnd());

            return std::make_unique<CommaExpr>(std::move(commaExps));
        }

        PExpr finishCall(PExpr& callee) {
            std::vector<PExpr> arguments;
            if (!check(RIGHT_PARENT)) {
                do {
                    if (arguments.size() >= 255) {
                        Lox::error(peek(), "Can't have more than 255 arguments");
                    }
                    arguments.push_back(expression());
                } while(match({COMMA}));
            }
            isCall = false; 
            Token paren = consume(RIGHT_PARENT, "Expect ')' after arguments.");
            return std::make_unique<Call>(std::move(callee), paren, std::move(arguments));
        }

        PExpr primary() {
            if (match({FALSE})) return std::make_unique<Literal>(LoxObject(false));
            if (match({TRUE})) return std::make_unique<Literal>(LoxObject(true));
            if (match({NIL})) return std::make_unique<Literal>(LoxObject());

            if (match({THIS})) return std::make_unique<This>(previous());
            
            if (match({IDENTIFIER})) return std::make_unique<Variable>(previous());

            if (match({NUMBER, STRING})) 
                return std::make_unique<Literal>(LoxObject(previous()));
            
            if (match({LEFT_PAREN})) {
                PExpr expr = expression();
                consume(RIGHT_PARENT, "Expect ')' after expression.");
                return std::make_unique<Grouping>(std::move(expr));
            }

            if (match({SUPER})) {
                Token keyword = previous();
                consume(DOT, "Expect '.' after 'super'.");
                Token method = consume(IDENTIFIER, "Expect superclass method name.");
                return std::make_unique<Super>(keyword, method);
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