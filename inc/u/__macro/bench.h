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

#pragma once

#include "type.h"

#include <time.h>

#define nsec_of(tv)         ((tv).tv_sec * 1000000000 + (tv).tv_nsec) /* timespec */
#define nsec_diff(tv1, tv2) (nsec_of(tv1) - nsec_of(tv2))
typedef struct {
  str_t msg;
  struct timespec begin;
  struct timespec end;
  size_t diff;

  size_t i;
  size_t n;
} __tack_t;

extern bool __benchmark(__tack_t* tack);

#define benchmark(...)                                                                             \
  for (__tack_t T = {__VA_ARGS__}; (T.i != 0 && T.i != T.n) || __benchmark(&T); T.i++)
