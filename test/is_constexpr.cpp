#include <ice/ice.hpp>

struct A {};

int main() {
    static_assert(IS_CONSTEXPR(1) == true, "");

    int i = 1;
    static_assert(IS_CONSTEXPR(i) == false, "");

    constexpr int j = 0;
    static_assert(IS_CONSTEXPR(j) == true, "");
    static_assert(IS_CONSTEXPR(j + 42) == true, "");
    static_assert(IS_CONSTEXPR(i + j) == false, "");

    A a;
    static_assert(IS_CONSTEXPR(a) == false, "");

    constexpr A a2{};
    static_assert(IS_CONSTEXPR(a2) == true, "");
}
