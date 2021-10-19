#pragma once

namespace lox {

class Lox {
    public:
       static void report (int line, std::string where, std::string message);
       static void run (std::string source);
       static void runFile (std::string path);
       static void runPrompt();
       static void error(int line, std::string message);
    private:
        static bool hadError; 
};

}