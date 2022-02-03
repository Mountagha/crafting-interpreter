#pragma once 

#include <unordered_map>
#include "loxObject.hpp"
#include "token.hpp"


namespace lox {

class Environment {
    public:
        void define(std::string s, LoxObject value);
        LoxObject get(Token name);   
    private:
        std::unordered_map<std::string, LoxObject> values{}; 
};

} // namespace lox