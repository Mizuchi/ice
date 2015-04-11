#include <ice/ice.hpp>
#include <vector>
#include <array>

template <class T>
auto make_array(ice::Const<T> t) -> std::array<int, T::get()> {
    return {};
}
std::vector<int> make_array(size_t n) { return std::vector<int>(n); }

int main() {
    auto a = make_array(ICE(100));
    static_assert(std::is_same<decltype(a), std::array<int, 100>>::value, "");

    size_t n = 100;
    auto b = make_array(ICE(n));
    static_assert(std::is_same<decltype(b), std::vector<int>>::value, "");
}
