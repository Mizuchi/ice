#define IS_CONSTEXPR(expr)                                                     \
    (__builtin_constant_p(expr) || __builtin_constant_p(({                     \
        auto kfjhddtgdcvteclundrrblvcjcglvkbg = (expr);                        \
        static_cast<void>(kfjhddtgdcvteclundrrblvcjcglvkbg);                   \
        0;                                                                     \
                                   })))
