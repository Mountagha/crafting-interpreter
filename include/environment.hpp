#pragma once 

#include <unordered_map>
#include "loxObject.hpp"
#include "token.hpp"
#include <memory>


namespace lox {
class Environment;
using PEnvironment = std::shared_ptr<Environment>;


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