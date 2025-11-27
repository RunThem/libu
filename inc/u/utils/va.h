/* MIT License
 *
 * Copyright (c) 2023 RunThem <iccy.fun@outlook.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *
 * doc:
 *    [](https://github.com/notfoundry/ppstep)
 *    [](https://github.com/swansontec/map-macro)
 *    [](http://jhnet.co.uk/articles/cpp_magic)
 *    [](https://disenone.github.io/2021/03/31/cpp-preprocess)
 *    [](https://github.com/pfultz2/Cloak/wiki/C-Preprocessor-tricks,-tips,-and-idioms#deferred-expression)
 *
 * run:
 *    gcc/clang -P -E -Du_va_debug -DLIBU_VA_EVAL va.h
 *
 * */

/* clang-format off */

#pragma once

#ifndef U_VA_H__
#define U_VA_H__

#define u_va_for_let(type, var, val)                                                               \
  for (type var = val, *____##var = &var; ____##var; ____##var = nullptr)

#define u_va_let(type, name, def) type u_va_cat($__, __line__, name) = def
#define u_va_var(name)            u_va_cat($__, __line__, name)

#ifndef LIBU_VA_EVAL

#define u_va_first(first, ...) first

#define u_va_cat(x, y, ...)   __u_va_cat(x, y, __VA_ARGS__)
#define __u_va_cat(x, y, ...) x##y##__VA_ARGS__

#define u_va_at(N, ...)                                                                            \
  u_va_cat(__u_va_at, N)(__VA_ARGS__, , , , , , , , )
#define __u_va_at10(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, ...) u_va_first(__VA_ARGS__)
#define __u_va_at9(_1, _2, _3, _4, _5, _6, _7, _8, _9, ...)       u_va_first(__VA_ARGS__)
#define __u_va_at8(_1, _2, _3, _4, _5, _6, _7, _8, ...)           u_va_first(__VA_ARGS__)
#define __u_va_at7(_1, _2, _3, _4, _5, _6, _7, ...)               u_va_first(__VA_ARGS__)
#define __u_va_at6(_1, _2, _3, _4, _5, _6, ...)                   u_va_first(__VA_ARGS__)
#define __u_va_at5(_1, _2, _3, _4, _5, ...)                       u_va_first(__VA_ARGS__)
#define __u_va_at4(_1, _2, _3, _4, ...)                           u_va_first(__VA_ARGS__)
#define __u_va_at3(_1, _2, _3, ...)                               u_va_first(__VA_ARGS__)
#define __u_va_at2(_1, _2, ...)                                   u_va_first(__VA_ARGS__)
#define __u_va_at1(_1, ...)                                       u_va_first(__VA_ARGS__)
#define __u_va_at0(...)                                           u_va_first(__VA_ARGS__)

#define u_va_has(...)        u_va_first(__VA_OPT__(1, ) 0)
#define u_va_eat(first, ...) __VA_ARGS__

#define u_va_if(condition) u_va_cat(__u_va_if_, condition)
#define __u_va_if_1(...)   __VA_ARGS__
#define __u_va_if_0(...)

#define u_va_has_if(...) u_va_if(u_va_has(__VA_ARGS__))

#define u_va_elseif(condition) u_va_cat(__u_va_elseif_, condition)
#define __u_va_elseif_1(...)   __VA_ARGS__ __u_va_elseif_1_else
#define __u_va_elseif_0(...)   __u_va_elseif_0_else
#define __u_va_elseif_1_else(...)
#define __u_va_elseif_0_else(...) __VA_ARGS__

#define u_va_cnt(...)                                                                              \
        u_va_elseif(u_va_has(__VA_ARGS__)) (                                                       \
          u_va_at(10, __VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)                                  \
        ) (                                                                                        \
          0                                                                                        \
        )

#define u_va_cnt_is(N, ...) u_va_at(N, u_va_cat(__u_va_cnt_is, u_va_cnt(__VA_ARGS__)))
#define __u_va_cnt_is0      1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
#define __u_va_cnt_is1      0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0
#define __u_va_cnt_is2      0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
#define __u_va_cnt_is3      0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0
#define __u_va_cnt_is4      0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0
#define __u_va_cnt_is5      0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0
#define __u_va_cnt_is6      0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0
#define __u_va_cnt_is7      0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0
#define __u_va_cnt_is8      0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0
#define __u_va_cnt_is9      0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0
#define __u_va_cnt_is10     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1

#define u_va_check(N, ...) u_va_at(N, u_va_cat(__u_va_check, u_va_cnt(__VA_ARGS__)))
#define __u_va_check0      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
#define __u_va_check1      1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
#define __u_va_check2      1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0
#define __u_va_check3      1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0
#define __u_va_check4      1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0
#define __u_va_check5      1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0
#define __u_va_check6      1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0
#define __u_va_check7      1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0
#define __u_va_check8      1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0
#define __u_va_check9      1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0
#define __u_va_check10     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0

#define u_va_list(N, ...)                                                                          \
        u_va_if(u_va_check(N, __VA_ARGS__)) (                                                      \
          , u_va_cat(__u_va_list, N)(__VA_ARGS__)                                                  \
        )
#define __u_va_list10(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, ...) __VA_ARGS__
#define __u_va_list9(_1, _2, _3, _4, _5, _6, _7, _8, _9, ...)       __VA_ARGS__
#define __u_va_list8(_1, _2, _3, _4, _5, _6, _7, _8, ...)           __VA_ARGS__
#define __u_va_list7(_1, _2, _3, _4, _5, _6, _7, ...)               __VA_ARGS__
#define __u_va_list6(_1, _2, _3, _4, _5, _6, ...)                   __VA_ARGS__
#define __u_va_list5(_1, _2, _3, _4, _5, ...)                       __VA_ARGS__
#define __u_va_list4(_1, _2, _3, _4, ...)                           __VA_ARGS__
#define __u_va_list3(_1, _2, _3, ...)                               __VA_ARGS__
#define __u_va_list2(_1, _2, ...)                                   __VA_ARGS__
#define __u_va_list1(_1, ...)                                       __VA_ARGS__
#define __u_va_list0(...)                                           __VA_ARGS__

#define u_va_0th(def, ...) u_va_th(0, def, __VA_ARGS__)
#define u_va_1th(def, ...) u_va_th(1, def, __VA_ARGS__)
#define u_va_2th(def, ...) u_va_th(2, def, __VA_ARGS__)
#define u_va_th(N, def, ...)                                                                       \
        u_va_elseif(u_va_check(N, __VA_ARGS__)) (                                                  \
          u_va_at(N, __VA_ARGS__)                                                                  \
        ) (                                                                                        \
          def                                                                                      \
        )                                                                                          \

#else /* !LIBU_VA_EVAL */

#ifdef u_va_debug
#define echo(arg) <><><><><><><><><><><><><><><><><><><><><><><> u_va_##arg
#endif

/*
 * cat
 **/
#define u_va_cat(x, y, ...) x##y##__VA_ARGS__

#ifdef u_va_debug
echo(cat)
u_va_cat(hello_, world)             /* : hello_world */
u_va_cat(hello_, the_, world)       /* : hello_the_world */
#endif


/*
 * eval 2^8 128
 **/
#define u_va_eval(...)     __u_va_eval_3(__VA_ARGS__)
#define __u_va_eval_1(...) __VA_ARGS__
#define __u_va_eval_2(...) __u_va_eval_1(__u_va_eval_1(__VA_ARGS__))
#define __u_va_eval_3(...) __u_va_eval_2(__u_va_eval_2(__VA_ARGS__))
#define __u_va_eval_4(...) __u_va_eval_3(__u_va_eval_3(__VA_ARGS__))
#define __u_va_eval_5(...) __u_va_eval_4(__u_va_eval_4(__VA_ARGS__))
#define __u_va_eval_6(...) __u_va_eval_5(__u_va_eval_5(__VA_ARGS__))
#define __u_va_eval_7(...) __u_va_eval_6(__u_va_eval_6(__VA_ARGS__))
#define __u_va_eval_8(...) __u_va_eval_7(__u_va_eval_7(__VA_ARGS__))


/*
 * defer call
 **/
#define u_va_defer(n, m) u_va_cat(__u_va_defer_, n)(m)
#define space()
#define __u_va_defer_1(macro) macro space()
#define __u_va_defer_2(macro) macro space space()()
#define __u_va_defer_3(macro) macro space space space()()()
#define __u_va_defer_4(macro) macro space space space space()()()()
#define __u_va_defer_5(macro) macro space space space space space()()()()()
#define __u_va_defer_6(macro) macro space space space space space space()()()()()()
#define __u_va_defer_7(macro) macro space space space space space space space()()()()()()()
#define __u_va_defer_8(macro) macro space space space space space space space space()()()()()()()()


/*
 * let
 **/
#define u_va_let(name) u_va_eval(u_va_defer(1, u_va_cat)(_##name##_, __LINE__))

#ifdef u_va_debug
echo(let)
#line 100
u_va_let(a)       /* : _a_100 */
#endif


/*
 * unpack
 * */
#define u_va_unpack(...)   __u_va_unpack __VA_ARGS__
#define __u_va_unpack(...) __VA_ARGS__


/*
 * inc
 **/
#define u_va_inc(x)    u_va_cat(__u_va_inc_, x)
#define __u_va_inc_0   1
#define __u_va_inc_1   2
#define __u_va_inc_2   3
#define __u_va_inc_3   4
#define __u_va_inc_4   5
#define __u_va_inc_5   6
#define __u_va_inc_6   7
#define __u_va_inc_7   8
#define __u_va_inc_8   9
#define __u_va_inc_9   10
#define __u_va_inc_10  11
#define __u_va_inc_11  12
#define __u_va_inc_12  13
#define __u_va_inc_13  14
#define __u_va_inc_14  15
#define __u_va_inc_15  16
#define __u_va_inc_16  17
#define __u_va_inc_17  18
#define __u_va_inc_18  19
#define __u_va_inc_19  20
#define __u_va_inc_20  21
#define __u_va_inc_21  22
#define __u_va_inc_22  23
#define __u_va_inc_23  24
#define __u_va_inc_24  25
#define __u_va_inc_25  26
#define __u_va_inc_26  27
#define __u_va_inc_27  28
#define __u_va_inc_28  29
#define __u_va_inc_29  30
#define __u_va_inc_30  31
#define __u_va_inc_31  32
#define __u_va_inc_32  33
#define __u_va_inc_33  34
#define __u_va_inc_34  35
#define __u_va_inc_35  36
#define __u_va_inc_36  37
#define __u_va_inc_37  38
#define __u_va_inc_38  39
#define __u_va_inc_39  40
#define __u_va_inc_40  41
#define __u_va_inc_41  42
#define __u_va_inc_42  43
#define __u_va_inc_43  44
#define __u_va_inc_44  45
#define __u_va_inc_45  46
#define __u_va_inc_46  47
#define __u_va_inc_47  48
#define __u_va_inc_48  49
#define __u_va_inc_49  50
#define __u_va_inc_50  51
#define __u_va_inc_51  52
#define __u_va_inc_52  53
#define __u_va_inc_53  54
#define __u_va_inc_54  55
#define __u_va_inc_55  56
#define __u_va_inc_56  57
#define __u_va_inc_57  58
#define __u_va_inc_58  59
#define __u_va_inc_59  60
#define __u_va_inc_60  61
#define __u_va_inc_61  62
#define __u_va_inc_62  63
#define __u_va_inc_63  64
#define __u_va_inc_64  65
#define __u_va_inc_65  66
#define __u_va_inc_66  67
#define __u_va_inc_67  68
#define __u_va_inc_68  69
#define __u_va_inc_69  70
#define __u_va_inc_70  71
#define __u_va_inc_71  72
#define __u_va_inc_72  73
#define __u_va_inc_73  74
#define __u_va_inc_74  75
#define __u_va_inc_75  76
#define __u_va_inc_76  77
#define __u_va_inc_77  78
#define __u_va_inc_78  79
#define __u_va_inc_79  80
#define __u_va_inc_80  81
#define __u_va_inc_81  82
#define __u_va_inc_82  83
#define __u_va_inc_83  84
#define __u_va_inc_84  85
#define __u_va_inc_85  86
#define __u_va_inc_86  87
#define __u_va_inc_87  88
#define __u_va_inc_88  89
#define __u_va_inc_89  90
#define __u_va_inc_90  91
#define __u_va_inc_91  92
#define __u_va_inc_92  93
#define __u_va_inc_93  94
#define __u_va_inc_94  95
#define __u_va_inc_95  96
#define __u_va_inc_96  97
#define __u_va_inc_97  98
#define __u_va_inc_98  99
#define __u_va_inc_99  100
#define __u_va_inc_100 101
#define __u_va_inc_101 102
#define __u_va_inc_102 103
#define __u_va_inc_103 104
#define __u_va_inc_104 105
#define __u_va_inc_105 106
#define __u_va_inc_106 107
#define __u_va_inc_107 108
#define __u_va_inc_108 109
#define __u_va_inc_109 110
#define __u_va_inc_110 111
#define __u_va_inc_111 112
#define __u_va_inc_112 113
#define __u_va_inc_113 114
#define __u_va_inc_114 115
#define __u_va_inc_115 116
#define __u_va_inc_116 117
#define __u_va_inc_117 118
#define __u_va_inc_118 119
#define __u_va_inc_119 120
#define __u_va_inc_120 121
#define __u_va_inc_121 122
#define __u_va_inc_122 123
#define __u_va_inc_123 124
#define __u_va_inc_124 125
#define __u_va_inc_125 126
#define __u_va_inc_126 127
#define __u_va_inc_127 128
#define __u_va_inc_128 128

#ifdef u_va_debug
echo(inc)
u_va_inc(0)         /* : 1   */
u_va_inc(1)         /* : 2   */
u_va_inc(15)        /* : 16  */
u_va_inc(127)       /* : 128 */
u_va_inc(128)       /* : 128 */
#endif


/*
 * inc
 **/
#define u_va_dec(x)    u_va_cat(__u_va_dec_, x)
#define __u_va_dec_0   0
#define __u_va_dec_1   0
#define __u_va_dec_2   1
#define __u_va_dec_3   2
#define __u_va_dec_4   3
#define __u_va_dec_5   4
#define __u_va_dec_6   5
#define __u_va_dec_7   6
#define __u_va_dec_8   7
#define __u_va_dec_9   8
#define __u_va_dec_10  9
#define __u_va_dec_11  10
#define __u_va_dec_12  11
#define __u_va_dec_13  12
#define __u_va_dec_14  13
#define __u_va_dec_15  14
#define __u_va_dec_16  15
#define __u_va_dec_17  16
#define __u_va_dec_18  17
#define __u_va_dec_19  18
#define __u_va_dec_20  19
#define __u_va_dec_21  20
#define __u_va_dec_22  21
#define __u_va_dec_23  22
#define __u_va_dec_24  23
#define __u_va_dec_25  24
#define __u_va_dec_26  25
#define __u_va_dec_27  26
#define __u_va_dec_28  27
#define __u_va_dec_29  28
#define __u_va_dec_30  29
#define __u_va_dec_31  30
#define __u_va_dec_32  31
#define __u_va_dec_33  32
#define __u_va_dec_34  33
#define __u_va_dec_35  34
#define __u_va_dec_36  35
#define __u_va_dec_37  36
#define __u_va_dec_38  37
#define __u_va_dec_39  38
#define __u_va_dec_40  39
#define __u_va_dec_41  40
#define __u_va_dec_42  41
#define __u_va_dec_43  42
#define __u_va_dec_44  43
#define __u_va_dec_45  44
#define __u_va_dec_46  45
#define __u_va_dec_47  46
#define __u_va_dec_48  47
#define __u_va_dec_49  48
#define __u_va_dec_50  49
#define __u_va_dec_51  50
#define __u_va_dec_52  51
#define __u_va_dec_53  52
#define __u_va_dec_54  53
#define __u_va_dec_55  54
#define __u_va_dec_56  55
#define __u_va_dec_57  56
#define __u_va_dec_58  57
#define __u_va_dec_59  58
#define __u_va_dec_60  59
#define __u_va_dec_61  60
#define __u_va_dec_62  61
#define __u_va_dec_63  62
#define __u_va_dec_64  63
#define __u_va_dec_65  64
#define __u_va_dec_66  65
#define __u_va_dec_67  66
#define __u_va_dec_68  67
#define __u_va_dec_69  68
#define __u_va_dec_70  69
#define __u_va_dec_71  70
#define __u_va_dec_72  71
#define __u_va_dec_73  72
#define __u_va_dec_74  73
#define __u_va_dec_75  74
#define __u_va_dec_76  75
#define __u_va_dec_77  76
#define __u_va_dec_78  77
#define __u_va_dec_79  78
#define __u_va_dec_80  79
#define __u_va_dec_81  80
#define __u_va_dec_82  81
#define __u_va_dec_83  82
#define __u_va_dec_84  83
#define __u_va_dec_85  84
#define __u_va_dec_86  85
#define __u_va_dec_87  86
#define __u_va_dec_88  87
#define __u_va_dec_89  88
#define __u_va_dec_90  89
#define __u_va_dec_91  90
#define __u_va_dec_92  91
#define __u_va_dec_93  92
#define __u_va_dec_94  93
#define __u_va_dec_95  94
#define __u_va_dec_96  95
#define __u_va_dec_97  96
#define __u_va_dec_98  97
#define __u_va_dec_99  98
#define __u_va_dec_100 99
#define __u_va_dec_101 100
#define __u_va_dec_102 101
#define __u_va_dec_103 102
#define __u_va_dec_104 103
#define __u_va_dec_105 104
#define __u_va_dec_106 105
#define __u_va_dec_107 106
#define __u_va_dec_108 107
#define __u_va_dec_109 108
#define __u_va_dec_110 109
#define __u_va_dec_111 110
#define __u_va_dec_112 111
#define __u_va_dec_113 112
#define __u_va_dec_114 113
#define __u_va_dec_115 114
#define __u_va_dec_116 115
#define __u_va_dec_117 116
#define __u_va_dec_118 117
#define __u_va_dec_119 118
#define __u_va_dec_120 119
#define __u_va_dec_121 120
#define __u_va_dec_122 121
#define __u_va_dec_123 122
#define __u_va_dec_124 123
#define __u_va_dec_125 124
#define __u_va_dec_126 125
#define __u_va_dec_127 126
#define __u_va_dec_128 127

#ifdef u_va_debug
echo(inc)
u_va_dec(0)         /* : 0  */
u_va_dec(1)         /* : 0  */
u_va_dec(15)        /* : 14 */
u_va_dec(16)        /* : 15 */
#endif


/*
 * first
 **/
#define u_va_first(_0, ...) _0

#ifdef u_va_debug
echo(first)
u_va_first()              /* :   */
u_va_first(0)             /* : 0 */
u_va_first(0, 1, 2)       /* : 0 */
#endif


/*
 * has args
 **/
#define u_va_has(...)          u_va_first(__VA_OPT__(1, ) 0)
#define __u_va_has(first, ...) first

#ifdef u_va_debug
echo(has)
u_va_has()              /* : 0 */
u_va_has(0)             /* : 1 */
u_va_has(0, 1, 2)       /* : 1 */
#endif


/*
 * once args
 **/
#define u_va_once(...)                                                                             \
  __u_va_once_ok(u_va_has(__VA_ARGS__), __u_va_eat_first(__VA_ARGS__))
#define __u_va_eat_first(_0, ...) u_va_first(__VA_OPT__(0, ) 1)
#define __u_va_once_ok(has, eat)  u_va_cat(__u_va_once_ok_, has, eat)
#define __u_va_once_ok_11         1
#define __u_va_once_ok_10         0
#define __u_va_once_ok_01         0
#define __u_va_once_ok_00         0

#ifdef u_va_debug
echo(once)
u_va_once()               /* : 0 */
u_va_once(0)              /* : 1 */
u_va_once(0, 1, 2)        /* : 0 */
#endif


/*
 * not
 **/
#define u_va_not(b)  u_va_cat(__u_va_not_, b)
#define __u_va_not_1 0
#define __u_va_not_0 1

#ifdef u_va_debug
echo(not)
u_va_not(1)       /* : 0 */
u_va_not(0)       /* : 1 */
#endif


/*
 * bool
 **/
#define u_va_bool(x)          __is_probe(__bool_##x)
#define __is_probe(...)     __second(__VA_ARGS__, 1)
#define __bool_0            __probe()
#define __probe()           ~, 0
#define __second(a, b, ...) b

#ifdef u_va_debug
echo(bool)
u_va_bool(0)        /* : 0 */
u_va_bool(1)        /* : 1 */
u_va_bool(15)       /* : 1 */
u_va_bool(16)       /* : 1 */
#endif


/*
 * if
 **/
#define u_va_if(condition) u_va_cat(__u_va_if_, condition)
#define __u_va_if_1(...)   __VA_ARGS__
#define __u_va_if_0(...)

#ifdef u_va_debug
echo(if)
u_va_if(0)(true)        /* :      */
u_va_if(1)(true)        /* : true */
#endif


/*
 * elseif
 **/
#define u_va_elseif(condition)    u_va_cat(__u_va_elseif_, condition)
#define __u_va_elseif_1(...)      __VA_ARGS__ __u_va_elseif_1_else
#define __u_va_elseif_0(...)      __u_va_elseif_0_else
#define __u_va_elseif_1_else(...)
#define __u_va_elseif_0_else(...) __VA_ARGS__

#ifdef u_va_debug
echo(if_else)
u_va_elseif(0)(true)(false)       /* : false */
u_va_elseif(1)(true)(false)       /* : true  */
#endif


/*
 * cut
 **/
#define u_va_cut(num, ...)               u_va_eval(__u_va_cut(1, num, __VA_ARGS__))
#define __u_va_cut_ok_11                 1
#define __u_va_cut_ok_10                 0
#define __u_va_cut_ok_01                 0
#define __u_va_cut_ok_00                 0
#define __u_va_cut_ok(num, has)          u_va_cat(__u_va_cut_ok_, num, has)
#define __u_va_cut_def()                 __u_va_cut
#define __u_va_cut(first, num, arg, ...)                                                           \
  u_va_if(__u_va_cut_ok(u_va_bool(num), u_va_has(__VA_ARGS__))) (                                  \
    u_va_elseif(first) (                                                                           \
      arg                                                                                          \
    )(                                                                                             \
      , arg                                                                                        \
    )                                                                                              \
                                                                                                   \
    u_va_defer(2, __u_va_cut_def)()(u_va_dec(first), u_va_dec(num), __VA_ARGS__)                   \
  )                                                                                                \
                                                                                                   \
  u_va_elseif(u_va_has(__VA_ARGS__)) (                                                             \
  )(                                                                                               \
    u_va_if(u_va_bool(num)) (                                                                      \
      , arg                                                                                        \
    )                                                                                              \
  )

#ifdef u_va_debug
echo(cut)
u_va_cut(0, 0, 1, 2, 3, 4)        /* :                   */
u_va_cut(1, 0, 1, 2, 3, 4)        /* : 0                 */
u_va_cut(2, 0, 1, 2, 3, 4)        /* : 0 , 1             */
u_va_cut(3, 0, 1, 2, 3, 4)        /* : 0 , 1 , 2         */
u_va_cut(4, 0, 1, 2, 3, 4)        /* : 0 , 1 , 2 , 3     */
u_va_cut(5, 0, 1, 2, 3, 4)        /* : 0 , 1 , 2 , 3 , 4 */
u_va_cut(6, 0, 1, 2, 3, 4)        /* : 0 , 1 , 2 , 3 , 4 */
#endif


/*
 * slice
 **/
#define u_va_slice(num, ...) u_va_eval(__u_va_slice(1, num, __VA_ARGS__))
#define __u_va_slice_def()   __u_va_slice
#define __u_va_slice(first, num, arg, ...)                                                         \
  u_va_elseif(u_va_bool(num)) (                                                                    \
    u_va_defer(2, __u_va_slice_def)()(first, u_va_dec(num), __VA_ARGS__)                           \
  )(                                                                                               \
    u_va_elseif(u_va_has(__VA_ARGS__)) (                                                           \
      u_va_elseif(first) (                                                                         \
        arg                                                                                        \
      )(                                                                                           \
        , arg                                                                                      \
      )                                                                                            \
                                                                                                   \
      u_va_defer(3, __u_va_slice_def)()(u_va_dec(first), u_va_dec(num), __VA_ARGS__)               \
    )(                                                                                             \
      u_va_elseif(first) (                                                                         \
        arg                                                                                        \
      )(                                                                                           \
        , arg                                                                                      \
      )                                                                                            \
    )                                                                                              \
  )

#ifdef u_va_debug
echo(slice)
u_va_slice(0, 0, 1, 2, 3, 4)        /* : 0 , 1 , 2 , 3 , 4 */
u_va_slice(1, 0, 1, 2, 3, 4)        /* : 1 , 2 , 3 , 4     */
u_va_slice(2, 0, 1, 2, 3, 4)        /* : 2 , 3 , 4         */
u_va_slice(3, 0, 1, 2, 3, 4)        /* : 3 , 4             */
u_va_slice(4, 0, 1, 2, 3, 4)        /* : 4                 */
u_va_slice(5, 0, 1, 2, 3, 4)        /* :                   */
u_va_slice(6, 0, 1, 2, 3, 4)        /* :                   */
#endif


/*
 * at
 **/
#define u_va_at(i, ...) u_va_eval(u_va_defer(1, u_va_first)(u_va_slice(i, __VA_ARGS__)))

#ifdef u_va_debug
echo(at)
u_va_at(3)                      /* :   */
u_va_at(0, 0, 1, 2, 3, 4)       /* : 0 */
u_va_at(1, 0, 1, 2, 3, 4)       /* : 1 */
u_va_at(2, 0, 1, 2, 3, 4)       /* : 2 */
u_va_at(3, 0, 1, 2, 3, 4)       /* : 3 */
u_va_at(4, 0, 1, 2, 3, 4)       /* : 4 */
u_va_at(5, 0, 1, 2, 3, 4)       /* :   */
u_va_at(6, 0, 1, 2, 3, 4)       /* :   */
#endif


/*
 * opt
 **/
#define u_va_opt(i, ...) __u_va_opt(u_va_slice(i, __VA_ARGS__))
#define __u_va_opt(...)  __VA_OPT__(, )

#ifdef u_va_debug
echo(opt)
u_va_opt(3)                         /* :   */
u_va_opt(3, 0)                      /* :   */
u_va_opt(3, 0, 1)                   /* :   */
u_va_opt(3, 0, 1, 2)                /* :   */
u_va_opt(3, 0, 1, 2, 3)             /* : , */
u_va_opt(3, 0, 1, 2, 3, 4)          /* : , */
u_va_opt(3, 0, 1, 2, 3, 4, 5)       /* : , */
#endif


/*
 * th
 **/
#define u_va_th(i, def, ...) u_va_at(0, u_va_at(i, __VA_ARGS__) u_va_opt(i, __VA_ARGS__) def)
#define u_va_0th(def, ...)   u_va_th(0, def, __VA_ARGS__)
#define u_va_1th(def, ...)   u_va_th(1, def, __VA_ARGS__)
#define u_va_2th(def, ...)   u_va_th(2, def, __VA_ARGS__)

#ifdef u_va_debug
echo(th)
u_va_th(3, Y)                         /* : Y */
u_va_th(3, Y, 0)                      /* : Y */
u_va_th(3, Y, 0, 1)                   /* : Y */
u_va_th(3, Y, 0, 1, 2)                /* : Y */
u_va_th(3, Y, 0, 1, 2, 3)             /* : 3 */
u_va_th(3, Y, 0, 1, 2, 3, 4)          /* : 3 */
u_va_th(3, Y, 0, 1, 2, 3, 4, 5)       /* : 3 */
#endif


/*
 * cnt
 **/
#define u_va_cnt(...)    u_va_eval(__u_va_cnt(0, "ignore", __VA_ARGS__))
#define __u_va_cnt_def() __u_va_cnt
#define __u_va_cnt(first, arg,  ...)                                                               \
  u_va_elseif(u_va_has(__VA_ARGS__)) (                                                             \
    u_va_defer(2, __u_va_cnt_def)()(u_va_inc(first), __VA_ARGS__)                                  \
  )(                                                                                               \
    first                                                                                          \
  )

#ifdef u_va_debug
echo(cnt)
u_va_cnt()                     /* : 0 */
u_va_cnt(1)                    /* : 1 */
u_va_cnt(1, 2)                 /* : 2 */
u_va_cnt(1, 2, 3)              /* : 3 */
u_va_cnt(1, 2, 3, 4)           /* : 4 */
u_va_cnt(1, 2, 3, 4, 5)        /* : 5 */
#endif


/*
 * cnt_is
 **/
#define u_va_cnt_is(n, ...)                                                                        \
  u_va_eval(u_va_defer(1, u_va_first)(__u_va_cnt_is(u_va_at(n, _, __VA_ARGS__)) 0))
#define __u_va_cnt_is(...)  __VA_OPT__(1, )

#ifdef u_va_debug
echo(cnt_is)
u_va_cnt_is(2)                    /* : 0 */
u_va_cnt_is(2, 2)                 /* : 0 */
u_va_cnt_is(2, 2, 3)              /* : 1 */
u_va_cnt_is(2, 2, 3, 4)           /* : 1 */
u_va_cnt_is(2, 2, 3, 4, 5)        /* : 1 */
#endif


/*
 * list
 **/
#define u_va_list(i, ...) u_va_opt(i, __VA_ARGS__) u_va_slice(i, __VA_ARGS__)

#ifdef u_va_debug
echo(list)
u_va_list(3, 1)                   /* :         */
u_va_list(3, 1, 2)                /* :         */
u_va_list(3, 1, 2, 3)             /* :         */
u_va_list(3, 1, 2, 3, 4)          /* : , 4     */
u_va_list(3, 1, 2, 3, 4, 5)       /* : , 4 , 5 */
#endif


/*
 * last cut
 * */
#define u_va_lcut(...) u_va_cut(u_va_dec(u_va_cnt(__VA_ARGS__)), __VA_ARGS__)

#ifdef u_va_debug
echo(lcut)
u_va_lcut()                     /* :               */
u_va_lcut(1)                    /* :               */
u_va_lcut(1, 2)                 /* : 1             */
u_va_lcut(1, 2, 3)              /* : 1 , 2         */
u_va_lcut(1, 2, 3, 4)           /* : 1 , 2 , 3     */
u_va_lcut(1, 2, 3, 4, 5)        /* : 1 , 2 , 3 , 4 */
#endif


/*
 * last
 * */
#define u_va_last(...) u_va_at(u_va_dec(u_va_cnt(__VA_ARGS__)), __VA_ARGS__)

#ifdef u_va_debug
echo(last)
u_va_last()                     /* :   */
u_va_last(1)                    /* : 1 */
u_va_last(1, 2)                 /* : 2 */
u_va_last(1, 2, 3)              /* : 3 */
u_va_last(1, 2, 3, 4)           /* : 4 */
u_va_last(1, 2, 3, 4, 5)        /* : 5 */
#endif


/*
 * mapof
 **/
#define u_va_map(macro, ...)                                                                       \
  u_va_if(__u_va_map_ok(u_va_has(__VA_ARGS__), u_va_once(__VA_ARGS__))) (                          \
    u_va_eval(u_va_defer(1, __u_va_map)(macro, u_va_lcut(__VA_ARGS__)))                            \
  )                                                                                                \
                                                                                                   \
  u_va_if(u_va_has(__VA_ARGS__)) (                                                                 \
    macro(u_va_last(__VA_ARGS__))                                                                  \
  )
#define __u_va_map_ok_11         0
#define __u_va_map_ok_10         1
#define __u_va_map_ok_01         0
#define __u_va_map_ok_00         0
#define __u_va_map_ok(has, once) u_va_cat(__u_va_map_ok_, has, once)
#define __u_va_map_def()         __u_va_map
#define __u_va_map(macro, first, ...)                                                              \
  macro(first),                                                                                    \
  u_va_if(u_va_has(__VA_ARGS__)) (                                                                 \
    u_va_defer(2, __u_va_map_def)()(macro, __VA_ARGS__)                                            \
  )

#ifdef u_va_debug
echo(map)
#  define _a(n) (n)
u_va_map(_a)                      /* :                         */
u_va_map(_a, 1)                   /* : (1)                     */
u_va_map(_a, 1, 2)                /* : (1), (2)                */
u_va_map(_a, 1, 2, 3)             /* : (1), (2), (3)           */
u_va_map(_a, 1, 2, 3, 4)          /* : (1), (2), (3), (4)      */
u_va_map(_a, 1, 2, 3, 4, 5)       /* : (1), (2), (3), (4), (5) */
#endif


/*
 * mapof
 * */
#define u_va_mapof(macro, ...) u_va_if(u_va_has(__VA_ARGS__)) (                                    \
    u_va_eval(__u_va_mapof(macro, __VA_ARGS__))                                                    \
  )
#define __u_va_mapof_def()     __u_va_mapof
#define __u_va_mapof(macro, first, ...)                                                            \
  macro(first)                                                                                     \
  u_va_if(u_va_has(__VA_ARGS__)) (                                                                 \
    u_va_defer(2, __u_va_mapof_def)()(macro, __VA_ARGS__)                                          \
  )

#ifdef u_va_debug
echo(mapof)
#  define _b(n) (n),
u_va_mapof(_b)                    /* :                     */
u_va_mapof(_b, 1)                 /* : (1),                */
u_va_mapof(_b, 1, 2)              /* : (1), (2),           */
u_va_mapof(_b, 1, 2, 3)           /* : (1), (2), (3),      */
u_va_mapof(_b, 1, 2, 3, 4)        /* : (1), (2), (3), (4), */
#endif

#endif /* !LIBU_VA_EVAL */

#endif /* !U_VA_H__ */
