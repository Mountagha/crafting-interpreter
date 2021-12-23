#include <iostream>
#include <array>
#include <tuple>

void print() {} // To end the recursion when the parameter pack is empty

template<typename T, typename... Types>
void print(T first, Types... others) {
    std::cout << first << '\n';
    print(others...);
}

template<typename T>
class AddSpace {
    private:
        T const& ref;  // refer to the argument pased in constructor
    public:
        AddSpace(T const& r): ref(r) {}
        friend std::ostream& operator<<(std::ostream& os, AddSpace<T> s){
            return os << s.ref << ' '; // ouptput passed argument and a space
        }
};

template<typename... Types>
void better_print(Types const&... args) {
    (std::cout << ... << AddSpace(args)) << '\n'; // We using AddSpace class above to add a space
}


// type for arbitrary number of indices
template<std::size_t...>
struct Indices {

};
// this can be used to define afunction that calls prints for the elements of a std::array or tuple
template<typename T, std::size_t... Idx>
void printByIdx(T t, Indices<Idx...>) {
    print(std::get<Idx>(t)...);
}

int main() {
    std::string s("abc");
    print(8, 2.9, s, "other const char");
    better_print(8, 2.9, s, "other const char");

    std::array<std::string, 5> arr = {"hello", "my", "new", "!", "World"};
    printByIdx(arr, Indices<0, 4, 3>());

    auto t = std::make_tuple(12, "monkeys", 2.0);
    printByIdx(t, Indices<0, 1, 2>());
}
