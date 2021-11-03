#include <iostream> // for debugging purposes
#include "include/lox.hpp"

using namespace lox;


int main(int argc, char *argv[]) {
    if (argc > 2) {
        std::cerr << "Usage: jlox [script]" << std::endl;
        exit(64);
    } else if (argc == 2){
        Lox::runFile(argv[1]);
    } else {
        Lox::runPrompt();
    }
    return 0;
}