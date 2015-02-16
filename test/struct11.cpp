#include <ice/ice.hpp>
#include <cassert>

struct A {
    constexpr A(int i) : i(i) {}
    constexpr int get() const { return i; }
    int i;
};

template <class T> constexpr int g(folly::ice::Const<T> t) {
    static_assert(t.get().get() == 1, "");
    return t.get().get() + 1;
}

int g(A) { return 5; }

int main() {
    constexpr A a1{1};
    A a2{10};
    auto arg = ICE(a1);
    static_assert(g(arg) == 2, "");
    assert(g(ICE(a2)) == 5);
}
