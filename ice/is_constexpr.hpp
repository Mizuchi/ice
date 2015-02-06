#define IS_CONSTEXPR(expr)                                                     \
    (__builtin_constant_p(expr) || __builtin_constant_p(({                     \
        auto _ = (expr);                                                       \
        static_cast<void>(_);                                                  \
        0;                                                                     \
                                   })))
