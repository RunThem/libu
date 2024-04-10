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

#ifndef U_H__
#define U_H__

#if defined(__clang__) && __clang_major__ < 16
#  error "Please use the Clang.v16 or later toolchain."
#endif

#if defined(__GNUC__) && __GNUC__ < 13 && !defined(__clang__)
#  error "Please use the GCC.v13 or later toolchain."
#endif

#include "utils/alloc.h"
#include "utils/debug.h"
#include "utils/io.h"
#include "utils/keyword.h"
#include "utils/misc.h"
#include "utils/print.h"
#include "utils/type.h"
#include "utils/va.h"

/**/

#if defined(uvec_def)
#  include "ivec.h"
#endif

#if defined(umap_def)
#  include "imap.h"
#endif

#if defined(uavl_def)
#  include "iavl.h"
#endif

#if defined(ulst_def)
#  include "ilst.h"
#endif

/* #include "istr.h" */

#endif /* !U_H__ */
