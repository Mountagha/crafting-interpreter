#include "stack_template.hpp"
#include <iostream>
#include <algorithm>
#include <vector>

template<int Val, typename T>
T addValue(T x) {
    return x + Val;
}

int main() {
    Stack<int, std::vector<int>> intStack;
    intStack.push(2);
    intStack.push(5);
    std::cout << intStack.top() << '\n';

    std::vector<int> v{1, 2, 3, 4, 5};
    std::vector<int> v2;
    v2.reserve(5);
    std::transform(v.begin(), v.end(), v2.begin(), addValue<5, int>);
    for (auto const& val : v2){
        std::cout << val << ", ";

    }

    Stack<int> intStack2;
    intStack2.push(3);
    intStack2.push(8);
    Stack<float> floatStack;
    floatStack = intStack2;
    std::cout << "float stack top: " << floatStack.top() 
            << " int stack top : " << intStack2.top() << '\n'; 
}