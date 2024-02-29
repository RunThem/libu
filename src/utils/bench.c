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

thread_local __err__t __err__ = {};

inline bool __benchmark(__tack_t* tack) {
  char buf[32] = {0};

  if (tack->i == 0) {
    clock_gettime(CLOCK_REALTIME, &tack->begin);
    return true;
  }

  clock_gettime(CLOCK_REALTIME, &tack->end);
  tack->diff = ((tack->end.tv_sec * 1000000000 + tack->end.tv_nsec) -
                (tack->begin.tv_sec * 1000000000 + tack->begin.tv_nsec)) /
               tack->n;

  if (tack->diff > 1000 * 1000 * 1000) {
    snprintf(buf, sizeof(buf), "%6.2f\x1b[35ms\x1b[0m", (f64_t)tack->diff / 1000 * 1000 * 1000);
  } else if (tack->diff > 1000 * 1000) {
    snprintf(buf, sizeof(buf), "%6.2f\x1b[34mms\x1b[0m", (f64_t)tack->diff / 1000 * 1000);
  } else if (tack->diff > 1000) {
    snprintf(buf, sizeof(buf), "%6.2f\x1b[33mus\x1b[0m", (f64_t)tack->diff / 1000);
  } else {
    snprintf(buf, sizeof(buf), "%6zu\x1b[32mns\x1b[0m", tack->diff);
  }

  // #define __INF_FMT       "[INF]\x1b[02m[%s $%d %s]\x1b[0m: "
  fprintf(stderr, "    { %s }[\x1b[31m%lu\x1b[0m] bench @%s\n", buf, tack->n, tack->msg);

  return false;
}
