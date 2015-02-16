#include <ice/ice.hpp>
#include <vector>
#include <cassert>
#include <iostream>

constexpr int fibImpl(int t) {
    return t <= 1 ? t : fibImpl(t - 1) + fibImpl(t - 2);
}

template <class T> constexpr int fib(folly::ice::Const<T> t) {
    static_assert(t >= 0, "");
    return fibImpl(t);
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
    {
        auto arg = ICE(four());
        static_assert(fib(arg) == 3, ""); // constexpr version
    }
    {
        auto arg = ICE(5);
        static_assert(fib(arg) == 5, ""); // constexpr version
    }
    {
        constexpr int two = 2;
        auto arg = ICE(two + four());
        static_assert(fib(arg) == 8, ""); // constexpr version
    }
    {
        int j = 7;
        auto arg = ICE(j);
        assert(fib(arg) == 13); // non-constexpr version
    }
    // fib(ICE(two - four())); // compile time error
}
