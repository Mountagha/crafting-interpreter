#include <utility>      // for std::invoke
#include <functional>   // for std::forward
#include <chrono>
#include <iostream>
#include <thread>
#include <type_traits>


template<typename Callable, typename... Args>
decltype(auto) timer(Callable&& op, Args&&... args) {
    if constexpr(!std::is_same_v<std::invoke_result_t<Callable, Args...>, void>) {
        auto t1 = std::chrono::high_resolution_clock::now();
        decltype(auto) ret{std::invoke(std::forward<Callable>(op),
                        std::forward<Args>(args)...)};
        auto t2 = std::chrono::high_resolution_clock::now();
        std::cout << "Ran for " 
                << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t2).count() 
                << '\n';
        return ret;
    } else {
        auto t1 = std::chrono::high_resolution_clock::now();
        std::invoke(std::forward<Callable>(op),
                        std::forward<Args>(args)...);
        auto t2 = std::chrono::high_resolution_clock::now();
        std::cout << "Ran for " 
                << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t2).count() 
                << '\n';
        return; 
    }
}

void f1(int a) {
    std::this_thread::sleep_for(std::chrono::microseconds(a));
}

int f2(int b) {
    std::this_thread::sleep_for(std::chrono::microseconds(b));
    return b;
}

int main() {
    // test for void call
    timer(f1, 10000);
    // test for non void call
    auto ret = timer(f2, 20000);
}


