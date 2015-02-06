template <class T> void smart_assert(Constexpr<T> t) {
    static_assert(t, "error");
}
void smart_assert(bool t) { assert(t); }

template <class Float> auto checked_sqrt(Float f) -> decltype(std::sqrt(f)) {
    smart_assert(f >= 0);
    return std::sqrt(f);
}

int main() {
    double s = -1;
    checked_sqrt(s); // runtime error
    // checked_sqrt(-1); // compile-time error
    return checked_sqrt(5); // okay
}

