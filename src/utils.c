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

#include <u/u.h>

/***************************************************************************************************
 * Macro
 **************************************************************************************************/
/*
 * cmp & eq func from base type
 * */
fn_compe_def(char, (x == y), (x > y));
fn_compe_def(int, (x == y), (x > y));
fn_compe_def(uint, (x == y), (x > y));
fn_compe_def(byte_t, (x == y), (x > y));
fn_compe_def(i8_t, (x == y), (x > y));
fn_compe_def(u8_t, (x == y), (x > y));
fn_compe_def(i16_t, (x == y), (x > y));
fn_compe_def(u16_t, (x == y), (x > y));
fn_compe_def(i32_t, (x == y), (x > y));
fn_compe_def(u32_t, (x == y), (x > y));
fn_compe_def(i64_t, (x == y), (x > y));
fn_compe_def(u64_t, (x == y), (x > y));

fn_compe_def(size_t, (x == y), (x > y));
fn_compe_def(ssize_t, (x == y), (x > y));

#ifdef __SIZEOF_INT128__
fn_compe_def(i128_t, (x == y), (x > y));
fn_compe_def(u128_t, (x == y), (x > y));
#endif

fn_compe_def(u_str_t, (x->len == y->len && 0 == strcmp(x->ptr, y->ptr)), (strcmp(x->ptr, y->ptr)));

/***************************************************************************************************
 * Hahs function
 **************************************************************************************************/
/* clang-format off */
[[gnu::unused]]
pub inline u_hash_t u_hash_int8bit(const u8_t* ptr, size_t len)  { return (u_hash_t) * (u8_t*)ptr; }
pub inline u_hash_t u_hash_int16bit(const u8_t* ptr, size_t len) { return (u_hash_t) * (u16_t*)ptr; }
pub inline u_hash_t u_hash_int32bit(const u8_t* ptr, size_t len) { return (u_hash_t) * (u32_t*)ptr; }
pub inline u_hash_t u_hash_int64bit(const u8_t* ptr, size_t len) { return (u_hash_t) * (u64_t*)ptr; }
/* clang-format on */

/***************************************************************************************************
 * Display
 **************************************************************************************************/
pub void __printb(u_cstr_t name, const u8_t* mem, size_t size) {
  u8_t byte = 0;

  printf("\x1b[36;1m%s\x1b[0m(%ld)\n", name, size);

  for (size_t pos = 0; pos < size; pos++) {
    byte = mem[pos];

    if ((pos % 8) == 0) {
      printf("  0x%08lx: ", pos);
    }

    printf("%d%d%d%d%d%d%d%d %s",
           bit(byte, 7),
           bit(byte, 6),
           bit(byte, 5),
           bit(byte, 4),
           bit(byte, 3),
           bit(byte, 2),
           bit(byte, 1),
           bit(byte, 0),
           (pos % 4) == 3 ? " " : "");

    if (pos != 0 && pos != size - 1 && (pos + 1) % 8 == 0) {
      printf("\n");
    }
  }

  printf("\n");
}

pub void __printh(u_cstr_t name, const u8_t* mem, size_t size) {
  char buf[17] = {0};
  size_t i     = 0;
  size_t pos   = 0;
  size_t idx   = 0;

  printf("\x1b[36;1m%s\x1b[0m(%ld)\n", name, size);

  for (; pos < size; pos++) {
    if ((pos % 16) == 0) {
      printf("  0x%08lx: ", pos);
    }

    printf("%02x %s", mem[pos] & 0xff, (pos % 16) == 7 ? " " : "");
    if ((pos % 16) == 15) {
      for (i = 0; i < sizeof(buf) - 1; i++) {
        idx    = pos - 15 + i;
        buf[i] = isprint(mem[idx]) ? mem[idx] : '.';
      }

      buf[i - 1] = '\0';
      printf("%s\n", buf);
    }
  }

  if ((pos % 16) != 0) {
    for (i = 0; i < pos % 16 + 2; i++) {
      idx    = pos - (pos % 16) + i - 1;
      buf[i] = isprint(mem[idx]) ? mem[idx] : '.';
    }

    buf[i - 1] = '\0';
    printf("%*s\n", 47 - 3 * ((int)pos % 16) + (int)i, buf);
  }
}

/***************************************************************************************************
 * Benchmark
 **************************************************************************************************/
#define BASE_THOUSANDS(take, base)                                                                 \
  ({                                                                                               \
    auto _ = 0;                                                                                    \
                                                                                                   \
    if (take != 0) {                                                                               \
      _ = take % base;                                                                             \
      take /= base;                                                                                \
    }                                                                                              \
                                                                                                   \
    _;                                                                                             \
  })

typedef struct {
  struct timespec s;
  struct timespec e;
  size_t cnt;
  char* msg;
  bool run;
} u_bm_t;

thread_local u_bm_t bm;

pub bool __bm_entry(const char* msg, size_t cnt) {
  size_t total = 10'0000'0000L; /* nanoseconds/per second */
  size_t ave   = 0;
  size_t s     = 0; /* second */
  size_t ms    = 0; /* millisecond */
  size_t us    = 0; /* microsecond */
  size_t ns    = 0; /* nanosecond */

  /* start */
  if (!bm.run) {
    bm.run = true;
    bm.msg = strdup(msg);
    bm.cnt = cnt == 0 ? 1 : cnt;
    clock_gettime(CLOCK_MONOTONIC, &bm.s);
  } else { /* end */
    bm.run = false;
    clock_gettime(CLOCK_MONOTONIC, &bm.e);
    total = total * (bm.e.tv_sec - bm.s.tv_sec) + (bm.e.tv_nsec - bm.s.tv_nsec);

    ave   = total / cnt;
    ns    = total % 1000;
    total = total / 1000;
    us    = total % 1000;
    total = total / 1000;
    ms    = total % 1000;
    total = total / 1000;
    s     = total % 1000;
    total = total / 1000;

#define C(s) "\x1b[32m" #s "\x1b[0m"
    fprintf(stderr, "Benchmark %s:\n", bm.msg);
    fprintf(stderr,
            "Total time: %3zu" C(s) ", %3zu" C(ms) ", %3zu" C(us) ", %3zu" C(ns) ". ",
            s,
            ms,
            us,
            ns);

    if (cnt != 1) {
      fprintf(stderr, "Average time: %5zu" C(ns) "/%zu", ave, (size_t)cnt);
    }

    fprintf(stderr, "\n");

    free(bm.msg);
  }

  return bm.run;
}
