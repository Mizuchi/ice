#include "c.h"

template <class T>
bool isFirstClass(folly::ice::Any<T> t, std::vector<double> x) {
    double s = 0;
    for (int i = 0; i < t.get().dimension; i++) {
        s += t.get().w[i] * x[i];
    }
    return s - t.get().b >= 0;
}

int main() {
    auto examples = gen_examples(10, 5e6);
    bool b = false;
    auto m = ICE(get_model());

    auto t1 = std::chrono::system_clock::now();
    for (auto &i : examples) {
        b ^= isFirstClass(m, i);
    }
    auto t2 = std::chrono::system_clock::now();
    std::cout << std::chrono::duration<double>(t2 - t1).count() << " seconds"
              << std::endl;

    return b;
}
