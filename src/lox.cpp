#include <iostream> // for debugging
#include <fstream>
#include <sstream>
#include <memory>
#include "scanner.hpp"
#include "lox.hpp"
#include "parser.hpp"
#include "ASTprinter.hpp"

namespace lox
{

    bool Lox::hadError{false};

    void Lox::report(int line, std::string where, std::string message)
    {
        std::cerr << "[line " << line
                  << "] Error" << where
                  << ": " << message << std::endl;
        hadError = true;
    }

    void Lox::error(int line, std::string message)
    {
        report(line, "", message);
    }

    void Lox::error(Token token, std::string message) {
        if (token.token_type == EOF)
            report(token.line, " at end", message);
        else 
            report (token.line, " at '" + token.lexeme + "'", message);
    }

    void Lox::run(const std::string &source)
    {
        Scanner scanner(source);
        std::vector<Token> tokens = scanner.scanTokens();

        for (Token token : tokens)
        {
            std::cout << token << std::endl;
        }
        Parser parser{tokens};
        std::unique_ptr<Expr> expression = parser.parse();

        // Stop if there was a syntax error.
        if (hadError) return;

        std::cout << ASTprinter{}.print(expression) << '\n';

    }
    void Lox::runFile(std::string path)
    {
        std::ifstream in{path};
        if (!in)
        { // handle this better later
            std::cerr << "file not found" << std::endl;
            exit(EXIT_FAILURE);
        }
        std::stringstream sstr;
        sstr << in.rdbuf();
        std::string content = sstr.str();
        run(content);
    }

    void Lox::runPrompt()
    {
        for (;;)
        {
            std::cout << "> ";
            std::string line;
            if (!getline(std::cin, line))
                break;
            //std::erase(std::find(line.begin(), line.end(), '\0'));
            run(line);
            // reset-had-error
            hadError = false;
        }
    }

} // namespace lox