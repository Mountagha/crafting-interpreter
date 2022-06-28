#include <vector>
#include <iostream>
#include <string>
#include <utility>
#include <functional>

template<typename Iter, typename Callable, typename... Args>
void foreach (Iter current, Iter end, Callable op, Args const&... args) {
    while (current != end) {
        std::invoke(op, 
                    args...,
                    *current);
        current++;
    }
}

// a class with a member function that shall be called
class MyClass {
    public:
        void memfunc(int i) const {
            std::cout << "MyClass::memfunc() called for: " << i << '\n';
        }
};

int main() {
    std::vector<int> primes = {2, 3, 5, 7, 11, 13, 17, 19};

    // passs lambda as callable and an additional argument:
    foreach(primes.begin(), primes.end(), 
            [](std::string const &prefix, int i) {
                std::cout << prefix << i << '\n';
            }, " - value: ");
    
    MyClass obj;
    foreach(primes.begin(), primes.end(), &MyClass::memfunc, obj);
}

