#pragma once 

#include <string>
#include "token.hpp"

namespace lox {

enum class LoxType {
    Nil=0, 
    Bool,
    Number,
    String,
};

inline bool operator<(LoxType a, LoxType b) {
    return static_cast<int>(a) < static_cast<int>(b);
}

class LoxObject {
    public:
        LoxObject() : lox_type(LoxType::Nil) {}
        explicit LoxObject(bool b): lox_type(LoxType::Bool), boolean{b} {}
        explicit LoxObject(double d): lox_type(LoxType::Number), number{d} {}
        explicit LoxObject(std::string s): lox_type(LoxType::String), string{s} {}
        // add Callable later

        explicit LoxObject(Token token);
        LoxObject(const LoxObject&);
        LoxObject& operator=(const LoxObject& );
        ~LoxObject();


        friend bool operator==(const LoxObject& a, const LoxObject& b);
        friend bool operator<(const LoxObject& a, const LoxObject& b);
        friend LoxObject operator-(LoxObject a);
        friend LoxObject operator!(LoxObject a);

        LoxObject& operator+=(const LoxObject& o);
        LoxObject& operator-=(const LoxObject& o);
        LoxObject& operator*=(const LoxObject& o);
        LoxObject& operator/=(const LoxObject& o);

        LoxObject get(Token name);
        LoxObject set(Token name, LoxObject value);
        
        operator std::string() const;
        operator double() const;
        operator bool() const;


    private:
        LoxType lox_type = LoxType::Nil;
        double number = 0.;
        bool boolean = false;
        std::string string = "";

        void cast(LoxType t) {
            if (t == lox_type) return;
            // handle other types later
            switch(t) {
                case LoxType::Nil: break;
                case LoxType::Bool: boolean = (bool)(*this); break;
                case LoxType::Number: number = (double)(*this); break;
                case LoxType::String: string = (std::string)(*this); break;
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