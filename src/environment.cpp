#include <string>
#include "environment.hpp"

namespace lox {

Environment::Environment() { enclosing = nullptr; }
Environment::Environment(Environment* environment) { enclosing = environment; }
Environment::~Environment () { delete enclosing; }

void Environment::define(std::string s, LoxObject value) {
    values.insert_or_assign(s, value);
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

} // namespace lox