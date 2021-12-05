#pragma once

#include <string>
#include <iostream>
#include "token.hpp"

using namespace lox;

// not currently used due to hasError variable. Need to figure this out later
static void report (int line, std::string where, std::string message){
    std::cerr << "[line " << line 
            << "] Error" << where
            << ": " << message << std::endl;
}
static void error(int line, std::string message) {
    report(line, "", message);
}

static void error(Token token, std::string message) {
    if (token.token_type == EOF)
        report(token.line, " at end", message);
    else 
        report (token.line, " at '" + token.lexeme + "'", message);
}