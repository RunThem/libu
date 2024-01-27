#include <sys/mman.h>
#include <sys/user.h>

/* */
#include <u/util/obj.h>

/*
 * -[clofn](https://github.com/yulon/clofn)
 * -[obj.h](https://github.com/small-c/obj.h)
 *
 * @code:
 * ```
 * typedef struct {
 *   int num;
 *
 *   int (*get)(void);
 *   void (*set)(int);
 *   void (*sum)(int, int, int, int, int, int, int, int, int);
 * } st_t;
 *
 * void st_set(int num) {
 *   obj_self(st_t*, self);
 *
 *   self->num = num;
 * }
 *
 * int st_get(void) {
 *   obj_self(st_t*, self);
 *
 *   return self->num;
 * }
 *
 * void st_sum(int a, int b, int c, int d, int e, int f, int g, int h, int k) {
 *   obj_self(st_t*, self);
 *
 *   self->num = a + b + c + d + e + f + g + h + k;
 * }
 *
 * st_t* st_new() {
 *   st_t* self = u_zalloc(sizeof(st_t));
 *   u_mem_if(self);
 *
 *   obj_method(self, sum, st_sum);
 *   obj_method(self, get, st_get);
 *   obj_method(self, set, st_set);
 *
 *   inf("%p, %p, %p", self->set, self->get, self->sum);
 *
 *   return self;
 *
 * err:
 *   return nullptr;
 * }
 *
 * void st_cleanup(st_t* self) {
 *   u_assert(self == nullptr);
 *
 *   obj_cleanup(self->get);
 *   obj_cleanup(self->set);
 *   obj_cleanup(self->sum);
 *
 *   u_free(self);
 * }
 *
 * int main(int argc, const char** argv) {
 *   st_t* st = st_new();
 *   inf("st is %p", st);
 *
 *   st->set(10);
 *   inf("a is %d", st->num);
 *   inf("%d", st->get());
 *
 *   st->set(9234);
 *   inf("%d", st->get());
 *
 *   st->sum(1, 2, 3, 4, 5, 6, 7, 8, 9);
 *   inf("%d", st->get());
 *
 *   st_cleanup(st);
 *
 *   return 0;
 * }
 * ```
 * */

#if defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
#  define __OBJ_X64 1
#elif defined(__i386) || defined(__i386__) || defined(_M_IX86)
#  define __OBJ_X86 1
#else
#  error This architecture is not supported!
#endif

any_t __obj_method(any_t prototype, any_t data) {
  u8_t* code    = nullptr;
  size_t offset = 0;
  size_t ihsize = 0;

  for (offset = 0; offset < __OBJ_MAXPHSIZE; offset++) {
    if (*(size_t*)((uintptr_t)prototype + offset) == (size_t)__OBJ_CLOFNNUM) {
      break;
    }
  }

  u_err_if(offset == 0 || offset == __OBJ_MAXPHSIZE);

#pragma pack(push, 1)
#if __OBJ_X64

  /*
   *  push rax
   *  mov  rax, addr
   *  jmp  rax
   * */
  static struct {
    uintptr_t data;
    u8_t push_rax;
    u8_t mov_rax[2];
    uintptr_t addr;
    u8_t jmp_rax[2];
  } asm_code = {
      .push_rax = 0x50,
      .mov_rax  = {0x48, 0xB8},
      .jmp_rax  = {0xFF, 0xE0}
  };
  /* ihsize = offset + sizeof(any_t) * 2 + 5; */

#elif __OBJ_X86
  /*
   * jmp addr
   * */
  static struct {
    uintptr_t data;
    u8_t jmp;
    uintptr_t addr;
  } asm_code = {.jmp = 0xE9};
  /* ihsize = offset + sizeof(any_t) * 2 + 1; */

#endif
#pragma pack(pop)

  ihsize = offset + sizeof(asm_code);
  code   = u_calloc(ihsize, sizeof(u8_t));
  u_mem_if(code);

  if (mprotect(any(((uintptr_t)code >> PAGE_SHIFT) << PAGE_SHIFT),
               ihsize,
               PROT_READ | PROT_EXEC | PROT_WRITE)) {
    return nullptr;
  }

#if __OBJ_X64
  asm_code.addr = (uintptr_t)prototype + offset + sizeof(uintptr_t) - 1;
#elif __OBJ_X86
  asm_code.addr = ((uintptr_t)prototype + offset + sizeof(uintptr_t)) - ((uintptr_t)code + ihsize);
#endif

  asm_code.data = (uintptr_t)data;
  memcpy(code, prototype, offset);
  memcpy(code + offset, &asm_code, sizeof(asm_code));

  return any(code);

err:
  u_free_if(code);
  return nullptr;
}
