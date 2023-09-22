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
 **/

/* clang-format off */
#ifdef va_debug
#  define echo(arg) >>>>>>>>> va_##arg
#endif


/*
 * cat
 **/
#define va_cat(x, y, ...) x##y##__VA_ARGS__

#ifdef va_debug
echo(cat)
va_cat(hello_, world)             /* : hello_world */
va_cat(hello_, the_, world)       /* : hello_the_world */
#endif


/*
 * eval 2^8 128
 **/
#define va_eval(...)     __va_eval_8(__VA_ARGS__)
#define __va_eval_1(...) __VA_ARGS__
#define __va_eval_2(...) __va_eval_1(__va_eval_1(__VA_ARGS__))
#define __va_eval_3(...) __va_eval_2(__va_eval_2(__VA_ARGS__))
#define __va_eval_4(...) __va_eval_3(__va_eval_3(__VA_ARGS__))
#define __va_eval_5(...) __va_eval_4(__va_eval_4(__VA_ARGS__))
#define __va_eval_6(...) __va_eval_5(__va_eval_5(__VA_ARGS__))
#define __va_eval_7(...) __va_eval_6(__va_eval_6(__VA_ARGS__))
#define __va_eval_8(...) __va_eval_7(__va_eval_7(__VA_ARGS__))


/*
 * defer call
 **/
#define va_defer(n, m) va_cat(__va_defer_, n)(m)
#define space()
#define __va_defer_1(macro) macro space()
#define __va_defer_2(macro) macro space space()()
#define __va_defer_3(macro) macro space space space()()()
#define __va_defer_4(macro) macro space space space space()()()()
#define __va_defer_5(macro) macro space space space space space()()()()()
#define __va_defer_6(macro) macro space space space space space space()()()()()()
#define __va_defer_7(macro) macro space space space space space space space()()()()()()()
#define __va_defer_8(macro) macro space space space space space space space space()()()()()()()()


/*
 * inc
 **/
#define va_inc(x)    va_cat(__va_inc_, x)
#define __va_inc_0   1
#define __va_inc_1   2
#define __va_inc_2   3
#define __va_inc_3   4
#define __va_inc_4   5
#define __va_inc_5   6
#define __va_inc_6   7
#define __va_inc_7   8
#define __va_inc_8   9
#define __va_inc_9   10
#define __va_inc_10  11
#define __va_inc_11  12
#define __va_inc_12  13
#define __va_inc_13  14
#define __va_inc_14  15
#define __va_inc_15  16
#define __va_inc_16  17
#define __va_inc_17  18
#define __va_inc_18  19
#define __va_inc_19  20
#define __va_inc_20  21
#define __va_inc_21  22
#define __va_inc_22  23
#define __va_inc_23  24
#define __va_inc_24  25
#define __va_inc_25  26
#define __va_inc_26  27
#define __va_inc_27  28
#define __va_inc_28  29
#define __va_inc_29  30
#define __va_inc_30  31
#define __va_inc_31  32
#define __va_inc_32  33
#define __va_inc_33  34
#define __va_inc_34  35
#define __va_inc_35  36
#define __va_inc_36  37
#define __va_inc_37  38
#define __va_inc_38  39
#define __va_inc_39  40
#define __va_inc_40  41
#define __va_inc_41  42
#define __va_inc_42  43
#define __va_inc_43  44
#define __va_inc_44  45
#define __va_inc_45  46
#define __va_inc_46  47
#define __va_inc_47  48
#define __va_inc_48  49
#define __va_inc_49  50
#define __va_inc_50  51
#define __va_inc_51  52
#define __va_inc_52  53
#define __va_inc_53  54
#define __va_inc_54  55
#define __va_inc_55  56
#define __va_inc_56  57
#define __va_inc_57  58
#define __va_inc_58  59
#define __va_inc_59  60
#define __va_inc_60  61
#define __va_inc_61  62
#define __va_inc_62  63
#define __va_inc_63  64
#define __va_inc_64  65
#define __va_inc_65  66
#define __va_inc_66  67
#define __va_inc_67  68
#define __va_inc_68  69
#define __va_inc_69  70
#define __va_inc_70  71
#define __va_inc_71  72
#define __va_inc_72  73
#define __va_inc_73  74
#define __va_inc_74  75
#define __va_inc_75  76
#define __va_inc_76  77
#define __va_inc_77  78
#define __va_inc_78  79
#define __va_inc_79  80
#define __va_inc_80  81
#define __va_inc_81  82
#define __va_inc_82  83
#define __va_inc_83  84
#define __va_inc_84  85
#define __va_inc_85  86
#define __va_inc_86  87
#define __va_inc_87  88
#define __va_inc_88  89
#define __va_inc_89  90
#define __va_inc_90  91
#define __va_inc_91  92
#define __va_inc_92  93
#define __va_inc_93  94
#define __va_inc_94  95
#define __va_inc_95  96
#define __va_inc_96  97
#define __va_inc_97  98
#define __va_inc_98  99
#define __va_inc_99  100
#define __va_inc_100 101
#define __va_inc_101 102
#define __va_inc_102 103
#define __va_inc_103 104
#define __va_inc_104 105
#define __va_inc_105 106
#define __va_inc_106 107
#define __va_inc_107 108
#define __va_inc_108 109
#define __va_inc_109 110
#define __va_inc_110 111
#define __va_inc_111 112
#define __va_inc_112 113
#define __va_inc_113 114
#define __va_inc_114 115
#define __va_inc_115 116
#define __va_inc_116 117
#define __va_inc_117 118
#define __va_inc_118 119
#define __va_inc_119 120
#define __va_inc_120 121
#define __va_inc_121 122
#define __va_inc_122 123
#define __va_inc_123 124
#define __va_inc_124 125
#define __va_inc_125 126
#define __va_inc_126 127
#define __va_inc_127 128
#define __va_inc_128 128

