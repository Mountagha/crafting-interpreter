#include "loxCallable.hpp"
#include "interpreter.hpp"
#include "environment.hpp"


namespace lox {

LoxFunction::LoxFunction(Function* decl, std::shared_ptr<Environment> encl) {
    declaration = decl;
    enclosing = encl;

}

LoxFunction::~LoxFunction() { /* handle later */ }

LoxObject LoxFunction::operator()(Interpreter& in, std::vector<LoxObject> args) {
    auto environment = std::make_shared<Environment>(enclosing);
    for (int i = 0; i < declaration->params.size(); i++) {
        environment->define(declaration->params[i].lexeme, args[i]);
    }
    in.executeBlock(declaration->body, environment);

    return LoxObject();
}
} // namespace lox