#pragma once 

#include <string>
#include <ostream>
#include <vector>
#include "token.hpp"

namespace lox {

enum class LoxType {
    Nil=0, 
    Bool,
    Number,
    String,
    Callable,
    Class,
    Instance
};

inline bool operator<(LoxType a, LoxType b) {
    return static_cast<int>(a) < static_cast<int>(b);
}

class Interpreter;
class LoxCallable;
class LoxClass;
class LoxInstance;

class LoxObject {
    public:
        LoxObject() : lox_type(LoxType::Nil) {}
        explicit LoxObject(bool b): lox_type(LoxType::Bool), boolean{b} {}
        explicit LoxObject(double d): lox_type(LoxType::Number), number{d} {}
        explicit LoxObject(std::string s): lox_type(LoxType::String), string{s} {}
        explicit LoxObject( LoxCallable* callable, Interpreter* in);
        explicit LoxObject( LoxClass* lk, Interpreter* in);
        explicit LoxObject( LoxInstance* li, Interpreter* in);

        explicit LoxObject(Token token);
        LoxObject(const LoxObject&);
        LoxObject& operator=(const LoxObject& );
        ~LoxObject();

        // Get, Set
        LoxObject get(Token name);
        LoxObject set(Token name, LoxObject value);

        LoxObject operator()(Interpreter& in, std::vector<LoxObject> args);


        friend bool operator==(const LoxObject& a, const LoxObject& b);
        friend bool operator<(const LoxObject& a, const LoxObject& b);
        friend std::ostream& operator<<(std::ostream& os, const LoxObject& o);
        friend LoxObject operator-(LoxObject a);
        friend LoxObject operator!(LoxObject a);

        LoxObject& operator+=(const LoxObject& o);
        LoxObject& operator-=(const LoxObject& o);
        LoxObject& operator*=(const LoxObject& o);
        LoxObject& operator/=(const LoxObject& o);

        operator std::string() const;
        operator double() const;
        operator bool() const;


    private:
        LoxType lox_type = LoxType::Nil;
        double number = 0.;
        bool boolean = false;
        std::string string = "";
        LoxCallable* function = nullptr;
        Interpreter* interpreter = nullptr;
        LoxClass* loxklass = nullptr;
        LoxInstance* instance = nullptr;


        void cast(LoxType t) {
            if (t == lox_type) return;
            if (lox_type == LoxType::Callable) {
                throw std::runtime_error("Cannot convert callable to non-callable");
            }
            if (t == LoxType::Class || t == LoxType::Instance) {
                throw std::runtime_error("Cannot convert class to non-class");
            }
            switch(t) {
                case LoxType::Nil: break;
                case LoxType::Bool: boolean = (bool)(*this); break;
                case LoxType::Number: number = (double)(*this); break;
                case LoxType::String: string = (std::string)(*this); break;
                case LoxType::Callable:
                    throw std::runtime_error("Cannot convert non-callable to callable");
                // handle other types later
            }

            lox_type = t;
        }

};


inline bool operator!=(const LoxObject& a, const LoxObject& b){
    return !(a == b);
}

inline bool operator<=(const LoxObject& a, const LoxObject& b){
    return a < b || a == b;
}

inline bool operator>(const LoxObject& a, const LoxObject& b){
    return !(a <= b);
}

inline bool operator>=(const LoxObject& a, const LoxObject& b) {
    return!(a < b);
}

inline LoxObject operator+(LoxObject a, const LoxObject& b) {
    return a += b;
}

inline LoxObject operator-(LoxObject a, const LoxObject& b) {
    return a -= b;
}

inline LoxObject operator*(LoxObject a, const LoxObject& b) {
    return a *= b;
}

inline LoxObject operator/(LoxObject a, const LoxObject& b) {
    return a /= b;
}

} // end of lox namespace