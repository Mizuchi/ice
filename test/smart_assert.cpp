#include <ice/ice.hpp>
#include <cassert>
#include <cmath>
#include <stdexcept>

void smart_assert(bool t) { t ? 0 : throw 0; /* assert(t); */ }
template <class T> void smart_assert(folly::Const<T> t) {
    static_assert(t, "error");
}

template <class T> auto checked_sqrt(folly::Any<T> f) {
    smart_assert(f >= 0);
    return sqrt(f);
}

int main() {
    checked_sqrt(FROZEN(5)); // okay
    double s = -1;
    try {
        checked_sqrt(FROZEN(s)); // runtime error
        assert(false);
    } catch (...) {
    }
    s = 5;
    checked_sqrt(FROZEN(s));
    // checked_sqrt(FROZEN(-1)); // compile-time error
    // checked_sqrt(5); // compile-time error
}
