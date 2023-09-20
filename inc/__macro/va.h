#pragma once

#ifdef va_debug
#  define echo(arg) >>>>>>>>> va_##arg
#endif

/*
 * [](https://github.com/notfoundry/ppstep)
 * [](https://github.com/swansontec/map-macro)
 * [](http://jhnet.co.uk/articles/cpp_magic)
 * [](https://disenone.github.io/2021/03/31/cpp-preprocess)
 * [](https://github.com/pfultz2/Cloak/wiki/C-Preprocessor-tricks,-tips,-and-idioms#deferred-expression)
 * */

/* clang-format off */
/* eval */
#define va_eval(...)      __va_eval_16(__VA_ARGS__)
#define __va_eval_1(...)  __VA_ARGS__
#define __va_eval_2(...)  __va_eval_1(__va_eval_1(__VA_ARGS__))
#define __va_eval_4(...)  __va_eval_2(__va_eval_2(__VA_ARGS__))
#define __va_eval_8(...)  __va_eval_4(__va_eval_4(__VA_ARGS__))
#define __va_eval_16(...) __va_eval_8(__va_eval_8(__VA_ARGS__))


/* defer call */
#define defer(n, m) va_cat(defer_, n)(m)
#define space()
#define defer_1(m)  m space()
#define defer_2(m)  m space space()()
#define defer_3(m)  m space space space()()()
#define defer_4(m)  m space space space()()()


/* cat */
#define va_cat(x, y) x##y

#ifdef va_debug
echo(cat)
va_cat(hello, world) /* : helloworld */
#endif


/* has args */
#define va_has(...)          __va_has(__VA_OPT__(1, ) 0)
#define __va_has(first, ...) first

#ifdef va_debug
echo(has)
va_has()        /* : 0 */
va_has(0)       /* : 1 */
va_has(0, 1, 2) /* : 1 */
#endif


/* inc */
#define va_inc(x)   va_cat(__va_inc_, x)
#define __va_inc_0  1
#define __va_inc_1  2
#define __va_inc_2  3
#define __va_inc_3  4
#define __va_inc_4  5
#define __va_inc_5  6
#define __va_inc_6  7
#define __va_inc_7  8
#define __va_inc_8  9
#define __va_inc_9  10
#define __va_inc_10 11
#define __va_inc_11 12
#define __va_inc_12 13
#define __va_inc_13 14
#define __va_inc_14 15
#define __va_inc_15 16
#define __va_inc_16 16

#ifdef va_debug
echo(inc)
va_inc(0)  /* : 1  */
va_inc(1)  /* : 2  */
va_inc(15) /* : 16 */
va_inc(16) /* : 16 */
#endif


/* inc */
#define va_dec(x)   va_cat(__va_dec_, x)
#define __va_dec_0  0
#define __va_dec_1  0
#define __va_dec_2  1
#define __va_dec_3  2
#define __va_dec_4  3
#define __va_dec_5  4
#define __va_dec_6  5
#define __va_dec_7  6
#define __va_dec_8  7
#define __va_dec_9  8
#define __va_dec_10 9
#define __va_dec_11 10
#define __va_dec_12 11
#define __va_dec_13 12
#define __va_dec_14 13
#define __va_dec_15 14
#define __va_dec_16 15

#ifdef va_debug
echo(inc)
va_dec(0)  /* : 0  */
va_dec(1)  /* : 0  */
va_dec(15) /* : 14 */
va_dec(16) /* : 15 */
#endif


/* bool */
#define va_bool(x)          __is_probe(__bool_##x)
#define __second(a, b, ...) b
#define __is_probe(...)     __second(__VA_ARGS__, 1)
#define __probe()           ~, 0
#define __bool_0            __probe()

#ifdef va_debug
echo(bool)
va_bool(0)  /* : 0 */
va_bool(1)  /* : 1 */
va_bool(15) /* : 1 */
va_bool(16) /* : 1 */
#endif


/* if */
#define va_if(condition) va_cat(__va_if_, condition)
#define __va_if_1(...) __VA_ARGS__
#define __va_if_0(...)

#ifdef va_debug
echo(if)
va_if(0)(true)  /* :      */
va_if(1)(true)  /* : true */
#endif


