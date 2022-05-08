#include <iostream>
#include <chrono>

// since c11

constexpr bool 
doIsPrime (unsigned p, unsigned d) { // p: number to check, d: current divisor
    return d != 2 ? (p % d != 0) && doIsPrime(p, d-1) : (p % 2 != 0);
}

constexpr bool isPrimeC11 (unsigned p) {
    return p < 4 ? !(p < 2) : doIsPrime(p, p/2);
}

constexpr bool isPrimeC17 (unsigned int p) {
    for (unsigned int d=2; d<=p/2; ++d) {
        if (p % d == 0) return false;   // found divisor without remainder
    }
    return p > 1;
}

int main() {
    auto t1 = std::chrono::high_resolution_clock::now();
    std::string s = isPrimeC17(500) ? "is prime" : "is not prime";
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << s << '\n';
    std::cout << "time : " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " microseconds\n";
    return 0;
}





