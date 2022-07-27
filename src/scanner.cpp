#include "scanner.hpp"


namespace lox {

Scanner::Scanner(const std::string& source): 
    source{source}, start{0}, current{0}, line{1} 
{}

std::vector<Token> Scanner::scanTokens() {
    while(!isAtEnd()){
        // We are at the beginning of the next lexeme
        start = current;
        scanToken();
    }
    tokens.push_back(Token(EOFILE, "", line));
    return tokens;
}
}