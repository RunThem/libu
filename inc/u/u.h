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

#ifndef U_H__
#  define U_H__

#  ifdef __cplusplus
extern "C" {
#  endif

#  if 0
#    define u_defs(type, ...) u_va_map(_u_##type##_defs, __VA_ARGS__)
#  endif

#  if defined(__clang__) && __clang_major__ < 16
#    error "Please use the Clang.v16 or later toolchain."
#  endif

#  if defined(__GNUC__) && __GNUC__ < 13 && !defined(__clang__)
#    error "Please use the GCC.v13 or later toolchain."
#  endif

#  include <assert.h>
#  include <ctype.h>
#  include <errno.h>
#  include <fcntl.h>
#  include <setjmp.h>
#  include <stdalign.h>
#  include <stdarg.h>
#  include <stdatomic.h>
#  include <stdbool.h>
#  include <stddef.h>
#  include <stdint.h>
#  include <stdio.h>
#  include <stdlib.h>
#  include <string.h>
#  include <strings.h>
#  include <sys/socket.h>
#  include <sys/types.h>
#  include <threads.h>
#  include <time.h>
#  include <ucontext.h>
#  include <unistd.h>

/* clang-format off */
#  include "utils/va.h"
#  include "utils/type.h"
#  include "utils/keyword.h"
#  include "utils/alloc.h"
#  include "utils/misc.h"
#  include "utils/debug.h"
#  include "utils/print.h"
#  include "iatomic.h"
#  include "iavl.h"
#  include "ibuf.h"
#  include "ilock.h"
#  include "idbg.h"
#  include "ilst.h"
#  include "imap.h"
#  include "iset.h"
#  include "ivec.h"
#  include "ilfq.h"
#  include "iheap.h"
#  include "istr.h"
/* clang-format on */

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_H__ */