/* if_else */
#define va_elseif(condition)    va_cat(__va_elseif_, condition)
#define __va_elseif_1_else(...)
#define __va_elseif_0_else(...) __VA_ARGS__
#define __va_elseif_1(...)      __VA_ARGS__ __va_elseif_1_else
#define __va_elseif_0(...)      __va_elseif_0_else

#ifdef va_debug
echo(if_else)
va_elseif(0)(true)(false)  /* : false */
va_elseif(1)(true)(false)  /* : true  */
#endif


/* map */
#define va_map(macro, ...) va_eval(__va_map(macro, __VA_ARGS__))
#define __va_map_def()     __va_map
#define __va_map(macro, first, ...)                                                                \
  macro(first)                                                                                     \
  va_if(va_has(__VA_ARGS__)) (                                                                     \
    defer(2, __va_map_def)()(macro, __VA_ARGS__)                                                   \
  )

#ifdef va_debug
echo(map)
#define type_of_int(x) (int)x,
va_map(type_of_int, 1, 2, 3, 4) /* : (int)1, (int)2, (int)3, (int)4 */
#endif


/* cut */
#define va_cut(num, ...)               va_eval(__va_cut(1, num, __VA_ARGS__))
#define __va_cut_def()                 __va_cut
#define __va_cut_cat(prefix, num, has) prefix##num##has
#define __va_cut_ok_11                 1
#define __va_cut_ok_10                 0
#define __va_cut_ok_01                 0
#define __va_cut_ok_00                 0
#define __va_cut_ok(num, has)          __va_cut_cat(__va_cut_ok_, num, has)
#define __va_cut(first, num, arg, ...)                                                             \
  va_if(__va_cut_ok(va_bool(num), va_has(__VA_ARGS__))) (                                                                            \
    va_elseif(first) (                                                                             \
      arg                                                                                          \
    )(                                                                                             \
      , arg                                                                                        \
    )                                                                                              \
                                                                                                   \
    defer(2, __va_cut_def)()(va_dec(first), va_dec(num), __VA_ARGS__)                              \
  )                                                                                                \
                                                                                                   \
  va_elseif(va_has(__VA_ARGS__)) (                                                                 \
  )(                                                                                               \
    va_if(va_bool(num)) (                                                                          \
      , arg                                                                                        \
    )                                                                                              \
  )

#ifdef va_debug
echo(cut)
va_cut(0, 0, 1, 2, 3, 4) /* :                   */
va_cut(1, 0, 1, 2, 3, 4) /* : 0                 */
va_cut(2, 0, 1, 2, 3, 4) /* : 0 , 1             */
va_cut(3, 0, 1, 2, 3, 4) /* : 0 , 1 , 2         */
va_cut(4, 0, 1, 2, 3, 4) /* : 0 , 1 , 2 , 3     */
va_cut(5, 0, 1, 2, 3, 4) /* : 0 , 1 , 2 , 3 , 4 */
va_cut(6, 0, 1, 2, 3, 4) /* : 0 , 1 , 2 , 3 , 4 */
#endif


/* slice */
#define va_slice(num, ...)               va_eval(__va_slice(1, num, __VA_ARGS__))
#define __va_slice_def()                 __va_slice
#define __va_slice(first, num, arg, ...)                                                           \
  va_elseif(va_bool(num)) (                                                                        \
    defer(2, __va_slice_def)()(first, va_dec(num), __VA_ARGS__)                                    \
  )(                                                                                               \
    va_elseif(va_has(__VA_ARGS__)) (                                                               \
      va_elseif(first) (                                                                           \
        arg                                                                                        \
      )(                                                                                           \
        , arg                                                                                      \
      )                                                                                            \
                                                                                                   \
      defer(3, __va_slice_def)()(va_dec(first), va_dec(num), __VA_ARGS__)                          \
    )(                                                                                             \
      va_elseif(first) (                                                                           \
        arg                                                                                        \
      )(                                                                                           \
        , arg                                                                                      \
      )                                                                                            \
    )                                                                                              \
  )

