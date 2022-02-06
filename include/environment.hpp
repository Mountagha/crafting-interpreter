#pragma once 

#include <unordered_map>
#include "loxObject.hpp"
#include "token.hpp"


namespace lox {

class Environment {
    public:
        Environment();
        Environment(Environment* enclosing);
        void define(std::string s, LoxObject value);
        void assign(Token name, LoxObject value);
        LoxObject get(Token name);   
        ~Environment();
    private:
        Environment* enclosing;
        std::unordered_map<std::string, LoxObject> values{}; 
};

} // namespace lox