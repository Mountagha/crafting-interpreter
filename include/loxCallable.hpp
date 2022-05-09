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
        LoxFunction(Function* declaration, Interpreter* inpt, PEnvironment encl, bool isInit = false);
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
    
    private:
        bool isInitializer;
        Function* declaration;
        Interpreter* interpreter;
        PEnvironment enclosing;
};

class LoxInstance;

class LoxClass : public LoxCallable {
    public:
        LoxClass(Class* stmt, LoxClass* superClass, Interpreter* intp, PEnvironment encl);
        std::string name() const override { return "<class " + cname.lexeme + ">"; }
        LoxObject operator()(Interpreter& in, std::vector<LoxObject> args) override ;
        LoxObject function(Token name, LoxInstance* instance);
        size_t arity() const override;
    private:
        Interpreter* interpreter;
        LoxClass* super;
        Token cname;
        std::map<std::string, LoxObject> methods {};
        friend class LoxInstance;

};

class LoxInstance {
    public:
        LoxInstance(LoxClass* klass_): klass{klass_} { cname = klass->cname; }
        std::string name() const { return "<instance " + cname.lexeme + ">"; }
        LoxObject get(Token name);
        LoxObject set(Token name, LoxObject value);
    private:
        LoxClass* klass; 
        Token cname;
        std::map<std::string, LoxObject> fields {};
};

} // lox namespace