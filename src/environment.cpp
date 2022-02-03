#include "environment.hpp"

namespace lox {

void Environment::define(std::string s, loxObject value) {
    values.insert_or_assign(s, value);
}

loxObject get(Token name) {
    auto var = values.find(name.lexeme);
    if (var != values.end()) {
        return var->second;
    }
    throw std::runtime_error() // make custom runtime error
}
}