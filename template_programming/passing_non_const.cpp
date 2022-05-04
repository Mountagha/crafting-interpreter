#include <iostream>

template<typename T>
void outR(T& arg) {
    static_assert(!std::is_const_v<T>, "out parameter of foo<T>(T&) is const");
    std::cout << arg;
}

template<typename T, typename = std::enable_if_t<!std::is_const_v<T>>>
void outR2(T& arg){
    std::cout << arg;
}

// Since c++20
/*
template<typename T>
requires !std::is_const_v<T>
void outR3(T& arg) {
    std::cout << arg;
}
*/

int main(){
    std::string const s = "hi";
    //outR(s);
    outR2(s);
    return 0;
    // aim to fail at compilation
}