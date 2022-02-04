#include <string>
#include "environment.hpp"

namespace lox {

void Environment::define(std::string s, LoxObject value) {
    values.insert_or_assign(s, value);
}

LoxObject Environment::get(Token name) {
    auto var = values.find(name.lexeme);
    if (var != values.end()) {
        return var->second;
    }
    throw std::runtime_error("Undefined variable '" 
                            + name.lexeme + 
                            "' [line " + std::to_string(name.line) + "]"); // make custom runtime error
}

} // namespace lox