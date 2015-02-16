#include <ice/ice.hpp>
#include <cassert>
#include <typeinfo>

struct A {
    constexpr A(int i) : i(i) {}
    constexpr int value() const { return i; }
    int i;
};

template <class T> constexpr int g(folly::ice::Const<T> t) {
    static_assert(std::is_same<decltype(t.get()), const A>::value, "");
    static_assert(t.get().value() == 1, "");
    return t.get().value() + 1;
}

int g(A a) { return a.i * 2; }

int main() {
    constexpr A a1{1};

    auto arg = ICE(a1);
    static_assert(g(arg) == 2, "");

    auto arg2 = ICE(arg);
    static_assert(g(arg2) == 2, "");

    auto &ref = arg;
    auto arg3 = ICE(ref);
    static_assert(g(arg3) == 2, "");

    const auto &cref = arg;
    auto arg4 = ICE(cref);
    static_assert(g(arg4) == 2, "");

    A a2{10};
    assert(g(ICE(a2)) == 20);
}
