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

int u_vsnprintf(u_cstr_t buf, size_t maxl, u_cstr_t fmt, va_list va) {
  u32_t flags     = 0;
  u32_t width     = 0;
  u32_t precision = 0;
  u32_t n         = 0;
  size_t idx      = 0;

  while (*fmt) {
    /* format specifier? (%[flags][width][.precision][length] or %{[type]}) */
    if (*fmt != '%') {
      buf[idx++] = *fmt;
      fmt++;
      continue;
    } else {
      fmt++;
    }

    /* evaluate flags */
    flags = 0;
    do {
      /* clang-format off */
      switch (*fmt) {
        case '0': flags |= FLAGS_ZEROPAD; fmt++; n = 1; break;
        case '-': flags |= FLAGS_LEFT;    fmt++; n = 1; break;
        case '+': flags |= FLAGS_PLUS;    fmt++; n = 1; break;
        case ' ': flags |= FLAGS_SPACE;   fmt++; n = 1; break;
        case '#': flags |= FLAGS_HASH;    fmt++; n = 1; break;
        default:                                 n = 0; break;
      }
      /* clang-format on */
    } while (n);

    /* evaluate width field */
    width = 0;
    if (isdigit(*fmt)) {
      width = _atoi(&fmt);
    } else if (*fmt == '*') {
      int w = va_arg(va, int);
      if (w < 0) {
        flags |= FLAGS_LEFT; /* reverse padding */
        width = -w;
      } else {
        width = w;
      }

      fmt++;
    }

    /* evaluate precision field */
    precision = 0;
    if (*fmt == '.') {
      flags |= FLAGS_PRECISION;
      fmt++;
      if (isdigit(*fmt)) {
        precision = _atoi(&fmt);
      } else if (*fmt == '*') {
        int prec  = va_arg(va, int);
        precision = prec > 0 ? prec : 0;
        fmt++;
      }
    }

    /* evaluate length field */
    switch (*fmt) {
      case 'h':
        flags |= FLAGS_SHORT;
        fmt++;
        if (*fmt == 'h') {
          flags |= FLAGS_CHAR;
          fmt++;
        }
        break;

      case 'l':
        flags |= FLAGS_LONG;
        fmt++;
        if (*fmt == 'l') {
          flags |= FLAGS_LONG_LONG;
          fmt++;
        }
        break;

      case 't':
      case 'j':
      case 'z':
        flags |= FLAGS_LONG_LONG;
        fmt++;
        break;

      default:
        break;
    }
  }

  return 0;
}

int main(int argc, const u_cstr_t argv[]) {
  char str[] = "hello";
  int result = typeeq(char[], str);
  infln("result is %d", result);

  u_str_t us = &me(struct u_str_t, .len = 5, .cap = 16, .data = str);

#define basetypeid(t)                                                                              \
  _Generic(t,                                                                                      \
      char: "char",                                                                                \
      bool: "bool",                                                                                \
      i8_t: "i8_t",                                                                                \
      u8_t: "u8_t",                                                                                \
      i16_t: "i16_t",                                                                              \
      u16_t: "u16_t",                                                                              \
      i32_t: "i32_t",                                                                              \
      u32_t: "u32_t",                                                                              \
      i64_t: "i64_t",                                                                              \
      u64_t: "u64_t",                                                                              \
      i128_t: "i128_t",                                                                            \
      u128_t: "u128_t",                                                                            \
      f32_t: "f32_t",                                                                              \
      f64_t: "f64_t",                                                                              \
      f128_t: "f128_t",                                                                            \
      any_t: "any_t",                                                                              \
      u_cstr_t: "u_cstr_t",                                                                        \
      default: "other")

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

  println("%s", basetypeid(v1));
  println("%s", basetypeid(v2));
  println("%s", basetypeid(v3));
  println("%s", basetypeid(v4));
  println("%s", basetypeid(v5));
  println("%s", basetypeid(v6));
  println("%s", basetypeid(v7));
  println("%s", basetypeid(v8));
  println("%s", basetypeid(v9));
  println("%s", basetypeid(v10));
  println("%s", basetypeid(v11));
  println("%s", basetypeid(v12));
  println("%s", basetypeid(v13));
  println("%s", basetypeid(v14));
  println("%s", basetypeid(v15));
  println("%s", basetypeid(v16));
  println("%s", basetypeid(v17));

  /*
   * 自定义 vnprintf() 可行性报告
   *
   * 不使用 gcc 扩展几乎不可行
   *
   * 整数:   1
   * 浮点数: 8
   * 指针:   5
   *
   *
   * */

  return EXIT_SUCCESS;
err:
  errln("failed.");

  return EXIT_FAILURE;
}
