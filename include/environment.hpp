#pragma once 

#include <unordered_map>
#include "loxObject.hpp"
#include "token.hpp"
#include <memory>


namespace lox {
class Environment;
using PEnvironment = std::shared_ptr<Environment>;


class Environment : public std::enable_shared_from_this<Environment>{
    public:
        Environment();
        Environment(PEnvironment enclosing);
        void define(std::string s, LoxObject value);
        void assign(Token name, LoxObject value);
        static PEnvironment createNew(PEnvironment encl);
        static PEnvironment copy(PEnvironment env, PEnvironment encl);
        LoxObject getAt(unsigned int distance, std::string name, int index=-1);
        void assignAt(unsigned int distance, Token name, LoxObject value);
        PEnvironment ancestor(unsigned int distance);
        PEnvironment copy();

        LoxObject get(Token name);   
        ~Environment();
        
        PEnvironment enclosing;
    private:
        std::unordered_map<std::string, LoxObject> values{}; 
        std::vector<LoxObject> my_values{};
};

class ScopeEnvironment {
    /* 
    This class allows us to handle the environments nesting by
    backing up each time we enter a new block and restoring back 
    when we exit 
    */
    public:
        ScopeEnvironment(PEnvironment& encl, PEnvironment newEnv);
        ~ScopeEnvironment();
    private:
        PEnvironment backup_env;
        PEnvironment& enclosing;
};

} // namespace lox