template <class T>
constexpr int fib(Constexpr<T> t) {
    static_assert(t >= 0, "");
    return t <= 1 ? t : fib(t - 1) + fib(t - 2);
}

int fib(int t) {
    static std::vector<int> results = {0, 1};
    if (results.size() <= t) {
        int ret = fib(t - 1) + fib(t - 2);
        results.push_back(ret);
    }
    return results[t];
}

constexpr int four() { return 4; }

int main() {
  static_assert(fib(ICE_REF(four())) == 3, ""); // constexpr version

  static_assert(fib(ICE_REF(5)) == 5, ""); // constexpr version

  constexpr int i = 6;
  static_assert(fib(ICE_REF(6)) == 8, ""); // constexpr version

  // fib(ICE_REF(four() - i)); compile time error

  int j = 7;
  assert(fib(ICE_REF(7)) == 13); // non-constexpr version
}


