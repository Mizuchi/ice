#include <ice/ice.hpp>
#include <cassert>

struct A {
    constexpr A(int i) : i(i) {}
    constexpr int value() const { return i; }
    int i;
};

template <class T> auto g(ice::Const<T> t) {
    static_assert(std::is_same<decltype(t.get()), const A>::value, "");
    static_assert(t.get().value() == 1, "");
    return ICE(t.get().value() + 1);
}

int g(A) { return 5; }

int main() {
    constexpr A a1{1};
    A a2{10};
    auto x = g(ICE(a1));
    static_assert(x == 2, "");
    assert(g(ICE(a2)) == 5);
}
