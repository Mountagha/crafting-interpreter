#include <iostream> // for debugging
#include <fstream>
#include <sstream>
#include "scanner.hpp"
#include "lox.hpp"

namespace lox{

bool Lox::hadError{false};

void Lox::report (int line, std::string where, std::string message){
    std::cerr << "[line " << line 
            << "] Error" << where
            << ": " << message << std::endl;
    hadError = true;
}

void Lox::error(int line, std::string message) {
    report(line, "", message);
}

void Lox::run (std::string source) {
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();

    // for now we just print the tokens. maybe use references later
    for (Token token: tokens){
        std::cout << token << std::endl;
    }
}
void Lox::runFile (std::string path) {
    std::ifstream in {path};
    if(!in) { // handle this better later
        std::cerr << "file not found" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::stringstream sstr;
    sstr << in.rdbuf();
    std::string content = sstr.str();
    run(content);    
}

void Lox::runPrompt() {
    for (;;){
        std::cout << "> ";
        std::string line;
        if(!getline(std::cin, line)) break;
        run(line);
    }
}

}  // namespace lox