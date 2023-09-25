#pragma once

#if !defined(__clang__) || __clang_major__ < 16
#  error "Please use the Clang v16 or above toolchain, do not use the GCC toolchain"
#endif

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdalign.h>
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
