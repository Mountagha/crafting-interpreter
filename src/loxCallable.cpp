#include "loxCallable.hpp"
#include "interpreter.hpp"
#include "environment.hpp"
#include "return.hpp"


namespace lox {

LoxFunction::LoxFunction(Function* decl, std::shared_ptr<Environment> encl) {
    declaration = decl;
    enclosing = encl;

}

LoxFunction::~LoxFunction() { /* handle later */ }

LoxObject LoxFunction::operator()(Interpreter& intp, std::vector<LoxObject> args) {
    auto environment = std::make_shared<Environment>(enclosing);
    for (int i = 0; i < declaration->params.size(); i++) {
        environment->define(declaration->params[i].lexeme, args[i]);
    }
    try {
        intp.executeBlock(declaration->body, environment);
    } catch (ReturnExcept& returnValue) {
        return returnValue.get();
    }

    return LoxObject();
}

LoxObject LoxClass::operator()(Interpreter& intp, std::vector<LoxObject> args) {
    // to implement
    return LoxObject();    
}

LoxObject LoxInstance::get(Token name) {
    auto value = fields.find(name.lexeme);
    if (value != fields.end()) {
        return value->second;
    }
    // maybe create later a custom runtimeError in order to print
    // the line and/or the file along with the error message.
    throw std::runtime_error("Undefined property '" + name.lexeme + "'.");
}

LoxObject LoxInstance::set(Token name, LoxObject value) {
    return fields[name.lexeme] = value;
}


} // namespace lox