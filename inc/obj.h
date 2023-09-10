#pragma once

#include "u.h"

#define __OBJ_MAXPHSIZE 1024
#define __OBJ_CLOFNNUM  0x58ffffbffdffffafULL

static void __obj_call_fun(void) {
}

#define obj_self(T, var)                                                                           \
  volatile uintptr_t __obj_##var = __OBJ_CLOFNNUM;                                                 \
  typeof(T)(var)                 = any(__obj_##var);                                               \
  __obj_call_fun();

#define obj_cleanup(method) u_free_if(any(method))

void* __obj_method(void* prototype, void* data);
#define obj_method(self, method, func)                                                             \
  do {                                                                                             \
    (self)->method = __obj_method(func, self);                                                     \
  } while (0)
