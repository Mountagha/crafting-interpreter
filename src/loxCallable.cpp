#include "loxCallable.hpp"
#include "interpreter.hpp"
#include "environment.hpp"
#include "return.hpp"


namespace lox {

LoxFunction::LoxFunction(Function* decl, Interpreter* intp, std::shared_ptr<Environment> encl, bool isInit) {
    declaration = decl;
    enclosing = encl;
    isInitializer = isInit;
    interpreter = intp;
    interpreter->registerFunction(this, std::make_shared<Environment>(enclosing));
}

LoxFunction::LoxFunction(LoxFunction& other, std::shared_ptr<Environment> encl) {
    declaration = other.declaration;
    enclosing = encl;
    isInitializer = other.isInitializer;
    interpreter = other.interpreter;
    interpreter->registerFunction(this, enclosing);
}

LoxFunction::~LoxFunction() { interpreter->deleteFunction(this); }

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
        auto* method = interpreter->createFunction(m.get(), encl, isInit);
        methods[m->name.lexeme] = LoxObject(method, intp);
    }
}

LoxObject LoxClass::function(Token name, LoxInstance* instance) {
    // possible leak in this function. Check later.
    auto var = methods.find(name.lexeme);
    if (var != methods.end()) {
        LoxFunction* func = static_cast<LoxFunction*>(var->second.getFunction()); 
        PEnvironment environment = std::make_shared<Environment>(func->getEnclosing());
        if (auto obj = dynamic_cast<LoxClass *>(instance); obj == nullptr) { // if we got an instance instead of class.
            environment->define("this", LoxObject(instance, interpreter));
        }
        auto* new_method = interpreter->createFunction(func, environment); 
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

    LoxInstance* instance = interpreter->createInstance(this); 
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

LoxObject LoxClass::get(Token name) {
    auto value = class_fields.find(name.lexeme);
    if (value != class_fields.end()) {
        return value->second;
    }
    return function(name, this);
}

LoxObject LoxClass::set(Token name, LoxObject value) {
    return class_fields[name.lexeme] = value;
}


LoxInstance::LoxInstance(LoxClass* klass_): klass{klass_} { cname = klass->cname; }

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