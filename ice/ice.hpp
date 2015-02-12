#pragma once
#include <type_traits>
namespace folly {
namespace frozen {
namespace detail {

#define VAR(a) _SBL_VAR(a, __LINE__)
#define _SBL_VAR(a, b) __SBL_VAR(a, b)
#define __SBL_VAR(s, B) _sBl##s##B##l

#ifndef __clang__
template <typename T>
constexpr typename std::remove_reference<T>::type makeprval(T &&t) {
    return t;
}
#define IS_CONSTEXPR(e) noexcept(::folly::frozen::detail::makeprval(e))
#else
// TODO: remove this when the following bug is fixed
// http://llvm.org/bugs/show_bug.cgi?id=15481
// BTW, the approach uses another clang bug, which is fixed in 3.7
template <typename T> constexpr int zero(T) { return 0; }
#define IS_CONSTEXPR(expr)                                                     \
    __builtin_constant_p(::folly::frozen::detail::zero(expr))
#endif

template <class T1, class T2>
constexpr const T1 choose_expr(std::true_type, T1 &&e, T2 &&) {
    return e;
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
template <class T> struct IsFrozen : std::false_type {};
template <class T> struct IsFrozen<Any<T>> : std::true_type {};
}

// TODO: use [&] instead of [=] after the following bug is fixed
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=64977
#define FROZEN_IMPL(expr)                                                      \
    [=]() {                                                                    \
        using namespace folly::frozen;                                         \
        using detail::choose_expr;                                             \
        using VAR(t) = std::integral_constant<bool, IS_CONSTEXPR(expr)>;       \
        auto make_constexpr = [=]() {                                          \
            static constexpr auto VAR(e) = choose_expr(VAR(t){}, (expr), 0);   \
            struct VAR(T) {                                                    \
                constexpr operator decltype(VAR(e))() const { return VAR(e); } \
                static constexpr decltype(VAR(e)) get() { return VAR(e); }     \
            };                                                                 \
            return Const<VAR(T)>{};                                            \
        };                                                                     \
        auto make_expr = [=]() {                                               \
            static const auto VAR(e) = (expr);                                 \
            struct VAR(T) {                                                    \
                operator decltype(VAR(e))() const { return VAR(e); }           \
                static decltype(VAR(e)) get() { return VAR(e); }               \
            };                                                                 \
            return Nonconst<VAR(T)>{};                                         \
        };                                                                     \
        return choose_expr(VAR(t){}, make_constexpr(), make_expr());           \
    }()

#define FROZEN(expr)                                                           \
    (::folly::frozen::detail::choose_expr(                                     \
        ::folly::frozen::detail::IsFrozen<decltype(expr)>{}, (expr),           \
        FROZEN_IMPL(expr)))
}
}
