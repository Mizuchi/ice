#include <chrono>
#include <iostream>
#include <algorithm>
#include <cstdlib>

const int maxDimension = 20;

struct Model {
    int dimension;
    double w[maxDimension];
    double b;
};

bool isFirstClass(Model m, std::vector<double> x) {
    double s = 0;
    for (int i = 0; i < m.dimension; i++) {
        s += m.w[i] * x[i];
    }
    return s - m.b >= 0;
}

std::vector<std::vector<double>> gen_examples(int dimension, int count) {
    auto gen_random = []() {
        double r = double(rand()) / RAND_MAX;
        return r * 10 - 5; // return random number in [-5, 5]
    };

    auto gen_example = [=]() {
        std::vector<double> x(dimension);
        std::generate(x.begin(), x.end(), gen_random);
        return x;
    };

    std::vector<std::vector<double>> ret(count);
    std::generate(ret.begin(), ret.end(), gen_example);
    return ret;
}

constexpr Model get_model() {
    return {10, {-0.9, 4.1, -3.5, -4.8, -3.1, 0.4, 1.6, -1.2, 4.9, 4.5}, 0};
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
