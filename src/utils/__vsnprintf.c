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
 * */

#include <ctype.h>
#include <stdarg.h>
#include <u/utils/misc.h>

/*
 * 自定义 vnprintf() 可行性报告
 *
 * 不使用 gcc 扩展几乎不可行
 *
 * 整数:   1
 * 浮点数: 8
 * 指针:   5
 *
 * {[flags][width][.precision][format]}
 * {:type}
 *
 * format:
 *   {b} 二进制
 *   {o} 八进制
 *   {x} 十六进制
 *   {X} 十六进制
 *   {e} 科学计数法
 *   {E} 科学计数法
 *
 * flags:
 *   - 右对齐
 *   + 数字类型时输出正负号
 *   # 添加头, 如 #x10-> 0xa, #'c' -> 'c'
 *   0 左边补零
 *
 * width:
 *   number 宽度
 *
 * precision:
 *   .number 精度
 *
 * :type:
 *   自定义类型的格式化输出
 *
 * {{:
 *   '{'
 *
 * }}:
 *   '}'
 * */

/* internal flag definitions */
#define FLAGS_ZEROPAD   (1U << 0U)
#define FLAGS_LEFT      (1U << 1U)
#define FLAGS_PLUS      (1U << 2U)
#define FLAGS_SPACE     (1U << 3U)
#define FLAGS_HASH      (1U << 4U)
#define FLAGS_UPPERCASE (1U << 5U)
#define FLAGS_CHAR      (1U << 6U)
#define FLAGS_SHORT     (1U << 7U)
#define FLAGS_LONG      (1U << 8U)
#define FLAGS_LONG_LONG (1U << 9U)
#define FLAGS_PRECISION (1U << 10U)
#define FLAGS_ADAPT_EXP (1U << 11U)

// 'ntoa' conversion buffer size, this must be big enough to hold one converted
// numeric number including padded zeros (dynamically created on stack)
// default: 32 byte
#ifndef PRINTF_NTOA_BUFFER_SIZE
#  define PRINTF_NTOA_BUFFER_SIZE 32U
#endif

// 'ftoa' conversion buffer size, this must be big enough to hold one converted
// float number including padded zeros (dynamically created on stack)
// default: 32 byte
#ifndef PRINTF_FTOA_BUFFER_SIZE
#  define PRINTF_FTOA_BUFFER_SIZE 32U
#endif

// support for the floating point type (%f)
// default: activated
#ifndef PRINTF_DISABLE_SUPPORT_FLOAT
#  define PRINTF_SUPPORT_FLOAT
#endif

// support for exponential floating point notation (%e/%g)
// default: activated
#ifndef PRINTF_DISABLE_SUPPORT_EXPONENTIAL
#  define PRINTF_SUPPORT_EXPONENTIAL
#endif

// define the default floating point precision
// default: 6 digits
#ifndef PRINTF_DEFAULT_FLOAT_PRECISION
#  define PRINTF_DEFAULT_FLOAT_PRECISION 6U
#endif

// define the largest float suitable to print with %f
// default: 1e9
#ifndef PRINTF_MAX_FLOAT
#  define PRINTF_MAX_FLOAT 1e9
#endif

// support for the long long types (%llu or %p)
// default: activated
#ifndef PRINTF_DISABLE_SUPPORT_LONG_LONG
#  define PRINTF_SUPPORT_LONG_LONG
#endif

// support for the ptrdiff_t type (%t)
// ptrdiff_t is normally defined in <stddef.h> as long or long long type
// default: activated
#ifndef PRINTF_DISABLE_SUPPORT_PTRDIFF_T
#  define PRINTF_SUPPORT_PTRDIFF_T
#endif

static uint _atoi(u_cstr_t* str) {
  uint i = 0;
  while (isdigit(**str)) {
    i = i * 10u + (uint)(**str - '0');
    (*str)++;
  }

  return i;
}

// internal buffer output
static inline void out(char ch, u_cstr_t buf, size_t idx, size_t maxl) {
  if (idx < maxl) {
    ((char*)buf)[idx] = ch;
  }
}

// output the specified string in reverse, taking care of any zero-padding
static size_t out_rev(u_cstr_t buffer,
                      size_t idx,
                      size_t maxl,
                      u_cstr_t buf,
                      size_t len,
                      uint width,
                      uint flags) {
  const size_t start_idx = idx;

  // pad spaces up to given width
  if (!(flags & FLAGS_LEFT) && !(flags & FLAGS_ZEROPAD)) {
    for (size_t i = len; i < width; i++) {
      out(' ', buffer, idx++, maxl);
    }
  }

  // reverse string
  while (len) {
    out(buf[--len], buffer, idx++, maxl);
  }

  // append pad spaces up to given width
  if (flags & FLAGS_LEFT) {
    while (idx - start_idx < width) {
      out(' ', buffer, idx++, maxl);
    }
  }

  return idx;
}

