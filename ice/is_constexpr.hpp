#include<type_traits>
namespace ice {
namespace detail {
template <typename T> constexpr int makeprval(T) {
    return 0;
}
} // namespace detail
} // namespace yuki

#define IS_CONSTEXPR(expr)                                                     \
    (__builtin_constant_p(expr) ||                                             \
     __builtin_constant_p(::ice::detail::makeprval(expr)) ||                  \
     noexcept(::ice::detail::makeprval(expr)))
