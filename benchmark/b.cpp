#include "c.h"

bool isFirstClass(Model m, std::vector<double> x) {
    double s = 0;
    for (int i = 0; i < m.dimension; i++) {
        s += m.w[i] * x[i];
    }
    return s - m.b >= 0;
}

int main() {
    auto examples = gen_examples(10, 5e6);
    bool b = false;
    constexpr auto m = get_model();

    auto t1 = std::chrono::system_clock::now();
    for (auto &i : examples) {
        b ^= isFirstClass(m, i);
    }
    auto t2 = std::chrono::system_clock::now();
    std::cout << std::chrono::duration<double>(t2 - t1).count() << " seconds"
              << std::endl;

    return b;
}
