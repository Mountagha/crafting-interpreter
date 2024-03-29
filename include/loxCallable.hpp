#pragma once 

#include <vector>
#include <chrono>
#include <map>
#include "loxObject.hpp"
#include "Stmt.hpp"
#include "environment.hpp"


namespace lox {

using Arguments = std::vector<LoxObject>;

class LoxCallable {
    public:
        virtual ~LoxCallable() {}
        virtual LoxObject operator()(Interpreter& interpreter, std::vector<LoxObject> args) = 0;
        virtual size_t arity() const = 0;
        virtual std::string name() const = 0;
};

class TimeFunction : public LoxCallable {
    public:
        using Clock = std::chrono::high_resolution_clock;
        TimeFunction() {
            begin = std::chrono::high_resolution_clock::now();
        } 
        ~TimeFunction() {}
        size_t arity() const override { return 0; }
        LoxObject operator()(Interpreter&, std::vector<LoxObject>) override {
            double t = std::chrono::duration<double>(Clock::now() - begin).count();
            return LoxObject(t); 
        }
        std::string name() const { return "clock"; }
    private:
        decltype(Clock::now()) begin; 
};

class LoxFunction : public LoxCallable {
    public:
        LoxFunction(Function* declaration, Interpreter* intp, PEnvironment encl, bool isInit = false);
        LoxFunction(LoxFunction& other, PEnvironment encl);
        ~LoxFunction();
        size_t arity() const override { return declaration->params.size(); }
        std::string name() const override { return "<fun " + declaration->name.lexeme + ">"; }
        LoxObject operator()(Interpreter& in, std::vector<LoxObject> args) override ;

        // Needed getters & setters
        PEnvironment getEnclosing() {
            return enclosing;
        }
        Function* getDeclaration() {
            return declaration;
        }

        bool isGetter() const {
            return getter;
        }
    
    private:
        bool isInitializer;
        bool getter;
        Function* declaration;
        Interpreter* interpreter;
        PEnvironment enclosing;
};
class LoxClass;

class LoxInstance {
    public:
        LoxInstance() = default;
        LoxInstance(LoxClass* klass_); 
        std::string name() const { return "<instance " + cname.lexeme + ">"; }
        LoxObject get(Token name);
        LoxObject set(Token name, LoxObject value);
        LoxClass* getClass() const { return klass; };
        virtual ~LoxInstance() = default;   // so that I can use dynamic_cast.
    private:
        LoxClass* klass; 
        Token cname;
        std::map<std::string, LoxObject> fields {};
};

class LoxClass : public LoxCallable, public LoxInstance {
    public:
        LoxClass(Class* stmt, LoxClass* superClass, Interpreter* intp, PEnvironment encl);
        std::string name() const override { return "<class " + cname.lexeme + ">"; }
        LoxObject operator()(Interpreter& in, std::vector<LoxObject> args) override ;
        LoxObject function(Token name, LoxInstance* instance);
        LoxObject get(Token name);
        LoxObject set(Token name, LoxObject value);
        size_t arity() const override;
    private:
        Interpreter* interpreter;
        LoxClass* super;
        Token cname;
        std::map<std::string, LoxObject> methods {};
        std::map<std::string, LoxObject> class_fields {};
        friend class LoxInstance;

};

} // lox namespace
