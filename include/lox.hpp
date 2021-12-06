#pragma once

#include "token.hpp"

namespace lox {

class Lox {
    public:
       static void report (int line, std::string where, std::string message);
       static void run (const std::string& source);
       static void runFile (std::string path);
       static void runPrompt();
       static void error(int line, std::string message);
       static void error(Token token, std::string message);
    private:
        static bool hadError; 
};

}