#pragma once
#include <type_traits>
namespace folly {
namespace ice {
namespace detail {

#define ICE_VAR(a) _ICE_VAR(a, __LINE__)
#define _ICE_VAR(a, b) __ICE_VAR(a, b)
#define __ICE_VAR(a, b) __ice_var_guujijgtldsjlfaghnbjzxcvjkbnrdet##a##b

#ifndef __clang__
template <typename T>
constexpr typename std::remove_reference<T>::type makeprval(T &&t) {
    return t;
}
#define ICE_IS_CONSTEXPR(e) noexcept(::folly::ice::detail::makeprval(e))
#else
// XXX: remove this when the following bug is fixed
// http://llvm.org/bugs/show_bug.cgi?id=15481
// BTW, the approach uses another clang bug, which is fixed in 3.7
template <typename T> constexpr int zero(T) { return 0; }
#define ICE_IS_CONSTEXPR(e) __builtin_constant_p(::folly::ice::detail::zero(e))
#endif

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
} // namespace detail

template <class T> struct Any : T {};
template <class T> using Const = Any<detail::Const<T>>;
template <class T> using Nonconst = Any<detail::Nonconst<T>>;

namespace detail {
template <class T> struct IsIceImpl : std::false_type {};
template <class T> struct IsIceImpl<Any<T>> : std::true_type {};
template <class T> struct IsIce : IsIceImpl<typename std::decay<T>::type> {};
}

// XXX: use [&] instead of [=] after the following bug is fixed
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=64977
#define ICE_IMPL(expr)                                                         \
    [=]() {                                                                    \
        using namespace folly::ice;                                            \
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
        };                                                                     \
        auto make_expr = [=]() {                                               \
            static const auto ICE_VAR(e) = (expr);                             \
            struct ICE_VAR(T) {                                                \
                operator decltype(ICE_VAR(e))() const { return ICE_VAR(e); }   \
                static decltype(ICE_VAR(e)) get() { return ICE_VAR(e); }       \
            };                                                                 \
            return Nonconst<ICE_VAR(T)>{};                                     \
        };                                                                     \
        return choose_expr(ICE_VAR(b){}, make_constexpr(), make_expr());       \
    }()

#define ICE(expr)                                                              \
    ::folly::ice::detail::choose_expr(                                         \
        ::folly::ice::detail::IsIce<decltype(expr)>{}, (expr), ICE_IMPL(expr))
}
}
