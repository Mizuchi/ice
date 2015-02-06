#include <ice/is_constexpr.hpp>
#include <cstdio>

template <class T, class U = decltype(T{}.get())> struct R {
    constexpr operator U() { return T{}.get(); }
};

#define CONST(expr)                                                            \
    __builtin_choose_expr(IS_CONSTEXPR(expr), ({                               \
        static constexpr auto t =                                              \
            __builtin_choose_expr(IS_CONSTEXPR(expr), (expr), 0);              \
        struct T {                                                             \
            constexpr auto get() { return t; }                                 \
        };                                                                     \
        R<T>{};                                                                \
                                              }),                              \
                          (expr))

struct A {
    constexpr A(int i) : i(i) {}
    constexpr int get() { return i; }
    int i;
};

template <class T> void g(R<T> t) {
    static_assert(A(t).get() == 1);
    puts("constexpr");
}

void g(A) { puts("not constexpr"); }

int main() {
    A a1{1};
    constexpr A a2{1};
    g(CONST(a1));
    g(CONST(a2));
}