#ifdef va_debug
echo(slice)
va_slice(0, 0, 1, 2, 3, 4) /* : 0 , 1 , 2 , 3 , 4 */ 
va_slice(1, 0, 1, 2, 3, 4) /* : 1 , 2 , 3 , 4     */
va_slice(2, 0, 1, 2, 3, 4) /* : 2 , 3 , 4         */
va_slice(3, 0, 1, 2, 3, 4) /* : 3 , 4             */
va_slice(4, 0, 1, 2, 3, 4) /* : 4                 */
va_slice(5, 0, 1, 2, 3, 4) /* :                   */
va_slice(6, 0, 1, 2, 3, 4) /* :                   */
#endif


/* at */
#define va_at(i, ...)             va_eval(defer(1, __va_at_first)(va_slice(i, __VA_ARGS__)))
#define __va_at_first(first, ...) first

#ifdef va_debug
echo(at)
va_at(0, 0, 1, 2, 3, 4) /* : 0 */
va_at(1, 0, 1, 2, 3, 4) /* : 1 */
va_at(2, 0, 1, 2, 3, 4) /* : 2 */
va_at(3, 0, 1, 2, 3, 4) /* : 3 */
va_at(4, 0, 1, 2, 3, 4) /* : 4 */
va_at(5, 0, 1, 2, 3, 4) /* :   */
va_at(6, 0, 1, 2, 3, 4) /* :   */
#endif

  
/* opt */
#define va_opt(i, ...) __va_opt(va_slice(i, __VA_ARGS__))
#define __va_opt(...)  __VA_OPT__(, )

#ifdef va_debug
echo(opt)
va_opt(3)                   /* :   */
va_opt(3, 0)                /* :   */
va_opt(3, 0, 1)             /* :   */
va_opt(3, 0, 1, 2)          /* :   */
va_opt(3, 0, 1, 2, 3)       /* : , */
va_opt(3, 0, 1, 2, 3, 4)    /* : , */
va_opt(3, 0, 1, 2, 3, 4, 5) /* : , */
#endif


/* th */
#define va_th(i, def, ...) va_at(0, va_at(i, __VA_ARGS__) va_opt(i, __VA_ARGS__) def)
#define va_0th(def, ...)   va_th(0, def, __VA_ARGS__)
#define va_1th(def, ...)   va_th(1, def, __VA_ARGS__)
#define va_2th(def, ...)   va_th(2, def, __VA_ARGS__)

#ifdef va_debug
echo(th)
va_th(3, Y)                   /* : Y */
va_th(3, Y, 0)                /* : Y */
va_th(3, Y, 0, 1)             /* : Y */
va_th(3, Y, 0, 1, 2)          /* : Y */
va_th(3, Y, 0, 1, 2, 3)       /* : 3 */
va_th(3, Y, 0, 1, 2, 3, 4)    /* : 3 */
va_th(3, Y, 0, 1, 2, 3, 4, 5) /* : 3 */
#endif


/* size */
#define va_size(...)    (va_eval(__va_size(1, "ignore", __VA_ARGS__)) - 1)
#define __va_size_def() __va_size
#define __va_size(first, arg,  ...) \
  va_if(va_has(__VA_ARGS__)) (                                                                     \
    va_elseif(first) (                                                                             \
      1                                                                                            \
    )(                                                                                             \
      + 1                                                                                          \
    )                                                                                              \
                                                                                                   \
    defer(2, __va_size_def)()(va_dec(first), __VA_ARGS__)                                          \
  )                                                                                                \
                                                                                                   \
  va_elseif(va_has(__VA_ARGS__)) (                                                                 \
  )(                                                                                               \
    + 1                                                                                            \
  )

#ifdef va_debug
echo(size)
va_size()                 /* : (+ 1 -1)                        */
va_size(0)                /* : (1 + 1 - 1)                     */
va_size(0, 1)             /* : (1 + 1 + 1 - 1)                 */
va_size(0, 1, 2)          /* : (1 + 1 + 1 + 1 - 1)             */
va_size(0, 1, 2, 3)       /* : (1 + 1 + 1 + 1 + 1 - 1)         */
va_size(0, 1, 2, 3, 4)    /* : (1 + 1 + 1 + 1 + 1 + 1 - 1)     */
va_size(0, 1, 2, 3, 4, 5) /* : (1 + 1 + 1 + 1 + 1 + 1 + 1 - 1) */
#endif
