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

LoxClass::LoxClass(Class* stmt, Interpreter* intp, PEnvironment encl) {
    cname = stmt->name;
    for (auto& m: stmt->methods) {
        LoxCallable* method {static_cast<LoxCallable*>(new LoxFunction(m.get(), encl))}; // possible leak with get maybe 
        methods[m->name.lexeme] = LoxObject(method, intp);
    }
}

LoxObject LoxClass::findMethod(std::string name) {
    auto var = methods.find(name);
    if (var != methods.end()) {
        return var->second;
    }
     throw std::runtime_error("Undefined property '" + name + "'.");
}

LoxObject LoxClass::operator()(Interpreter& intp, std::vector<LoxObject> args) {
    LoxInstance* instance = new LoxInstance(this);  // possible leak here. not freed
    return LoxObject(instance, &intp);
}

LoxObject LoxInstance::get(Token name) {
    auto value = fields.find(name.lexeme);
    if (value != fields.end()) {
        return value->second;
    }
    return klass->findMethod(name.lexeme);
    
    // maybe create later a custom runtimeError in order to print
    // the line and/or the file along with the error message.
   
}

LoxObject LoxInstance::set(Token name, LoxObject value) {
    return fields[name.lexeme] = value;
}


} // namespace lox