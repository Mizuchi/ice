#pragma once
#include <type_traits>

namespace ice {
namespace detail {

#ifndef __clang__
template <typename T>
constexpr typename std::remove_reference<T>::type makeprval(T &&t) {
    return t;
}
#define ICE_IS_CONSTEXPR(e) noexcept(::ice::detail::makeprval(e))
#else
// XXX: remove this when the following clang bug is fixed
// http://llvm.org/bugs/show_bug.cgi?id=15481
// BTW, the approach uses another clang bug, which is fixed in 3.7
template <typename T> constexpr int zero(T) { return 0; }
#define ICE_IS_CONSTEXPR(e) __builtin_constant_p(::ice::detail::zero(e))
#endif // ifndef __clang__

template <class T1, class T2>
constexpr const T1 choose_expr(std::true_type, T1 &&expr1, T2 &&) {
    return expr1;
}
template <class T1, class T2>
constexpr const T2 choose_expr(std::false_type, T1 &&, T2 &&expr2) {
    return expr2;
}

template <class T> struct Const : T {};

template <class T> struct Nonconst : T {};

template <class T> struct Any : T {};

template <class T> struct IsIceImpl : std::false_type {};

template <class T> struct IsIceImpl<Any<T>> : std::true_type {};

template <class T> struct IsIce : IsIceImpl<typename std::decay<T>::type> {};
} // namespace detail

using detail::Any;

template <class T> using Const = Any<detail::Const<T>>;

template <class T> using Nonconst = Any<detail::Nonconst<T>>;

// generate a unique variable name
#define ICE_VAR(a) DETAIL_ICE_VAR(a, __LINE__)
#define DETAIL_ICE_VAR(a, b) DETAIL_ICE_VAR_IMPL(a, b)
#define DETAIL_ICE_VAR_IMPL(a, b) _ice_guujijgtldsjlfaghnbjzxcvjkbnrdet##a##b

// XXX: use [&] instead of [=] after the following GCC bug is fixed
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=64977
#define ICE_IMPL(expr)                                                         \
    [=]() {                                                                    \
        using namespace ice;                                            \
        using detail::choose_expr;                                             \
        using ICE_VAR(b) =                                                     \
            std::integral_constant<bool, ICE_IS_CONSTEXPR(expr)>;              \
        auto make_constexpr = [=]() {                                          \
            static constexpr auto ICE_VAR(e) =                                 \
                choose_expr(ICE_VAR(b){}, (expr), 0);                          \
            struct ICE_VAR(T) {                                                \
                constexpr ICE_VAR(T)() {}                                      \
                constexpr ICE_VAR(T)(ICE_VAR(T) && ) {}                        \
                constexpr ICE_VAR(T)(const ICE_VAR(T) &) {}                    \
                constexpr operator decltype(ICE_VAR(e))() const {              \
                    return ICE_VAR(e);                                         \
                }                                                              \
                static constexpr decltype(ICE_VAR(e)) get() {                  \
                    return ICE_VAR(e);                                         \
                }                                                              \
            };                                                                 \
            return Const<ICE_VAR(T)>{};                                        \
        }; /* make_constexpr lambda */                                         \
        auto make_expr = [=]() {                                               \
            static const auto ICE_VAR(e) = (expr);                             \
            struct ICE_VAR(T) {                                                \
                operator decltype(ICE_VAR(e))() const { return ICE_VAR(e); }   \
                static decltype(ICE_VAR(e)) get() { return ICE_VAR(e); }       \
            };                                                                 \
            return Nonconst<ICE_VAR(T)>{};                                     \
        }; /* make_expr lambda */                                              \
        return choose_expr(ICE_VAR(b){}, make_constexpr(), make_expr());       \
    }()

#define ICE(expr)                                                              \
    ::ice::detail::choose_expr(                                         \
        ::ice::detail::IsIce<decltype(expr)>{}, (expr), ICE_IMPL(expr))

} // namespace ice
