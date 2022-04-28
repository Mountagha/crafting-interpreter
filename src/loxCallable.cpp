#include "loxCallable.hpp"
#include "interpreter.hpp"
#include "environment.hpp"
#include "return.hpp"


namespace lox {

LoxFunction::LoxFunction(Function* decl, std::shared_ptr<Environment> encl, bool isInit) {
    declaration = decl;
    enclosing = encl;
    isInitializer = isInit;
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
        if (isInitializer) return enclosing->getAt(0, "this");
        return returnValue.get();
    }

    return LoxObject();
}

LoxClass::LoxClass(Class* stmt, LoxClass* superClass, Interpreter* intp, PEnvironment encl) {
    cname = stmt->name;
    super = superClass;
    interpreter = intp;
    bool isInit {false};

    for (auto& m: stmt->methods) {
        isInit = m->name.lexeme == "init" ? true : false;
        LoxCallable* method {static_cast<LoxCallable*>(new LoxFunction(m.get(), encl, isInit))}; // possible leak with get maybe 
        methods[m->name.lexeme] = LoxObject(method, intp);
    }
}

LoxObject LoxClass::function(Token name, LoxInstance* instance) {
    // possible leak in this function. Check later.
    auto var = methods.find(name.lexeme);
    if (var != methods.end()) {
        LoxFunction* func = static_cast<LoxFunction*>(var->second.getFunction()); // possible leak ?
        PEnvironment environment = std::make_shared<Environment>(func->getEnclosing());
        environment->define("this", LoxObject(instance, interpreter));
        LoxCallable* new_method {static_cast<LoxCallable*>(new LoxFunction(func->getDeclaration(), environment))}; 
        return LoxObject(new_method, interpreter);
    }
    if (super) return super->function(name, instance); 
    throw std::runtime_error("Undefined property '" + name.lexeme + "'.");
    // maybe create later a custom runtimeError in order to print
    // the line and/or the file along with the error message.
}

LoxObject LoxClass::operator()(Interpreter& intp, std::vector<LoxObject> args) {

    if (&intp != interpreter) {
        std::runtime_error("class constructed in different interpreter.");
    }

    LoxInstance* instance = new LoxInstance(this);  // possible leak here. not freed
    auto instance_object = LoxObject(instance, &intp);
    if (methods.find("init") != methods.end()) {
        // we are giving the line of the token 0 cause we don't care. We just want to retrieve the init method using its name. 
        instance_object.getInstance()->get({IDENTIFIER, "init", 0})(intp, args);
    }
    return instance_object;  
}

size_t LoxClass::arity() const {
    auto init_method = methods.find("init");
    if (init_method != methods.end()) {
        return init_method->second.getFunction()->arity();
    }
    return 0;
}

LoxObject LoxInstance::get(Token name) {
    auto value = fields.find(name.lexeme);
    if (value != fields.end()) {
        return value->second;
    }
    return klass->function(name, this);
}

LoxObject LoxInstance::set(Token name, LoxObject value) {
    return fields[name.lexeme] = value;
}


} // namespace lox