// internal itoa format
static size_t _ntoa_format(u_cstr_t buffer,
                           size_t idx,
                           size_t maxl,
                           u_cstr_t buf,
                           size_t len,
                           bool negative,
                           uint base,
                           uint prec,
                           uint width,
                           uint flags) {
  // pad leading zeros
  if (!(flags & FLAGS_LEFT)) {
    if (width && (flags & FLAGS_ZEROPAD) && (negative || (flags & (FLAGS_PLUS | FLAGS_SPACE)))) {
      width--;
    }
    while ((len < prec) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
      buf[len++] = '0';
    }
    while ((flags & FLAGS_ZEROPAD) && (len < width) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
      buf[len++] = '0';
    }
  }

  // handle hash
  if (flags & FLAGS_HASH) {
    if (!(flags & FLAGS_PRECISION) && len && ((len == prec) || (len == width))) {
      len--;
      if (len && (base == 16U)) {
        len--;
      }
    }
    if ((base == 16U) && !(flags & FLAGS_UPPERCASE) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
      buf[len++] = 'x';
    } else if ((base == 16U) && (flags & FLAGS_UPPERCASE) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
      buf[len++] = 'X';
    } else if ((base == 2U) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
      buf[len++] = 'b';
    }
    if (len < PRINTF_NTOA_BUFFER_SIZE) {
      buf[len++] = '0';
    }
  }

  if (len < PRINTF_NTOA_BUFFER_SIZE) {
    if (negative) {
      buf[len++] = '-';
    } else if (flags & FLAGS_PLUS) {
      buf[len++] = '+';  // ignore the space if the '+' exists
    } else if (flags & FLAGS_SPACE) {
      buf[len++] = ' ';
    }
  }

  return out_rev(buffer, idx, maxl, buf, len, width, flags);
}

size_t __vsnprintf(u_cstr_t buf, size_t maxl, u_cstr_t fmt, va_list va) {
  uint flags     = 0;
  uint width     = 0;
  uint precision = 0;
  uint format    = 0;
  uint base      = 0;
  uint n         = 0;
  size_t idx     = 0;
  bool isfmt     = false;

  while (*fmt) {
    /* format specifier? ({[flags][width][.precision]type} or {:type}) */
    if (fmt[0] == '}') {
      if (isfmt) {
        fmt++;
        isfmt = false;
        continue;
      } else {
        fmt++;

        if (fmt[0] == '}') {
          buf[idx++] = fmt[0];
          fmt++;
          continue;
        }
      }
    }

    if (fmt[0] != '{') {
      buf[idx++] = fmt[0];
      fmt++;
      continue;
    } else {
      fmt++;

      if (fmt[0] == '{') {
        buf[idx++] = fmt[0];
        fmt++;
        continue;
      }

      isfmt = true;
    }

    /*
     * TODO: {:type}
     * */

    /* evaluate flags */
    flags = 0;
    do {
      /* clang-format off */
      switch (fmt[0]) {
        case '0': flags |= FLAGS_ZEROPAD; fmt++; n = 1; break;
        case '-': flags |= FLAGS_LEFT;    fmt++; n = 1; break;
        case '+': flags |= FLAGS_PLUS;    fmt++; n = 1; break;
        case '#': flags |= FLAGS_HASH;    fmt++; n = 1; break;
        default:                                 n = 0; break;
      }
      /* clang-format on */
    } while (n);

    /* evaluate width field */
    width = 0;
    if (isdigit(fmt[0])) {
      width = _atoi(&fmt);
    }

    /* evaluate precision field */
    precision = 0;
    if (fmt[0] == '.') {
      flags |= FLAGS_PRECISION;
      fmt++;
      if (isdigit(fmt[0])) {
        precision = _atoi(&fmt);
      }
    }

    /* evaluate format field */
    base = 0;
    if (fmt[0] == 'X') {
      base = 16;
      flags |= FLAGS_UPPERCASE;
    } else if (fmt[0] == 'x') {
      base = 16;
    } else if (fmt[0] == 'o') {
      base = 8;
    } else if (fmt[0] == 'b') {
      base = 2;
    } else if (fmt[0] == 'E') {
      /* TODO */
      flags |= FLAGS_UPPERCASE;
    } else if (fmt[0] == 'e') {
      /* TODO */
    } else {
      base = 10;
      flags &= ~FLAGS_HASH;
    }

    if (fmt[0] != '}') {
      /* XXX: */
      continue;
    }

    fmt++;

    int type = va_arg(va, int);

    if (type == 1) {
    } else if (type == 2) {
    } else if (3 <= type && type <= 12) {
    } else if (13 <= type && type <= 15) {
    } else if (type == 128) {
    } else if (type == 129) {
    } else {
      /* XXX: */
    }
  }

  return idx;
}
