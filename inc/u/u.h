#pragma once

#if defined(__clang__) && __clang_major__ < 16
#  error "Please use the Clang.v16 or later toolchain."
#endif

#if defined(__GNUC__) && __GNUC__ < 13 && !defined(__clang__)
#  error "Please use the GCC.v13 or later toolchain."
#endif

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <setjmp.h>
#include <stdalign.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <time.h>

/* clang-format off */
#include "__macro/va.h"
#include "__macro/keyword.h"
#include "__macro/type.h"
#include "__macro/alloc.h"
#include "__macro/debug.h"
#include "__macro/print.h"
#include "__macro/misc.h"

#include "core.h"
#include "util.h"
