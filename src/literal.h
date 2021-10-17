#pragma once

namespace lox {
// to use later for literal type maybe
template<typename T>
class Literal {
    //std::optional<T> value;
    T value;
    Literal() = default;
    Literal(T value): value{value} {}
    ~Literal() = default;
    
};

} 