#include "loxObject.hpp"
#include "lox.hpp"
#include <sstream>

namespace lox {

LoxObject::LoxObject(const LoxObject& o){
    string = o.string;
    number = o.number;
    lox_type = o.lox_type;
    boolean = o.boolean;
}

LoxObject& LoxObject::operator=(const LoxObject& o){
    string = o.string;
    number = o.number;
    lox_type = o.lox_type;
    boolean = o.boolean;
    return *this;
}

LoxObject::~LoxObject() {} // nothing for now.

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
        // handle other types later
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
        // handle other types later

    }
    throw std::runtime_error("Could not convert object to number");
}

LoxObject::operator bool() const {
    switch(lox_type) {
        case LoxType::Nil: return false;
        case LoxType::Bool: return boolean;
        case LoxType::Number: return number != 0.;
        case LoxType::String: return string != "";
        // handle other types later
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
                number -= number;
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

} // namespace lox