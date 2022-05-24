#include <iostream>
#include <vector>

// number of elements in a raw array.
template<typename T, unsigned N>
std::size_t len(T(&)[N]) {
    return N;
}

// number of elements for a type having size type:
template<typename T>
typename T::size_type len(T const& t) {
    return t.size();
}

template<typename T>
auto len(T const& t) -> decltype( (void)(t.size()), T::size_type() ) {
    return t.size();
}

int main() {
    int a[10];
    std::cout << len(a);        // only len() for array matches
    std::cout << len("tmp");  // only len for array matches
    std::vector<int> v;
    std::cout << len(v);      // only len() for a type with size_type matches.
    int *p;
    //std::cout << len(p); // ERROR: no matching function is found.
    return 0;
}

