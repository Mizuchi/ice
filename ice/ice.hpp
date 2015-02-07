#pragma once
#define IS_CONSTEXPR(expr)                                                     \
    (__builtin_constant_p(expr) || __builtin_constant_p(({                     \
        auto kfjhddtgdcvteclundrrblvcjcglvkbg = (expr);                        \
        static_cast<void>(kfjhddtgdcvteclundrrblvcjcglvkbg);                   \
        0;                                                                     \
                                   })))

template <class T> struct TagConst {};
template <class T> struct TagNonconst {};
template <class T> struct Ice;

template <class T> struct Ice<TagConst<T>> {
    using type = decltype(T::get());
    static constexpr auto value = T::get();
    constexpr operator auto() { return value; }
};

template <class T> struct Ice<TagNonconst<T>> {
    using type = T;
    const T value;
    operator auto() const { return value; }
};

template <class T> using Const = Ice<TagConst<T>>;
template <class T> using Nonconst = Ice<TagNonconst<T>>;
template <class T> using Any = Ice<T>;

#define FROZEN(expr)                                                           \
    __builtin_choose_expr(IS_CONSTEXPR(expr), ({                               \
        static constexpr auto _36uH5e =                                        \
            __builtin_choose_expr(IS_CONSTEXPR(expr), (expr), 0);              \
        struct __6uH5e {                                                       \
            static constexpr auto get() { return _36uH5e; }                    \
        };                                                                     \
        Const<__6uH5e>{};                                                      \
                                              }),                              \
                          Nonconst<decltype(expr)>{expr})
