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

#include <u/utils/misc.h>
#include <u/utils/print.h>

void __printb(u_cstr_t name, const u8_t* mem, size_t size) {
  u8_t byte = 0;

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
}
