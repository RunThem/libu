/* third libs */
#include <u/u.h>

/* system libs */
#include <arpa/inet.h>
#include <dirent.h>
#include <fcntl.h>
#include <math.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netpacket/packet.h>
#include <regex.h>
#include <sys/ioctl.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <ucontext.h>
#include <unistd.h>

ret_t code = 0;

/*
 * namespace
 *
 * uv: vector
 * um: hash table
 * ut: avl tree
 * ul: list
 * us: string
 * ub: buffer
 * uf: file & directory
 * ug: log
 * */

typedef typeof(char*) u_cstr_t;
typedef struct u_str_t {
  size_t len;
  size_t cap;
  char* data;
}* u_str_t;

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

static u32_t _atoi(u_cstr_t* str) {
  u32_t i = 0;
  while (isdigit(**str)) {
    i = i * 10u + (u32_t)(**str - '0');
    (*str)++;
  }

  return i;
}

#define ch(c) ((char)(#c[0]))

#define u_builtin_basetypeid(t)                                                                    \
  ({                                                                                               \
    int _s = 0;                                                                                    \
                                                                                                   \
    if (__builtin_classify_type(t) == 5) {                                                         \
      _s = _Generic(t, u_cstr_t: 128, default: 129);                                               \
    } else {                                                                                       \
      _s = _Generic(t,                                                                             \
          default: 0,                                                                              \
          bool: 1,                                                                                 \
          char: 2,                                                                                 \
          i8_t: 3,                                                                                 \
          u8_t: 4,                                                                                 \
          i16_t: 5,                                                                                \
          u16_t: 6,                                                                                \
          i32_t: 7,                                                                                \
          u32_t: 8,                                                                                \
          i64_t: 9,                                                                                \
          u64_t: 10,                                                                               \
          i128_t: 11,                                                                              \
          u128_t: 12,                                                                              \
          f32_t: 13,                                                                               \
          f64_t: 14,                                                                               \
          f128_t: 15);                                                                             \
    }                                                                                              \
                                                                                                   \
    _s;                                                                                            \
  })

#define __u_vprintf_map(v) u_builtin_basetypeid(v), v
extern size_t _u_vprintf(u_cstr_t buf, size_t maxl, u_cstr_t fmt, va_list va);

#define u_snprintf(buf, maxl, fmt, ...)                                                            \
  _u_snprintf(buf, maxl, fmt, va_map(__u_vprintf_map, __VA_ARGS__))
size_t _u_snprintf(u_cstr_t buf, size_t maxl, u_cstr_t fmt, ...) {
  size_t ret;
  va_list ap;

  va_start(ap, fmt);
  ret = _u_vprintf(buf, maxl, fmt, ap);
  va_end(ap);

  return ret;
}

#define u_printf(fmt, ...) _u_printf(fmt, va_map(__u_vprintf_map, __VA_ARGS__))
size_t _u_printf(u_cstr_t fmt, ...) {
  size_t ret;
  va_list ap;
  char buf[4096] = {0};
  size_t maxl    = 4096;

  va_start(ap, fmt);
  ret = _u_vprintf(buf, maxl, fmt, ap);
  va_end(ap);

  ret = fwrite(buf, sizeof(char), ret, stdout);

  return ret;
}

size_t _u_vprintf(u_cstr_t buf, size_t maxl, u_cstr_t fmt, va_list va) {
  uint flags     = 0;
  uint width     = 0;
  uint precision = 0;
  uint format    = 0;
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
    switch (fmt[0]) {
      case 'x':
      case 'X':
      case 'o':
      case 'b': {
        uint base = 0;
        if (fmt[0] == 'X') {
          base = 16;
          flags |= FLAGS_UPPERCASE;
        } else if (fmt[0] == 'x') {
          base = 16;
        } else if (fmt[0] == 'o') {
          base = 8;
        } else if (fmt[0] == 'b') {
          base = 2;
        } else {
          base = 10;
          flags &= ~FLAGS_HASH;
        }
      }

      default:
        break;
    }
  }

  return idx;
}

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

int main(int argc, const u_cstr_t argv[]) {
  char str[] = "hello";
  int result = typeeq(char[], str);
  infln("result is %d", result);

  u_str_t us = &me(struct u_str_t, .len = 5, .cap = 16, .data = str);

  // char buf[100];
  // u_snprintf(buf, 100, "{}, {}", 1, 2);

  u_printf("{{}", true);

#if 0
  char v1;
  bool v2;
  i8_t v3;
  u8_t v4;
  i16_t v5;
  u16_t v6;
  i32_t v7;
  u32_t v8;
  i64_t v9;
  u64_t v10;
  i128_t v11;
  u128_t v12;
  f32_t v13;
  f64_t v14;
  f128_t v15;
  any_t v16;
  u_cstr_t v17;
  int* v18;
  char v19[10];

  println("v1   %d", u_builtin_basetypeid(v1));
  println("v2   %d", u_builtin_basetypeid(v2));
  println("v3   %d", u_builtin_basetypeid(v3));
  println("v4   %d", u_builtin_basetypeid(v4));
  println("v5   %d", u_builtin_basetypeid(v5));
  println("v6   %d", u_builtin_basetypeid(v6));
  println("v7   %d", u_builtin_basetypeid(v7));
  println("v8   %d", u_builtin_basetypeid(v8));
  println("v9   %d", u_builtin_basetypeid(v9));
  println("v10  %d", u_builtin_basetypeid(v10));
  println("v11  %d", u_builtin_basetypeid(v11));
  println("v12  %d", u_builtin_basetypeid(v12));
  println("v13  %d", u_builtin_basetypeid(v13));
  println("v14  %d", u_builtin_basetypeid(v14));
  println("v15  %d", u_builtin_basetypeid(v15));

  println("v16  %d", u_builtin_basetypeid(v16));
  println("v17  %d", u_builtin_basetypeid(v17));
  println("v18  %d", u_builtin_basetypeid(v18));
  println("v19  %d", u_builtin_basetypeid(v19));
  println("     %d", u_builtin_basetypeid("hello"));
  println("     %d", u_builtin_basetypeid(ch(1)));
#endif

  return EXIT_SUCCESS;
err:
  errln("failed.");

  return EXIT_FAILURE;
}
