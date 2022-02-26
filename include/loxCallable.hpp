#pragma once 

#include <vector>
#include <chrono>
#include "loxObject.hpp"
#include "interpreter.hpp"


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

};

}