#ifdef va_debug
echo(inc)
va_inc(0)         /* : 1   */
va_inc(1)         /* : 2   */
va_inc(15)        /* : 16  */
va_inc(127)       /* : 128 */
va_inc(128)       /* : 128 */
#endif


/*
 * inc
 **/
#define va_dec(x)    va_cat(__va_dec_, x)
#define __va_dec_0   0
#define __va_dec_1   0
#define __va_dec_2   1
#define __va_dec_3   2
#define __va_dec_4   3
#define __va_dec_5   4
#define __va_dec_6   5
#define __va_dec_7   6
#define __va_dec_8   7
#define __va_dec_9   8
#define __va_dec_10  9
#define __va_dec_11  10
#define __va_dec_12  11
#define __va_dec_13  12
#define __va_dec_14  13
#define __va_dec_15  14
#define __va_dec_16  15
#define __va_dec_17  16
#define __va_dec_18  17
#define __va_dec_19  18
#define __va_dec_20  19
#define __va_dec_21  20
#define __va_dec_22  21
#define __va_dec_23  22
#define __va_dec_24  23
#define __va_dec_25  24
#define __va_dec_26  25
#define __va_dec_27  26
#define __va_dec_28  27
#define __va_dec_29  28
#define __va_dec_30  29
#define __va_dec_31  30
#define __va_dec_32  31
#define __va_dec_33  32
#define __va_dec_34  33
#define __va_dec_35  34
#define __va_dec_36  35
#define __va_dec_37  36
#define __va_dec_38  37
#define __va_dec_39  38
#define __va_dec_40  39
#define __va_dec_41  40
#define __va_dec_42  41
#define __va_dec_43  42
#define __va_dec_44  43
#define __va_dec_45  44
#define __va_dec_46  45
#define __va_dec_47  46
#define __va_dec_48  47
#define __va_dec_49  48
#define __va_dec_50  49
#define __va_dec_51  50
#define __va_dec_52  51
#define __va_dec_53  52
#define __va_dec_54  53
#define __va_dec_55  54
#define __va_dec_56  55
#define __va_dec_57  56
#define __va_dec_58  57
#define __va_dec_59  58
#define __va_dec_60  59
#define __va_dec_61  60
#define __va_dec_62  61
#define __va_dec_63  62
#define __va_dec_64  63
#define __va_dec_65  64
#define __va_dec_66  65
#define __va_dec_67  66
#define __va_dec_68  67
#define __va_dec_69  68
#define __va_dec_70  69
#define __va_dec_71  70
#define __va_dec_72  71
#define __va_dec_73  72
#define __va_dec_74  73
#define __va_dec_75  74
#define __va_dec_76  75
#define __va_dec_77  76
#define __va_dec_78  77
#define __va_dec_79  78
#define __va_dec_80  79
#define __va_dec_81  80
#define __va_dec_82  81
#define __va_dec_83  82
#define __va_dec_84  83
#define __va_dec_85  84
#define __va_dec_86  85
#define __va_dec_87  86
#define __va_dec_88  87
#define __va_dec_89  88
#define __va_dec_90  89
#define __va_dec_91  90
#define __va_dec_92  91
#define __va_dec_93  92
#define __va_dec_94  93
#define __va_dec_95  94
#define __va_dec_96  95
#define __va_dec_97  96
#define __va_dec_98  97
#define __va_dec_99  98
#define __va_dec_100 99
#define __va_dec_101 100
#define __va_dec_102 101
#define __va_dec_103 102
#define __va_dec_104 103
#define __va_dec_105 104
#define __va_dec_106 105
#define __va_dec_107 106
#define __va_dec_108 107
#define __va_dec_109 108
#define __va_dec_110 109
#define __va_dec_111 110
#define __va_dec_112 111
#define __va_dec_113 112
#define __va_dec_114 113
#define __va_dec_115 114
#define __va_dec_116 115
#define __va_dec_117 116
#define __va_dec_118 117
#define __va_dec_119 118
#define __va_dec_120 119
#define __va_dec_121 120
#define __va_dec_122 121
#define __va_dec_123 122
#define __va_dec_124 123
#define __va_dec_125 124
#define __va_dec_126 125
#define __va_dec_127 126
#define __va_dec_128 127

