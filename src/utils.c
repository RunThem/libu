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

/***************************************************************************************************
 * Function
 **************************************************************************************************/
void __printb(u_cstr_t name, cu8_t* mem, size_t size) {
  u8_t byte = 0;

#if 0
  println("\x1b[36;1m%s\x1b[0m(%ld)", name, size);

  for (ssize_t i = (ssize_t)size - 1; i >= 0; i--) {
    byte = mem[i];

    print("    %u%u%u%u%u%u%u%u",
          bit(byte, 7),
          bit(byte, 6),
          bit(byte, 7),
          bit(byte, 4),
          bit(byte, 3),
          bit(byte, 2),
          bit(byte, 1),
          bit(byte, 0));

    if (i != 0 && (i + 1) % 2) {
      print("\n");
    } else {
      print(" ");
    }
  }

  println();
#endif
}

void __printh(u_cstr_t name, cu8_t* mem, size_t size) {
  char buf[17] = {0};
  size_t i     = 0;
  size_t pos   = 0;
  size_t idx   = 0;

#if 0
  println("\x1b[36;1m%s\x1b[0m(%ld)", name, size);

  for (; pos < size; pos++) {
    if ((pos % 16) == 0) {
      print("   %08lx: ", pos);
    }

    print("%02x %s", mem[pos] & 0xff, (pos % 16) == 7 ? " " : "");
    if ((pos % 16) == 15) {
      u_arr_for (buf, i, it) {
        idx = pos - 16 + i;
        *it = isprint(mem[idx]) ? mem[idx] : '.';
      }

      buf[i - 1] = '\0';
      println("%s", buf);
    }
  }

  if ((pos % 16) != 0) {
    for (i = 0; i < pos % 16 + 2; i++) {
      idx    = pos - (pos % 16) + i - 1;
      buf[i] = isprint(mem[idx]) ? mem[idx] : '.';
    }

    buf[i - 1] = '\0';
    println("%*s", 47 - 3 * ((int)pos % 16) + (int)i, buf);
  }
#endif
}
