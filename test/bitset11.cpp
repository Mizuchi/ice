#include <ice/ice.hpp>
#include <boost/dynamic_bitset.hpp>
#include <bitset>
template <class T> auto make_bitset(ice::Const<T> t) -> std::bitset<T::get()> {
    return {};
}
boost::dynamic_bitset<> make_bitset(size_t n) { return boost::dynamic_bitset<>{n}; }

int main() {
  auto a = make_bitset(ICE(100));
  static_assert(std::is_same<decltype(a), std::bitset<100>>::value, "");

  size_t n = 100;
  auto b = make_bitset(ICE(n));
  static_assert(std::is_same<decltype(b), boost::dynamic_bitset<>>::value, "");
}