#ifdef va_debug
echo(inc)
va_dec(0)         /* : 0  */
va_dec(1)         /* : 0  */
va_dec(15)        /* : 14 */
va_dec(16)        /* : 15 */
#endif




/*
 * first
 **/
#define va_first(_0, ...) _0

#ifdef va_debug
echo(first)
va_first()              /* :   */
va_first(0)             /* : 0 */
va_first(0, 1, 2)       /* : 0 */
#endif


/*
 * has args
 **/
#define va_has(...)          va_first(__VA_OPT__(1, ) 0)
#define __va_has(first, ...) first

#ifdef va_debug
echo(has)
va_has()              /* : 0 */
va_has(0)             /* : 1 */
va_has(0, 1, 2)       /* : 1 */
#endif


/*
 * once args
 **/
#define va_once(...)            __va_once_ok(va_has(__VA_ARGS__), __va_eat_first(__VA_ARGS__))
#define __va_eat_first(_0, ...) va_first(__VA_OPT__(0, ) 1)
#define __va_once_ok(has, eat)  va_cat(__va_once_ok_, has, eat)
#define __va_once_ok_11         1
#define __va_once_ok_10         0
#define __va_once_ok_01         0
#define __va_once_ok_00         0

#ifdef va_debug
echo(once)
va_once()               /* : 0 */
va_once(0)              /* : 1 */
va_once(0, 1, 2)        /* : 0 */
#endif


/*
 * not
 **/
#define va_not(b)  va_cat(__va_not_, b)
#define __va_not_1 0
#define __va_not_0 1

#ifdef va_debug
echo(not)
va_not(1)       /* : 0 */
va_not(0)       /* : 1 */
#endif


/*
 * bool
 **/
#define va_bool(x)          __is_probe(__bool_##x)
#define __is_probe(...)     __second(__VA_ARGS__, 1)
#define __bool_0            __probe()
#define __probe()           ~, 0
#define __second(a, b, ...) b

#ifdef va_debug
echo(bool)
va_bool(0)        /* : 0 */
va_bool(1)        /* : 1 */
va_bool(15)       /* : 1 */
va_bool(16)       /* : 1 */
#endif


/*
 * if
 **/
#define va_if(condition) va_cat(__va_if_, condition)
#define __va_if_1(...)   __VA_ARGS__
#define __va_if_0(...)

#ifdef va_debug
echo(if)
va_if(0)(true)        /* :      */
va_if(1)(true)        /* : true */
#endif


/* 
 * elseif
 **/
#define va_elseif(condition)    va_cat(__va_elseif_, condition)
#define __va_elseif_1(...)      __VA_ARGS__ __va_elseif_1_else
#define __va_elseif_0(...)      __va_elseif_0_else
#define __va_elseif_1_else(...)
#define __va_elseif_0_else(...) __VA_ARGS__

#ifdef va_debug
echo(if_else)
va_elseif(0)(true)(false)       /* : false */
va_elseif(1)(true)(false)       /* : true  */
#endif


/*
 * cut
 **/
