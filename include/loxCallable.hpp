#pragma once 

#include <vector>
#include "loxObject.hpp"
#include "interpreter.hpp"


namespace lox {

using Arguments = std::vector<LoxObject>;

class LoxCallable {
    public:
        virtual ~LoxCallable() {}
        virtual LoxObject operator()(Interpreter& interpreter, Arguments arguments) = 0;
        virtual size_t arity() const = 0;
        virtual std::string name() const = 0;
};
}