#include "loxObject.hpp"
#include "loxCallable.hpp"
#include "interpreter.hpp"
#include "lox.hpp"
#include <sstream>

namespace lox {

LoxObject::LoxObject(LoxCallable* callable, Interpreter* in) 
    : function{callable}, interpreter{in}, lox_type{LoxType::Callable} {}

LoxObject::LoxObject(LoxInstance* li, Interpreter* in) 
    : instance{li}, interpreter{in}, lox_type{LoxType::Instance} {}

LoxObject::LoxObject(LoxClass* lk, Interpreter* in)
    : loxklass{lk}, interpreter{in}, lox_type{LoxType::Class} {}

LoxObject::LoxObject(const LoxObject& o){
    string = o.string;
    number = o.number;
    lox_type = o.lox_type;
    boolean = o.boolean;
    function = o.function;
    instance = o.instance;
    loxklass = o.loxklass;
    interpreter = o.interpreter;
    if (interpreter) saveCallableCopy();
}

LoxObject& LoxObject::operator=(const LoxObject& o){
    string = o.string;
    number = o.number;
    lox_type = o.lox_type;
    boolean = o.boolean;
    function = o.function;
    instance = o.instance;
    loxklass = o.loxklass;
    interpreter = o.interpreter;
    if (interpreter) saveCallableCopy();
    return *this;
}

void LoxObject::saveCallableCopy() {
    switch (lox_type) {
        case LoxType::Callable:
            interpreter->addUser(function);
            break;
        case LoxType::Class:
            interpreter->addUser(loxklass);
            break;
        case LoxType::Instance:
            interpreter->addUser(instance);
            break;
        default:
            throw std::logic_error("Expected object to be callable or class.");
    }
}

LoxObject::~LoxObject() {
    if (interpreter) {
        switch (lox_type) {
            case LoxType::Callable:
                interpreter->removeUser(function);
                break;
            case LoxType::Class:
                interpreter->removeUser(loxklass);
                break;
            case LoxType::Instance:
                interpreter->removeUser(instance);
                break;
            default:
                throw std::logic_error("Lox Object has bad type when destroyed.");
        }
    }
} 

LoxObject LoxObject::get(Token name) {
    if (lox_type != LoxType::Instance) {
        throw std::runtime_error("Cannot get property from a non-class instance");
    }
    return instance->get(name);
}

LoxObject LoxObject::set(Token name, LoxObject value) {
    if (lox_type != LoxType::Instance) {
        throw std::runtime_error("Cannot set property on non-class instance.");
    }
    return instance->set(name, value);
}

LoxObject LoxObject::operator()(Interpreter& in, std::vector<LoxObject> args) {
    if (lox_type != LoxType::Callable) {
        std::runtime_error("Cannot call non-callable");
    }
    if (lox_type == LoxType::Class) {
        if (args.size() != loxklass->arity()) {
            std::string msg = "Function argument count mismatch. Expected " 
                + std::to_string(loxklass->arity()) + ", got " 
                + std::to_string(args.size()) + "\n";
            throw std::runtime_error(msg);
        }
        return (*loxklass)(in, args);
    }
    if (args.size() != function->arity()){
        std::string msg = "Function argument count mismatch. Expected "
            + std::to_string(function->arity()) + ", got" 
            + std::to_string(args.size()) + "\n";
        throw std::runtime_error(msg);
    }
    return (*function)(in, args);
    // return LoxObject();
}

LoxObject::LoxObject(Token token) {
    switch(token.token_type) {
        case TokenType::NIL:
            lox_type = LoxType::Nil;
            break;
        case TokenType::TRUE:
            lox_type = LoxType::Bool;
            boolean = true;
            break; 
        case TokenType::FALSE:
            lox_type = LoxType::Bool;
            boolean = false;
            break;
        case TokenType::NUMBER:
            lox_type = LoxType::Number;
            // limit scope of stringstream.
            {
                std::stringstream ss(token.lexeme);
                ss >> number;
            }
            break;
        case TokenType::STRING:
            lox_type = LoxType::String;
            string = token.lexeme;
            break;
        default:
            throw std::runtime_error("Invalid Lox Object"); 
    }
}

LoxObject::operator std::string() const {
    switch(lox_type) {
        case LoxType::Nil: return "nil";
        case LoxType::Bool: return boolean ? "true" : "false";
        case LoxType::Number: 
        {
            std::stringstream ss;
            ss.precision(15);
            ss << number;
            return ss.str();
        }

        case LoxType::String: return string;
        case LoxType::Callable:
            return function->name();
        case LoxType::Class:
            return loxklass->name();
        case LoxType::Instance:
            return instance->name();
    }
    throw std::runtime_error("Could not convert object to string");
}

LoxObject::operator double() const {

    switch(lox_type){
        case LoxType::Nil: return 0.;
        case LoxType::Bool: return boolean ? 1. : 0.;
        case LoxType::Number: return number;
        case LoxType::String: 
        {
            std::stringstream ss(string);
            double num;
            ss >> num;
            if (ss.fail() || ss.bad()) throw std::runtime_error("Bad cast.");
            return num;
        }
        case LoxType::Callable:
        case LoxType::Class:
        case LoxType::Instance:
            break;
    }
    throw std::runtime_error("Could not convert object to number");
}

LoxObject::operator bool() const {
    switch(lox_type) {
        case LoxType::Nil: return false;
        case LoxType::Bool: return boolean;
        case LoxType::Number: return number != 0.;
        case LoxType::String: return string != "";
        case LoxType::Callable:
        case LoxType::Class:
        case LoxType::Instance:
            return true;
    }

    throw std::runtime_error("Could not convert object to bool.");
}

bool operator==(const LoxObject& a, const LoxObject& b) {
    if (a.lox_type == b.lox_type){
        switch(a.lox_type){
            case LoxType::Nil:
                return true;
            case LoxType::Bool:
                return a.boolean == b.boolean;
            case LoxType::Number:
                return a.number == b.number;
            case LoxType::String:
                return a.string == b.string;
            default:
                throw std::runtime_error("Cannot compare object for equalities.");
        } 
    }
    if (a.lox_type > b.lox_type) return b == a; // don't get this YET.
    if (a.lox_type == LoxType::Nil || b.lox_type == LoxType::Nil) return false;
    switch(a.lox_type) {
        case LoxType::Bool: return a.boolean == (bool)b;
        case LoxType::Number: return a.number == (double)b;
        case LoxType::String: return a.string == (std::string)b;
        default:
            throw std::runtime_error("Cannot compare objects for equality.");
    }
    return false;
}

bool operator<(const LoxObject& a, const LoxObject& b) {
    if (a.lox_type == b.lox_type) {
        switch (a.lox_type) {
            case LoxType::Nil:
            case LoxType::Bool:
                throw std::runtime_error("Nil and bool are not well ordered.");
            case LoxType::Number:
                return a.number < b.number;
            case LoxType::String:
                return a.string < b.string;
            default:
                throw std::runtime_error("Object are not well ordered.");
        }
    }
    throw std::runtime_error("Only object of the same type can be well ordered.");
}

LoxObject& LoxObject::operator+=(const LoxObject& o) {
    if (lox_type == o.lox_type) {
        switch(lox_type) {
            case LoxType::Nil:
                throw std::runtime_error("Cannot add nil.");
            case LoxType::Bool:
                throw std::runtime_error("Cannot add bools.");
            case LoxType::Number:
                number += o.number;
                break;
            case LoxType::String:
                string += o.string;
                break;
            default:
                throw std::runtime_error("Cannot add objects.");
        }
        return *this;
    }

    if (lox_type < o.lox_type) {
        cast(o.lox_type);
        return (*this) += o;
    } else {
        auto b = o;
        b.cast(lox_type);
        return (*this) += b;
    }
}

LoxObject& LoxObject::operator-=(const LoxObject& o) {
    if (lox_type == o.lox_type) {
        switch(lox_type) {
            case LoxType::Nil: 
                throw std::runtime_error("Cannot subtract Nil.");
            case LoxType::Bool:
                throw std::runtime_error("Cannot subtract bools.");
            case LoxType::Number:
                number -= o.number;
                break;
            case LoxType::String:
                throw std::runtime_error("Cannot subtract strings.");
            default:
                throw std::runtime_error("Cannot subtract objects.");
        }
        return *this;
    }

    if (lox_type < o.lox_type) {
        cast(o.lox_type);
        return (*this) -= o;
    } else {
        auto b = o;
        b.cast(lox_type);
        return (*this) -= b;
    }
}

LoxObject& LoxObject::operator*=(const LoxObject& o) {
    if (lox_type == o.lox_type) {
        switch(lox_type) {
            case LoxType::Nil:
                throw std::runtime_error("Cannot multiply nil.");
            case LoxType::Bool:
                throw std::runtime_error("Cannot multiply bools.");
            case LoxType::Number:
                number *= o.number;
                break;
            case LoxType::String:
                throw std::runtime_error("Cannot multiply strings.");
                break;
            default:
                throw std::runtime_error("Cannot multiply objects.");
        }
        return *this;
    }

    if (lox_type < o.lox_type) {
        cast(o.lox_type);
        return (*this) *= o;
    } else {
        auto b = o;
        b.cast(lox_type);
        return (*this) *= b;
    }
}

LoxObject& LoxObject::operator/=(const LoxObject& o) {
    if (lox_type == o.lox_type) {
        switch(lox_type) {
            case LoxType::Nil:
                throw std::runtime_error("Cannot divide nil.");
            case LoxType::Bool:
                throw std::runtime_error("Cannot divide bools.");
            case LoxType::Number:
                if (o.number == 0.) 
                    throw std::runtime_error("Attempted a division by Zero\n");
                number /= o.number;
                break;
            case LoxType::String:
                throw std::runtime_error("Cannot divide strings.");
                break;
            default:
                throw std::runtime_error("Cannot multiply objects.");
        }
        return *this;
    }

    if (lox_type < o.lox_type) {
        cast(o.lox_type);
        return (*this) /= o;
    } else {
        auto b = o;
        b.cast(lox_type);
        return (*this) /= b;
    }
}
LoxObject operator-(LoxObject a) {
    switch(a.lox_type) {
        case LoxType::Number:
            a.number = -a.number;
            break;
        default:
            throw std::runtime_error("Unary - is not defined for current type.");
    }
    return a;
}

LoxObject operator!(LoxObject a) {
    switch(a.lox_type) {
        case LoxType::Nil:
            a.boolean = true;
            break;
        case LoxType::Number:
            a.boolean = a.number == 0.;
            break;
        case LoxType::String:
            a.boolean = a.string == "";
            break;
        case LoxType::Bool:
            a.boolean = !a.boolean;
            break;
        default:
            throw std::runtime_error("Cannot negate object.");
    }
    a.lox_type = LoxType::Bool;
    return a; 
}
std::ostream& operator<<(std::ostream& os, const LoxObject& o) {
    switch (o.lox_type) {
        case LoxType::Nil:
            os << "nil";
            break;
        case LoxType::Bool:
            os << (o.boolean ? "true" : "false");
            break;
        case LoxType::Number:
            os << o.number;
            break;
        case LoxType::String:
            os << o.string;
            break;
    } 
    return os;
}

} // namespace lox