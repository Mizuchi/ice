#include <ice/ice.hpp>
#include <vector>
#include <cassert>
#include <iostream>

constexpr int fibImpl(int t) {
    return t <= 1 ? t : fibImpl(t - 1) + fibImpl(t - 2);
}

template <class T> auto fib(Const<T>) {
    static_assert(Const<T>{} >= 0, "");
    return FROZEN(fibImpl(Const<T>{}));
}

int fib(int t) {
    static std::vector<int> results = {0, 1};
    if (int(results.size()) <= t) {
        int ret = fib(t - 1) + fib(t - 2);
        results.push_back(ret);
    }
    return results[t];
}

constexpr int four() { return 4; }

int main() {
    auto x1 = fib(FROZEN(four()));
    static_assert(x1 == 3, ""); // constexpr version

    auto x2 = fib(FROZEN(5));
    static_assert(x2 == 5, ""); // constexpr version

    constexpr int two = 2;
    auto x3 = fib(FROZEN(two + four()));
    static_assert(x3 == 8, ""); // constexpr version

    // fib(FROZEN(two - four())); // compile time error
    int j = 7;
    assert(fib(FROZEN(j)) == 13); // non-constexpr version
}
