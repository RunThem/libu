/* libs */
#include <sys/mman.h>
#include <sys/user.h>
#include <unistd.h>

// #include "buf.h"
// #include "fs.h"
// #include "list.h"
#include "map.h"
// #include "queue.h"
// #include "sock.h"
// #include "stack.h"
// #include "str.h"
#include "u.h"
// #include "vec.h"

// #include <backtrace-supported.h>
// #include <backtrace.h>

#define _typeof(t) __builtin_classify_type(t)

any_t __bt_state = nullptr;

void boo() {
  // backtrace_print((struct backtrace_state*)__bt_state, 0, stderr);
}

#define __OBJ_MAXPHSIZE 1024
#define __OBJ_CLOFNNUM  0x58ffffbffdffffafULL

static inline bool obj_active_memory(void* ptr, size_t size) {
  return mprotect((void*)(((size_t)ptr >> PAGE_SHIFT) << PAGE_SHIFT),
                  size,
                  PROT_READ | PROT_EXEC | PROT_WRITE) == 0;
}

static any_t obj(any_t prototype, void* data) {
  uint8_t* code = nullptr;
  size_t offset = 0;
  size_t ihsize;

  for (; offset < __OBJ_MAXPHSIZE; offset++) {
    if (*as(prototype + offset, size_t*) == __OBJ_CLOFNNUM) {
      break;
    }
  }

  u_err_if(offset == 0);

#pragma pack(push, 1)
  // push rax
  // mov  rax, addr
  // jmp  rax
  static struct {
    uintptr_t data;
    uint8_t push_rax;
    uint8_t mov_rax[2];
    uintptr_t addr;
    uint8_t jmp_rax[2];
  } asmc = {
      .push_rax = 0x50,
      .mov_rax  = {0x48, 0xB8},
      .jmp_rax  = {0xFF, 0xE0}
  };
  // ihsize = offset + sizeof(void *) * 2 + 5;
#pragma pack(pop)

  ihsize = offset + sizeof(asmc);
  code   = u_talloc(ihsize, uint8_t*);
  u_mem_if(code);

  u_err_if(obj_active_memory(code, ihsize) == 0);

  asmc.addr = (uintptr_t)prototype + offset + sizeof(uintptr_t) - 1;
  asmc.data = (uintptr_t)data;

  memcpy(code, prototype, offset);
  memcpy(code + offset, &asmc, sizeof(asmc));

  return code;

err:
  inf("could't find closure declaration at prototype function (%p)!", prototype);

  u_free_if(code);

  return nullptr;
}

#define obj_setup(T, var) volatile typeof(T*)(var) = (void*)__OBJ_CLOFNNUM

typedef struct {
  int a;

  int (*get)(void);
  void (*set)(int);
} st_t;

static void st_set(int num) {
  obj_setup(st_t, self);

  inf("self is %p", self);

  self->a = num;
}

static int st_get(void) {
  // volatile st_t* self = any(__OBJ_CLOFNNUM);
  obj_setup(st_t, self);

  inf("self is %p", self);

  return self->a;
}

static st_t* st_new() {
  st_t* self = u_talloc(sizeof(st_t), st_t*);
  u_mem_if(self);

  self->get = obj(st_get, self);
  self->set = obj(st_set, self);

  return self;

err:
  return nullptr;
}

int main(int argc, const char** argv) {
  // __bt_state = backtrace_create_state(argv[1], 0, nullptr, nullptr);

  st_t* st = st_new();
  // inf("st is %p", st);

  st->get = obj(st_get, nullptr);

  st_get();

  // st->set(10);

  // inf("%d", st->get());

  // st->set(9234);

  // inf("%d", st->get());

  return 0;
}
