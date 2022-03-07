#pragma once

#include <exception>
#include "loxObject.hpp"

namespace lox {

class ReturnExcept : public std::exception {
    public:
        ReturnExcept(LoxObject& val) : value{val} {}
        const char * what() const noexcept override { return "Lox return through exception "; }
        LoxObject get() const { return value; }
    private:
        LoxObject value;
};

} // namespace lox