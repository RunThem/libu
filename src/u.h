#pragma once

/* __va__ */
/* base */
#define __va_cut_one(_0, ...) __VA_ARGS__
#define __va_at_one(_0, ...)  _0

#define __va_cut_0(...) __VA_ARGS__
#define __va_cut_1(...) __va_cut_one(__VA_ARGS__)
#define __va_cut_2(...) __va_cut_1(__va_cut_one(__VA_ARGS__))
#define __va_cut_3(...) __va_cut_2(__va_cut_one(__VA_ARGS__))
#define __va_cut_4(...) __va_cut_3(__va_cut_one(__VA_ARGS__))
#define __va_cut_5(...) __va_cut_4(__va_cut_one(__VA_ARGS__))

#define __va_at_0(...) __va_at_one(__VA_ARGS__)
#define __va_at_1(...) __va_at_0(__va_cut_1(__VA_ARGS__))
#define __va_at_2(...) __va_at_0(__va_cut_2(__VA_ARGS__))
#define __va_at_3(...) __va_at_0(__va_cut_3(__VA_ARGS__))
#define __va_at_4(...) __va_at_0(__va_cut_4(__VA_ARGS__))
#define __va_at_5(...) __va_at_0(__va_cut_5(__VA_ARGS__))

#define __va_cat(x, y)                                 x##y
#define __va_at(i, ...)                                __va_cat(__va_at_, i)(__VA_ARGS__)
#define __va_cut(i, ...)                               __va_cat(__va_cut_, i)(__VA_ARGS__)
#define __va_size(_0, _1, _2, _3, _4, _5, _6, _n, ...) _n

#define va_cat(x, y)   __va_cat(x, y)
#define va_at(i, ...)  __va_at(i __VA_OPT__(, ) __VA_ARGS__)
#define va_cut(i, ...) __va_cut(i __VA_OPT__(, ) __VA_ARGS__)
#define va_size(...)   __va_size("ignore" __VA_OPT__(, ) __VA_ARGS__, 6, 5, 4, 3, 2, 1, 0)
#define va_empty(...)  __va_size("ignore" __VA_OPT__(, ) __VA_ARGS__, 0, 0, 0, 0, 0, 0, 1)
