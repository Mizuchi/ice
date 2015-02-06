#include <ice/is_constexpr.hpp>

struct A {};

int main() {
    static_assert(IS_CONSTEXPR(1) == true);

    int i = 1;
    static_assert(IS_CONSTEXPR(i) == false);

    constexpr int j = 0;
    static_assert(IS_CONSTEXPR(j) == true);

    A a;
    static_assert(IS_CONSTEXPR(a) == false);

    constexpr A a2{};
    static_assert(IS_CONSTEXPR(a2) == true);
}
