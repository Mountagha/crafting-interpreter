#include "loxObject.hpp"
#include "lox.hpp"
#include <sstream>

namespace lox {

LoxObject::LoxObject(const LoxObject& o){
    string = o.string;
    number = o.number;
    lox_type = o.lox_type;
    boolean = o.boolean;
}

LoxObject& LoxObject::operator=(const LoxObject& o){
    string = o.string;
    number = o.number;
    lox_type = o.lox_type;
    boolean = o.boolean;
    return *this;
}

LoxObject::~LoxObject() {} // nothing for now.

LoxObject::LoxObject(Token token) {
    switch(token.token_type) {
        case TokenType::NIL:
            lox_type = LoxType::Nil;
            break;
        case TokenType::TRUE:
            lox_type = LoxType::Bool;
            boolean = true;
            break; 
        case TokenType::FALSE:
            lox_type = LoxType::Bool;
            boolean = false;
            break;
        case TokenType::NUMBER:
            lox_type = LoxType::Number;
            // limit scope of stringstream.
            {
                std::stringstream ss(token.lexeme);
                ss >> number;
            }
            break;
        case TokenType::STRING:
            lox_type = LoxType::String;
            string = token.lexeme;
            break;
        default:
            throw std::runtime_error("Invalid Lox Object"); 
    }
}
}