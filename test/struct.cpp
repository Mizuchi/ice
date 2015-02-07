#include <ice/ice.hpp>
#include <cassert>

struct A {
    constexpr A(int i) : i(i) {}
    constexpr int get() { return i; }
    int i;
};

template <class T> constexpr int g(Const<T> t) {
    static_assert(A(t).get() == 1);
    return 1;
}

int g(A) { return 2; }

int main() {
    constexpr A a1{1};
    A a2{2};
    static_assert(g(FROZEN(a1)) == 1);
    assert(g(FROZEN(a2)) == 2);
}
