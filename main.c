/* libs */
// #include "buf.h"
// #include "fs.h"
// #include "list.h"
#include "map.h"
// #include "queue.h"
// #include "sock.h"
// #include "stack.h"
// #include "str.h"
#include "u.h"
#include "vec.h"

// #include <backtrace-supported.h>
// #include <backtrace.h>

#define _typeof(t) __builtin_classify_type(t)

any_t __bt_state = nullptr;

void boo() {
  // backtrace_print((struct backtrace_state*)__bt_state, 0, stderr);
}

#define CLOFN_PHSIZE_MAX      1024
#define _CLOFN_SCIENCE_NUMBER 0x58ffffbffdffffafULL

#include <sys/mman.h>
#include <sys/user.h>
static inline bool _clofn_active_memory(void* ptr, size_t size) {
  return mprotect((void*)(((size_t)ptr >> PAGE_SHIFT) << PAGE_SHIFT),
                  size,
                  PROT_READ | PROT_EXEC | PROT_WRITE) == 0;
}

#define def_clofn(result_type,                                                                     \
                  name,                                                                            \
                  closure_type /* size equal to machine word */,                                   \
                  closure_name,                                                                    \
                  args,                                                                            \
                  body)                                                                            \
  static result_type _clofn__##name args {                                                         \
    volatile closure_type closure_name = (closure_type)_CLOFN_SCIENCE_NUMBER;                      \
    body                                                                                           \
  }                                                                                                \
  static size_t _clofn__##name##__phsize = 0;                                                      \
  static size_t _clofn__##name##__phhash = 0;

void* _new_clofn(void* prototype, size_t* phsize, void* data) {
  printf("Clofn: prototype header (%p) { ", prototype);

  size_t offset = *phsize;
  if (!offset) {
    for (; offset < CLOFN_PHSIZE_MAX; offset++) {
      if (*(size_t*)((uintptr_t)prototype + offset) == (size_t)_CLOFN_SCIENCE_NUMBER) {
        if (!*phsize) {
          *phsize = offset;
        }

        printf("} @%zu+%zu\n", offset, sizeof(uintptr_t));

        goto mk;
      } else
        printf("%02X ", *(uint8_t*)(prototype + offset));
    }
    puts("...");

    printf("Clofn: could't find closure declaration at prototype function (%p)!\n", prototype);
    return NULL;
  } else {
    printf("Clofn: prototype header (%p) { ", prototype);
    for (size_t i = 0; i < (size_t)phsize; i++) {
      printf("%02X ", *(uint8_t*)(prototype + i));
    }
    printf("} @%zu+%lu\n", offset, sizeof(uintptr_t));
  }

mk:;

  size_t ihsize = offset + sizeof(void*) * 2 + 5;

  void* instance = malloc(ihsize);
  if (!_clofn_active_memory(instance, ihsize)) {
    puts("Clofn: could't change memory type of C.malloc allocated!");
    free(instance);
    return NULL;
  }
  memcpy(instance, prototype, offset);
  uintptr_t current = (uintptr_t)instance + offset;
  *(void**)current  = data;
  current += sizeof(void*);

  *(uint8_t*)current = 0x50;
  current++;
  *(uint8_t*)current = 0x48;
  current++;
  *(uint8_t*)current = 0xB8;
  current++;
  *(uintptr_t*)current =
      (uintptr_t)prototype + offset + sizeof(uintptr_t) - 1;  // 0x58 in _CLOFN_SCIENCE_NUMBER
  current += sizeof(uintptr_t);
  *(uint16_t*)current = 0xE0FF;

  printf("Clofn: instance header (%p) { ", instance);
  for (size_t i = 0; i < ihsize; i++) {
    printf("%02X ", *(uint8_t*)(instance + i));
  }
  printf("}\n");

  return instance;
}

typedef struct {
  int a;
  void (*get_a)(void);
} st_t;

static void _clofn__foo(void) {
  volatile size_t data = (size_t)0x58ffffbffdffffafULL;
  st_t* st             = (void*)data;

  printf("I'm foo, %d\n", st->a);
}

static size_t _clofn__foo__phsize = 0;
static size_t _clofn__foo__phhash = 0;

int main(int argc, const char** argv) {
  // __bt_state = backtrace_create_state(argv[1], 0, nullptr, nullptr);

#if 0
  st_t* st = malloc(sizeof(st_t));

  st->a = 12;

  // st->get_a = _new_clofn(_clofn__foo, &_clofn__foo__phsize, st);
  typeof(st->get_a) fn = _new_clofn(_clofn__foo, &_clofn__foo__phsize, st);

  fn();
#endif

  map(int, char) m = nullptr;

  m = map_new(int, char, fn_eq_use(int32), MAP_INT_HASH_FN);

  for (size_t i = 1; i < 10000; i++) {
    map_push(m, i, 'a');
  }

  for (size_t i = 1; i < 10000; i++) {
    map_pop(m, i);
  }

  // map_for(m) {
  //   inf("%d, '%c'", m->key, *m->val);
  // }

  map_cleanup(m);

  return 0;
}
