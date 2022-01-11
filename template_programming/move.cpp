#include <iostream>
#include <utility>

class X{};

void g (X&) {
    std::cout << "g() for variable\n";
}

void g (X const&) {
    std::cout << "g() for constants\n";
}

void g (X&&) {
    std::cout << "g() for movable objects\n";
}

// let f() perfect forward argument val to g():
template<typename T>
void f(T&& val) {
    g(std::forward<T>(val)); 
}

int main() {
    X v; 
    X const c;

    f(v);
    f(c);
    f(X());
    f(std::move(v));

    return 0;
}