#define va_cut(num, ...)               va_eval(__va_cut(1, num, __VA_ARGS__))
#define __va_cut_ok_11                 1
#define __va_cut_ok_10                 0
#define __va_cut_ok_01                 0
#define __va_cut_ok_00                 0
#define __va_cut_ok(num, has)          va_cat(__va_cut_ok_, num, has)
#define __va_cut_def()                 __va_cut
#define __va_cut(first, num, arg, ...)                                                             \
  va_if(__va_cut_ok(va_bool(num), va_has(__VA_ARGS__))) (                                          \
    va_elseif(first) (                                                                             \
      arg                                                                                          \
    )(                                                                                             \
      , arg                                                                                        \
    )                                                                                              \
                                                                                                   \
    va_defer(2, __va_cut_def)()(va_dec(first), va_dec(num), __VA_ARGS__)                           \
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
va_cut(0, 0, 1, 2, 3, 4)        /* :                   */
va_cut(1, 0, 1, 2, 3, 4)        /* : 0                 */
va_cut(2, 0, 1, 2, 3, 4)        /* : 0 , 1             */
va_cut(3, 0, 1, 2, 3, 4)        /* : 0 , 1 , 2         */
va_cut(4, 0, 1, 2, 3, 4)        /* : 0 , 1 , 2 , 3     */
va_cut(5, 0, 1, 2, 3, 4)        /* : 0 , 1 , 2 , 3 , 4 */
va_cut(6, 0, 1, 2, 3, 4)        /* : 0 , 1 , 2 , 3 , 4 */
#endif


/*
 * slice
 **/
#define va_slice(num, ...) va_eval(__va_slice(1, num, __VA_ARGS__))
#define __va_slice_def()   __va_slice
#define __va_slice(first, num, arg, ...)                                                           \
  va_elseif(va_bool(num)) (                                                                        \
    va_defer(2, __va_slice_def)()(first, va_dec(num), __VA_ARGS__)                                 \
  )(                                                                                               \
    va_elseif(va_has(__VA_ARGS__)) (                                                               \
      va_elseif(first) (                                                                           \
        arg                                                                                        \
      )(                                                                                           \
        , arg                                                                                      \
      )                                                                                            \
                                                                                                   \
      va_defer(3, __va_slice_def)()(va_dec(first), va_dec(num), __VA_ARGS__)                       \
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
va_slice(0, 0, 1, 2, 3, 4)        /* : 0 , 1 , 2 , 3 , 4 */ 
va_slice(1, 0, 1, 2, 3, 4)        /* : 1 , 2 , 3 , 4     */
va_slice(2, 0, 1, 2, 3, 4)        /* : 2 , 3 , 4         */
va_slice(3, 0, 1, 2, 3, 4)        /* : 3 , 4             */
va_slice(4, 0, 1, 2, 3, 4)        /* : 4                 */
va_slice(5, 0, 1, 2, 3, 4)        /* :                   */
va_slice(6, 0, 1, 2, 3, 4)        /* :                   */
#endif


/*
 * at
 **/
#define va_at(i, ...) va_eval(va_defer(1, va_first)(va_slice(i, __VA_ARGS__)))

#ifdef va_debug
echo(at)
va_at(3)                      /* :   */
va_at(0, 0, 1, 2, 3, 4)       /* : 0 */
va_at(1, 0, 1, 2, 3, 4)       /* : 1 */
va_at(2, 0, 1, 2, 3, 4)       /* : 2 */
va_at(3, 0, 1, 2, 3, 4)       /* : 3 */
va_at(4, 0, 1, 2, 3, 4)       /* : 4 */
va_at(5, 0, 1, 2, 3, 4)       /* :   */
va_at(6, 0, 1, 2, 3, 4)       /* :   */
#endif

  
/*
 * opt
 **/
#define va_opt(i, ...) __va_opt(va_slice(i, __VA_ARGS__))
#define __va_opt(...)  __VA_OPT__(, )

#ifdef va_debug
echo(opt)
va_opt(3)                         /* :   */
va_opt(3, 0)                      /* :   */
va_opt(3, 0, 1)                   /* :   */
va_opt(3, 0, 1, 2)                /* :   */
va_opt(3, 0, 1, 2, 3)             /* : , */
va_opt(3, 0, 1, 2, 3, 4)          /* : , */
va_opt(3, 0, 1, 2, 3, 4, 5)       /* : , */
#endif


/*
 * th
 **/
#define va_th(i, def, ...) va_at(0, va_at(i, __VA_ARGS__) va_opt(i, __VA_ARGS__) def)
#define va_0th(def, ...)   va_th(0, def, __VA_ARGS__)
#define va_1th(def, ...)   va_th(1, def, __VA_ARGS__)
#define va_2th(def, ...)   va_th(2, def, __VA_ARGS__)

#ifdef va_debug
echo(th)
va_th(3, Y)                         /* : Y */
va_th(3, Y, 0)                      /* : Y */
va_th(3, Y, 0, 1)                   /* : Y */
va_th(3, Y, 0, 1, 2)                /* : Y */
va_th(3, Y, 0, 1, 2, 3)             /* : 3 */
va_th(3, Y, 0, 1, 2, 3, 4)          /* : 3 */
va_th(3, Y, 0, 1, 2, 3, 4, 5)       /* : 3 */
#endif


/*
 * size
 **/
#define va_size(...)    va_eval(__va_size(0, "ignore", __VA_ARGS__))
#define __va_size_def() __va_size
#define __va_size(first, arg,  ...)                                                                \
  va_elseif(va_has(__VA_ARGS__)) (                                                                 \
    va_defer(2, __va_size_def)()(va_inc(first), __VA_ARGS__)                                       \
  )(                                                                                               \
    first                                                                                          \
  )                                                                                             

#ifdef va_debug
echo(size)
va_size()                     /* : 0 */
va_size(1)                    /* : 1 */
va_size(1, 2)                 /* : 2 */
va_size(1, 2, 3)              /* : 3 */
va_size(1, 2, 3, 4)           /* : 4 */
va_size(1, 2, 3, 4, 5)        /* : 5 */
#endif


/*
 * list
 **/
#define va_list(i, ...) va_opt(i, __VA_ARGS__) va_slice(i, __VA_ARGS__)

#ifdef va_debug
echo(list)
va_list(3, 1)                   /* :         */
va_list(3, 1, 2)                /* :         */
va_list(3, 1, 2, 3)             /* :         */
va_list(3, 1, 2, 3, 4)          /* : , 4     */
va_list(3, 1, 2, 3, 4, 5)       /* : , 4 , 5 */
#endif


/*
 * last cut
 * */
#define va_lcut(...) va_cut(va_dec(va_size(__VA_ARGS__)), __VA_ARGS__)

#ifdef va_debug
echo(lcut)
va_lcut()                     /* :               */
va_lcut(1)                    /* :               */
va_lcut(1, 2)                 /* : 1             */
va_lcut(1, 2, 3)              /* : 1 , 2         */
va_lcut(1, 2, 3, 4)           /* : 1 , 2 , 3     */
va_lcut(1, 2, 3, 4, 5)        /* : 1 , 2 , 3 , 4 */
#endif


/*
 * last
 * */
#define va_last(...) va_at(va_dec(va_size(__VA_ARGS__)), __VA_ARGS__)

#ifdef va_debug
echo(last)
va_last()                     /* :   */
va_last(1)                    /* : 1 */
va_last(1, 2)                 /* : 2 */
va_last(1, 2, 3)              /* : 3 */
va_last(1, 2, 3, 4)           /* : 4 */
va_last(1, 2, 3, 4, 5)        /* : 5 */
#endif


/*
 * mapof
 **/
#define va_map(macro, ...) va_if(__va_map_ok(va_has(__VA_ARGS__), va_once(__VA_ARGS__))) (         \
    va_eval(va_defer(1, __va_map)(macro, va_lcut(__VA_ARGS__)))                                    \
  )                                                                                                \
                                                                                                   \
  macro(va_last(__VA_ARGS__))
#define __va_map_ok_11         0
#define __va_map_ok_10         1
#define __va_map_ok_01         0
#define __va_map_ok_00         0
#define __va_map_ok(has, once) va_cat(__va_map_ok_, has, once)
#define __va_map_def()         __va_map
#define __va_map(macro, first, ...)                                                                \
  macro(first),                                                                                    \
  va_if(va_has(__VA_ARGS__)) (                                                                     \
    va_defer(2, __va_map_def)()(macro, __VA_ARGS__)                                                \
  )

#ifdef va_debug
echo(map)
#  define _a(n) (n)
va_map(_a, 1)                   /* : (1)                     */
va_map(_a, 1, 2)                /* : (1), (2)                */
va_map(_a, 1, 2, 3)             /* : (1), (2), (3)           */
va_map(_a, 1, 2, 3, 4)          /* : (1), (2), (3), (4)      */
va_map(_a, 1, 2, 3, 4, 5)       /* : (1), (2), (3), (4), (5) */
#endif


/*
 * mapof
 * */
#define va_mapof(macro, ...) va_eval(__va_mapof(macro, __VA_ARGS__))
#define __va_mapof_def()     __va_mapof
#define __va_mapof(macro, first, ...)                                                              \
  macro(first)                                                                                     \
  va_if(va_has(__VA_ARGS__)) (                                                                     \
    va_defer(2, __va_mapof_def)()(macro, __VA_ARGS__)                                              \
  )

#ifdef va_debug
echo(mapof)
#  define _b(n) (n),
va_mapof(_b, 1)                 /* : (1),                */
va_mapof(_b, 1, 2)              /* : (1), (2),           */
va_mapof(_b, 1, 2, 3)           /* : (1), (2), (3),      */
va_mapof(_b, 1, 2, 3, 4)        /* : (1), (2), (3), (4), */
#endif
