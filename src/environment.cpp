#include <string>
#include "environment.hpp"

namespace lox {

Environment::Environment() { enclosing = nullptr; }
Environment::Environment(PEnvironment environment) { enclosing = environment; }
Environment::~Environment () { }

PEnvironment Environment::createNew(PEnvironment encl) {
    return std::make_shared<Environment>(encl);
}

PEnvironment Environment::copy(PEnvironment env, PEnvironment encl) {
    auto newEnv = createNew(encl);
    newEnv->values = env->values;
    return newEnv;
}

PEnvironment Environment::copy() {
    auto newEnv = createNew(enclosing);
    newEnv->values = values;
    return newEnv;
}

void Environment::define(std::string s, LoxObject value) {
    values.insert_or_assign(s, value);
}

PEnvironment Environment::ancestor(unsigned int distance) {
    auto environment = shared_from_this();
    for (int i = 0; i < distance; i++) {
        environment = environment->enclosing;
    }
    return environment;
}

LoxObject Environment::getAt(unsigned int distance, std::string name) {
    return ancestor(distance)->values.at(name);
}

void Environment::assignAt(unsigned int distance, Token name, LoxObject value) {
    ancestor(distance)->values.insert_or_assign(name.lexeme, value);
}


LoxObject Environment::get(Token name) {
    auto var = values.find(name.lexeme);
    if (var != values.end()) {
        return var->second;
    }

    if (enclosing) return enclosing->get(name);

    throw std::runtime_error("Undefined variable '" 
                            + name.lexeme + 
                            "' [line " + std::to_string(name.line) + "]"); // make custom runtime error
}

void Environment::assign(Token name, LoxObject value) {
    auto var = values.find(name.lexeme);
    if (var != values.end()) {
        values[name.lexeme] = value;
        return;
    }

    if (enclosing) {
        enclosing->assign(name, value);
        return;
    }

    throw std::runtime_error("Undefined variable '" 
                            + name.lexeme + 
                            "' [line " + std::to_string(name.line) + "]"); // make custom runtime error

}

ScopeEnvironment::ScopeEnvironment(PEnvironment& encl, PEnvironment newEnv) 
    : backup_env{encl}, enclosing{encl} {
        encl = newEnv;
    }
ScopeEnvironment::~ScopeEnvironment() {
    enclosing = backup_env;
}

} // namespace lox