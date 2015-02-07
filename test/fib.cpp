#include <ice/ice.hpp>
#include <vector>
#include <cassert>
#include <iostream>

constexpr int fibImpl(int t) {
    return t <= 1 ? t : fibImpl(t - 1) + fibImpl(t - 2);
}

template <class T> constexpr int fib(Const<T> t) {
    static_assert(t >= 0);
    return fibImpl(t);
}

// template <class T> constexpr int fib(Const<T> t) {
//     return t <= 1 ? t : fib(FROZEN(t - 1)) + fib(FROZEN(t - 2));
// }
// template <long t> constexpr long fib() {
//     return t <= 1 ? t : fib<t - 1>() + fib<t - 2>();
// }

int fib(int t) {
    static std::vector<int> results = {0, 1};
    if (results.size() <= t) {
        int ret = fib(t - 1) + fib(t - 2);
        results.push_back(ret);
    }
    return results[t];
}

constexpr int four() { return 4; }

int main() {
    static_assert(fib(FROZEN(four())) == 3, ""); // constexpr version
    static_assert(fib(FROZEN(5)) == 5, "");      // constexpr version
    constexpr int two = 2;
    static_assert(fib(FROZEN(two + four())) == 8, ""); // constexpr version
    // fib(FROZEN(two - four())); // compile time error
    int j = 7;
    assert(fib(FROZEN(j)) == 13); // non-constexpr version
}
