#pragma once

#include <type_traits>
#include <vector>
#include <deque>

template<typename T, typename Cont=std::deque<T>>
class Stack {
    private:
        Cont container;
    public:
        void pop();
        void push(T const& element);
        T const& top() const;
        bool empty() const {
            return container.empty();
        }
        // assign stack of elements of type T2
        template<typename T2, typename Cont2>
        Stack& operator= (Stack<T2, Cont2> const&); // to illustrate assignmnent with # types.
        // to get access to private members of Stack<T2> for any type T2.
        template<typename, typename> friend class Stack;
};

template<typename T, typename Cont>
void Stack<T, Cont>::push(T const& element) {
    container.push_back(element);
}

template<typename T, typename Cont>
void Stack<T, Cont>::pop() {
    assert(!container.empty());
    container.pop_back();
}

template<typename T, typename Cont>
T const& Stack<T, Cont>::top() const {
    assert(!container.empty());
    return container.back();
}

template<typename T, typename Cont>
template<typename T2, typename Cont2>
Stack<T, Cont>& Stack<T, Cont>::operator=(Stack<T2, Cont2> const& op2) {
    Stack<T2> tmp(op2);  // Create a copy of the assigned stack

    container.clear();  // removing existing elements
    /* while (!tmp.empty()) {
        container.push_front(tmp.top());
        tmp.pop();
    } */ // using friend in the declaration allows us to access private elements of op2
    container.insert(container.begin(), 
                    op2.container.begin(),
                    op2.container.end());
    return *this;
    // Note that this won't work in our case 
}


