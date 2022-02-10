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
        Environment(PEnvironment enclosing);
        void define(std::string s, LoxObject value);
        void assign(Token name, LoxObject value);
        static PEnvironment createNew(PEnvironment encl);
        static PEnvironment copy(PEnvironment env, PEnvironment encl);
        PEnvironment copy();

        LoxObject get(Token name);   
        ~Environment();
        
        PEnvironment enclosing;
    private:
        std::unordered_map<std::string, LoxObject> values{}; 
};

} // namespace lox