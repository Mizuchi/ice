#include <ice/ice.hpp>
#include <cassert>

struct A {
    constexpr A(int i) : i(i) {}
    constexpr int get() const { return i; }
    int i;
};

template <class T> auto g(Const<T> t) {
    static_assert(t.get().get() == 1, "");
    return FROZEN(1);
}

int g(A) { return 2; }

int main() {
    constexpr A a1{1};
    A a2{2};
    auto x = g(FROZEN(a1));
    static_assert(x == 1, "");
    assert(g(FROZEN(a2)) == 2);
}
