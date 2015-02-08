#include <type_traits>
namespace detail {

#define VAR(a) _SBL_VAR(a, __LINE__)
#define _SBL_VAR(a, b) __SBL_VAR(a, b)
#define __SBL_VAR(s, B) _sBl##s##B##l

#ifndef __clang__
template <typename T>
constexpr typename std::remove_reference<T>::type makeprval(T &&t) {
    return t;
}
#define IS_CONSTEXPR(e) noexcept(detail::makeprval(e))
#else
template <typename T> constexpr int zero(T) { return 0; }
#define IS_CONSTEXPR(expr) __builtin_constant_p(detail::zero(expr))
#endif

template <class T> struct ConstTag {};
template <class T> struct NonconstTag {};
template <class T> struct Ice;
template <class T> struct Ice<ConstTag<T>> : T {};
template <class T> struct Ice<NonconstTag<T>> : T {};

template <class T1, class T2>
constexpr const T1 choose_expr(std::true_type, T1 &&expr1, T2 &&) {
    return expr1;
}
template <class T1, class T2>
constexpr const T2 choose_expr(std::false_type, T1 &&, T2 &&expr2) {
    return expr2;
}
}
template <class T> using Const = detail::Ice<detail::ConstTag<T>>;
template <class T> using Nonconst = detail::Ice<detail::NonconstTag<T>>;
template <class T> using Any = detail::Ice<T>;

#define FROZEN(expr)                                                           \
    [&]() {                                                                    \
        using VAR(t) = std::integral_constant<bool, IS_CONSTEXPR(expr)>;       \
        auto make_constexpr = [&]() {                                          \
            static constexpr auto VAR(expr1) =                                 \
                detail::choose_expr(VAR(t){}, (expr), 0);                      \
            struct VAR(T) {                                                    \
                constexpr operator decltype(VAR(expr1))() const {              \
                    return VAR(expr1);                                         \
                }                                                              \
                static constexpr decltype(VAR(expr1)) get() {                  \
                    return VAR(expr1);                                         \
                }                                                              \
            };                                                                 \
            return Const<VAR(T)>{};                                            \
        };                                                                     \
        auto make_expr = [&]() {                                               \
            static const auto VAR(expr1) = (expr);                             \
            struct VAR(T) {                                                    \
                operator decltype(VAR(expr1))() const { return VAR(expr1); }   \
                static decltype(VAR(expr1)) get() { return VAR(expr1); }       \
            };                                                                 \
            return Nonconst<VAR(T)>{};                                         \
        };                                                                     \
        return detail::choose_expr(VAR(t){}, make_constexpr(), make_expr());   \
    }()
