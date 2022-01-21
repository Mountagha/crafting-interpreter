#include <iostream>
#include <chrono>

template<unsigned p, unsigned d>    // p: number to check d: current divisor
struct DoIsPrime {
    static constexpr bool value = (p%d != 0) && DoIsPrime<p, d-1>::value;
};

template<unsigned p>    // end recursion if divisor is 2
struct DoIsPrime<p, 2> {
    static const bool value = (p%2 != 0);
};

template<unsigned p>    // primary template
struct IsPrime {
    // start recursion with divisor from p/2:
    static constexpr bool value = DoIsPrime<p, p/2>::value;
};

// special cases (to avoid endless recursion with template instanciation):
template<>
struct IsPrime<0> { static constexpr bool value = false; };
template<>
struct IsPrime<1> { static constexpr bool value = false; };
template<>
struct IsPrime<2> { static constexpr bool value = true; };
template<>
struct IsPrime<3> { static constexpr bool value = true; };

int main() {
    auto t1 = std::chrono::high_resolution_clock::now();
    std::string s = IsPrime<500>::value ? "is prime" : "is not prime";
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << s << '\n';
    std::cout << "time : " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " microseconds\n";
    return 0;
}





