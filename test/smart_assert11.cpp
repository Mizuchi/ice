#include <ice/ice.hpp>
#include <cassert>
#include <cmath>
#include <stdexcept>

void smart_assert(bool t) { t ? 0 : throw 0; /* assert(t); */ }
template <class T> void smart_assert(folly::ice::Const<T> t) {
    static_assert(t, "error");
}

template <class T> double checked_sqrt(folly::ice::Any<T> f) {
    smart_assert(f >= 0);
    return sqrt(f);
}

int main() {
    checked_sqrt(ICE(5)); // okay
    double s = -1;
    try {
        checked_sqrt(ICE(s)); // runtime error
        assert(false);
    } catch (...) {
    }
    s = 5;
    checked_sqrt(ICE(s));
    // checked_sqrt(ICE(-1)); // compile-time error
    // checked_sqrt(5); // compile-time error
}
